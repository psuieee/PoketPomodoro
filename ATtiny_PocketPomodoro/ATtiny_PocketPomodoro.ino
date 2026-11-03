/* ATtiny 85 Pocket Pomodoro
 *  By Aaron Sabu
 *  
 *  This handy timing device automatically sets a timer for work, and bounces between a work and rest period to keep you
 *  fcoused and take consistent breaks.
 *  
 *  FEATURES:
 *  - 12/24hr clock with date and day of week
 *  - Pomodoro timer that switches between a work and rest period
 *  - Track number of pomodoro reps
 *  - Clock setting and toggling between 12/24hr formats
 *  - Pause/reset pomodoro timer
 *  - 5 Button input
 *  - Buzzer with a mute function
 *  - 128x64 OLED screen
 */


// --- LIBRARIES ---
#include <uRTCLib.h>      // Library to talk to the DS3231 Real Time Clock chip
#include <TinyWireM.h>    // The I2C library specifically made for the ATtiny85
#include <Tiny4kOLED.h>   // Library to control the tiny OLED screen

// Set up RTC communication with I2C (0x68 is the standard I2C address for the DS3231)
uRTCLib rtc(0x68);

// --- TEXT ARRAYS ---
// "PROGMEM" is a special command. It tells the ATtiny85 to store these long text strings 
// in its main storage memory (Flash) instead of its tiny working memory (RAM) so it doesn't crash.
const char daysOfTheWeek[7][10] PROGMEM = { "SUNDAY", "MONDAY", "TUESDAY", "WEDNESDAY", "THURSDAY", "FRIDAY", "SATURDAY" };
const char monthsOfTheYear[12][4] PROGMEM = { "JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC" };

// --- HARDWARE PINS ---
#define BUTTON_PIN A3 // The analog pin reading the voltage from our 5 buttons
#define BUZZER_PIN 4  // The digital pin that turns the buzzer on and off

// --- POMODORO SETTINGS ---
uint16_t workTimer = 1500; // Default work time (in seconds for testing purposes)
uint16_t restTimer = 180; // Default rest time (in seconds)

// --- STATE VARIABLES ---
// These variables act as the "memory" for what the device is currently doing
uint16_t timerVal = 0;   // The current countdown number on the screen
uint8_t reps = 0;        // How many pomodoro cycles we've completed (the 'X' marks)
bool workMode = true;    // True = Work time, False = Rest time
bool timerMode = false;  // True = Timer is running, False = Timer is paused
bool lastTimerMode = false; // Remembers if the timer was running before we entered the settings menu

// --- CLOCK MEMORY VARIABLES ---
// These remember the *last* known time so the screen only updates when the numbers actually change
int8_t lastSecond = -1;
int8_t lastButtonVal = -1;
int8_t lastRTCSecond = -1; 
uint8_t current_h = 0;
uint8_t current_m = 0;
uint8_t current_s = 0;
uint8_t current_month = 0;
uint8_t current_day = 0;
uint8_t current_dayOfWeek = 0;

// --- BUTTON VOLTAGE THRESHOLDS ---
// Because 5 buttons share 1 pin, each button sends a different voltage (0 to 1023)
// These thresholds tell the code which voltage belongs to which button
#define THRESHOLD_1 81    // Center button
#define THRESHOLD_2 228   // Left button
#define THRESHOLD_3 337   // Up button
#define THRESHOLD_4 419   // Right button
#define THRESHOLD_5 600   // Down button
#define THRESHOLD_NONE 800 // No buttons pressed (voltage is very high)

// --- MENU VARIABLES ---
uint8_t currentMode = 0; // 0 = Normal, 1 = Set Hours, 2 = Set Mins, 3 = Set Secs

unsigned long lastBlinkTime = 0;
const uint16_t blinkInterval = 500; // Adjusts how frequently items blink when selected in the menu
bool blinkState = true;         
bool forceRedraw = false; // A special flag we can trigger to force the OLED to wipe and redraw itself

unsigned long lastRTCFetch = 0; 
bool use12HourFormat = true; // True = AM/PM mode, False = 24-hour military time

// --- Custom 8x16 Pause/Play icons ---
// These hex codes represent the pixels of our custom play/pause icons.
const uint8_t icon_play[] PROGMEM = {
  0x00, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x00,
  0x00, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00
};

const uint8_t icon_pause[] PROGMEM = {
  0x00, 0x00, 0xFE, 0xFE, 0x00, 0xFE, 0xFE, 0x00,
  0x00, 0x00, 0x7F, 0x7F, 0x00, 0x7F, 0x7F, 0x00
};

// --- Vars for Auto-Repeat when button held ---
unsigned long buttonPressTime = 0; // Records the exact millisecond a button was pressed
unsigned long lastRepeatTime = 0;  // Records the last time the auto-repeater triggered
const uint16_t HOLD_DELAY = 800;   // Wait 800ms before auto-repeating starts   
const uint8_t REPEAT_RATE = 150;   // Once repeating starts, trigger every 150ms      
bool handledLongPress = false; // A lock that ensures a 3-second hold only triggers its action once
bool mute = false; // True = Buzzer makes no sound

// --- HELPER FUNCTION: Adjust Time ---
// The '&' symbol means "Pass by Reference". Instead of passing a copy of the number, 
// this function modifies the actual current_h, current_m, or current_s variable directly.
void adjustTime(uint8_t &val, uint8_t limit, uint8_t btn) {
  if (btn == 3) val = (val >= limit) ? 0 : val + 1; // Up button (Left side on D-pad): Increase number, loop to 0 if at max
  else          val = (val == 0) ? limit : val - 1; // Down button (Right side on D-pad): Decrease number, loop to max if at 0
}

void setup() {
  // Define buzzer as output so it can push electricity to make sound
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Enable RTC module communication
  URTCLIB_WIRE.begin();

  /* * =======================================================================
     USE "rtc.set" TO SET THE DATE AND TIME
     =======================================================================
     Format: rtc.set(second, minute, hour, dayOfWeek, dayOfMonth, month, year);
   * * - second:     0-59
     - minute:     0-59
     - hour:       0-23 (Use 24-hour military time, e.g., 14 for 2:00 PM)
     - dayOfWeek:  1-7  (1 = Sunday, 2 = Monday, 3 = Tuesday, etc.)
     - dayOfMonth: 1-31
     - month:      1-12
     - year:       0-99 (Use the last two digits, e.g., 25 for 2025)
   * * Example below sets the clock to:
     Monday (2), April (4) 14th, 2025 (25) at 12:56:00
  */
  // rtc.set(0, 25, 13, 1, 14, 4, 25);

  // Setup OLED
  oled.begin(128, 64, sizeof(tiny4koled_init_defaults), tiny4koled_init_defaults);
  oled.enableChargePump(); // Provides the 7V+ needed to light up the OLED pixels
  oled.setRotation(1);     // Flips the screen orientation
  oled.setFont(FONT8X16);  // Sets the default font size
  oled.on();
  oled.clear();

  // Ensure that pomodoro starts out with the work timer first
  timerVal = workTimer;
  updateTimer();

    /* Button Numbers:
      0 = None pressed
      btn_1 = Center
      btn_2 = Left
      btn_3 = Up
      btn_4 = Right
      btn_5 = Down

      0 = None pressed
      btn_1 = Center
      btn_2 = Up
      btn_3 = Right
      btn_4 = Left
      btn_5 = Down
  */

  /* current_Mode var:
      0 = Normal operation
      1 = Set hours
      2 = Set minutes
      3 = Set seconds
  */
}

// helper function to print spaces when needed (used to erase old text from the screen)
void printSpaces(uint8_t count) {
  for (uint8_t i = 0; i < count; i++) {
    oled.write(' '); 
  }
}

void loop() {
  if (currentMode == 0) { // If we are in the normal clock mode (not setting the time)
    // ONLY update clock every 500ms. This prevents the ATtiny85 from freezing up 
    // trying to read the clock thousands of times a second, keeping buttons responsive.
    if (millis() - lastRTCFetch >= 500) {
      rtc.refresh();
      lastRTCFetch = millis();
      int8_t realRTCSecond = rtc.second();

      // Check if a physical real-world second has ticked by
      if (realRTCSecond != lastRTCSecond) {
        if (timerMode) { // If the pomodoro timer is currently playing
          if (timerVal > 0) {
            timerVal -= 1; // Tick the timer down by exactly 1 second
            
          // allow timers to bounce back and forth between work and rest
          } else {
            if (workMode == true) {
              victoryChime(); 
              workMode = false;
              timerVal = restTimer;
            } else {
              workAlertChime(); 
              updateReps();
              workMode = true;
              timerVal = workTimer;
            } 
          } 
        } 
        
        lastRTCSecond = realRTCSecond;
        forceRedraw = true; // force the OLED to refresh and update the numbers
      } 

      current_h = rtc.hour();
      current_m = rtc.minute();
      current_s = realRTCSecond;
      current_month = rtc.month();
      current_day = rtc.day();
      current_dayOfWeek = rtc.dayOfWeek();
    }
  }

  // --- BUTTON READING LOGIC ---
  uint8_t btn = readDPad();
  bool triggerAction = false; // When true, the specific action happens. Prevents stuff from happening 1000 times a second

  // when any button is pressed, evaluate if it's a new press, a hold, or an auto-repeat
  if (btn != 0) {
    if (btn != lastButtonVal) {
      // This is a brand new button press
      triggerAction = true;
      buttonPressTime = millis(); // Start the long-press stopwatch
      lastRepeatTime = millis();
      handledLongPress = false; 
    } else {

      // when down button is held, reset the timer
      if (btn == 5 && !handledLongPress) {
        // Check if the stopwatch has reached 3000ms (3 seconds)
        if (millis() - buttonPressTime >= 3000) {
          workMode = true;
          timerVal = workTimer;
          reps = 0;
          
          // Clear out the 'X' markings on the screen
          oled.setFont(FONT6X8);
          oled.setCursor(2, 7);
          printSpaces(13);
          oled.setFont(FONT8X16); 
          
          handledLongPress = true; // Lock it so it doesn't reset 100 times a second
          forceRedraw = true; 
        }
      }
      
      // Auto-repeat logic for Left and Right buttons (used for scrolling through times quickly)
      if (millis() - buttonPressTime >= HOLD_DELAY) {
        if (btn == 3 || btn == 4) { 
          // If enough time has passed since the last tick, trigger the action again
          if (millis() - lastRepeatTime >= REPEAT_RATE) {
            triggerAction = true;
            lastRepeatTime = millis();
          }
        }
      }
    }
  }
  
  // --- BUTTON ACTIONS ---
  // Only process these actions if the button logic above gave us the "Go" signal
  if (triggerAction) {
    // Beep unless mute is on, or if we are just holding the down button
    if (btn != lastButtonVal && btn != 5 && !mute) {
      beep(); 
    }

    // When center button is pressed, cycle through setting hours, minutes, seconds
    if (btn == 1) {
      if (currentMode == 0) {
        currentMode = 1; // Enter "Set Hours" mode
        lastTimerMode = timerMode; // Remember if Pomodoro was playing
        timerMode = false; // Pause Pomodoro while we set the clock
      } else {
        currentMode++;
        // Once exiting the set menu (mode > 3), save the new time to the RTC chip
        if (currentMode > 3) {
          currentMode = 0; 
          rtc.set(current_s, current_m, current_h, current_dayOfWeek, current_day, current_month, rtc.year());
          timerMode = lastTimerMode; // Resume Pomodoro if it was playing before
        }
      }
      forceRedraw = true;
    }

    // When either left/right buttons are pressed, cycle thru numbers in clock set menu
    else if (btn == 3 || btn == 4) {
      if (currentMode == 1) adjustTime(current_h, 23, btn);
      else if (currentMode == 2) adjustTime(current_m, 59, btn);
      else if (currentMode == 3) adjustTime(current_s, 59, btn);
      forceRedraw = true; 
    }

    // Up button: In clock set mode, toggle AM/PM. In normal mode, Play/Pause Pomodoro
    else if (btn == 2) {
      if (currentMode != 0) {
        use12HourFormat = !use12HourFormat; // Toggle AM/PM format
        // Erase the old AM/PM text just in case
        oled.setCursor(24, 1);
        printSpaces(1);
        oled.setCursor(89, 1);
        printSpaces(1);
      } else {
        timerMode = !timerMode; // Standard play/pause
      }
      forceRedraw = true; 
    }

    // Down button: toggle mute function
    else if (btn == 5) {
      mute = !mute;
    }
  }
  
  lastButtonVal = btn; // Remember what button was pressed for the next loop

  // --- MENU BLINKING LOGIC ---
  if (currentMode != 0) {
    // If in settings menu, toggle the blink state every half second (500ms)
    if (millis() - lastBlinkTime >= blinkInterval) {
      blinkState = !blinkState;
      lastBlinkTime = millis();
      forceRedraw = true;
    }
  } else {
    blinkState = true; // Always stay solid if not in the menu
  }

  // --- DRAWING TO SCREEN ---
  // Only draw if the second has changed OR if we manually forced a redraw (like pressing a button)
  if (current_s != lastSecond || forceRedraw) {
    updateClock(current_h, current_m, current_s);
    lastSecond = current_s;
    forceRedraw = false;
    
    updateTimer(); 
  }
}

// --- DISPLAY UPDATE FUNCTIONS ---

void updateClock(uint8_t h, uint8_t m, uint8_t s) {
  oled.setCursor(32, 1);

  uint8_t display_h = h;
  bool isPM = false;

  // --- VERY CHEAP 12-HOUR MATH ---
  if (use12HourFormat) {
    oled.setCursor(24, 1);
    if (h >= 12) {
      isPM = true;
      if (h > 12) display_h = h - 12; // Converts 13-23 to 1-11
    }
    if (h == 0) display_h = 12;       // Converts Midnight (0) to 12
  }

  // Print Hours (If in 'set hours' mode and blinkState is false, print blank spaces instead of the number to create a blink effect)
  if (currentMode == 1 && !blinkState) printSpaces(2);
  else printTwoDigits(display_h); // <--- Use the intercepted variable here!

  oled.write(':'); 

  // Print Minutes
  if (currentMode == 2 && !blinkState) printSpaces(2);
  else printTwoDigits(m);

  oled.write(':'); 

  // Print Seconds
  if (currentMode == 3 && !blinkState) printSpaces(2);
  else printTwoDigits(s);

  // --- PRINT AM / PM ---
  if (use12HourFormat) {
    oled.setFont(FONT6X8); 
    oled.setCursor(90, 2); 
    
    if (isPM) oled.write('P'); 
    else oled.write('A');      
    
    oled.write('M');
    oled.setFont(FONT8X16); 
  } else {
    printSpaces(2); // Erase AM/PM if switched back to 24-hour format
  }

  // Print the Day of the Week
  oled.setFont(FONT6X8);
  oled.setCursor(2, 0);
  oled.print((const __FlashStringHelper*)daysOfTheWeek[current_dayOfWeek - 1]);
  printSpaces(2);

  // Print the Date (e.g., 14 APR)
  oled.setCursor(86, 0);
  printTwoDigits(current_day);
  oled.write(' ');
  oled.print((const __FlashStringHelper*)monthsOfTheYear[current_month - 1]);
  oled.setFont(FONT8X16);
}

// Helper function to make sure numbers under 10 look like "09" instead of just "9"
void printTwoDigits(uint8_t number) {
  if (number < 10) {
    oled.write('0');
  }
  oled.print(number);
}

void updateTimer() {
  oled.setCursor(18, 4);
  
  // F() is a macro that stores this text in Flash memory instead of RAM (like PROGMEM)
  if (workMode) oled.print(F("Work "));
  else oled.print(F("Rest "));
  
  // Draw the custom play/pause icons based on the timerMode state
  if (timerMode) oled.bitmap(55, 4, 63, 6, icon_play);
  else oled.bitmap(55, 4, 63, 6, icon_pause);
  
  oled.setCursor(70, 4); 

  // Math to convert total raw seconds (e.g. 150) into Minutes and Seconds (2:30)
  uint8_t displayMin = 0;
  uint16_t displaySec = timerVal; 
  while (displaySec >= 60) {
    displaySec -= 60;
    displayMin++;
  }
  
  printTwoDigits(displayMin); 
  oled.write(':'); 
  printTwoDigits((uint8_t)displaySec); 

  printSpaces(2);
  
  // Print a small 'M' in the corner if the system is muted
  oled.setFont(FONT6X8);
  oled.setCursor(120, 7);
  if (mute) oled.write('M'); 
  else oled.write(' ');      
  oled.setFont(FONT8X16);
}

// --- ANALOG BUTTON READING ---
// Reads the single analog pin and decides which button was pressed based on the voltage thresholds
uint8_t readDPad() {
  int adcValue = analogRead(BUTTON_PIN);

  if (adcValue > THRESHOLD_NONE) return 0; // No button pressed
  if (adcValue < THRESHOLD_1)    return 1; // Center
  if (adcValue < THRESHOLD_2)    return 2; // Left
  if (adcValue < THRESHOLD_3)    return 3; // Up
  if (adcValue < THRESHOLD_4)    return 4; // Right
  if (adcValue < THRESHOLD_5)    return 5; // Down

  return 0; 
}

// --- BUZZER FUNCTIONS ---
// Pulses the buzzer high and low very fast to create a sound wave.
// delayTime = pitch (higher number = deeper sound). cycles = length of the beep.
void playNote(unsigned int delayTime, unsigned int cycles) {
  if (mute) return; // Instantly exit if muted
  for (unsigned int i = 0; i < cycles; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(BUZZER_PIN, LOW);
    delayMicroseconds(delayTime);
  }
}

void beep() {
  playNote(125, 120); // A quick, high-pitched confirmation chirp
}

void workAlertChime() {
  playNote(250, 160); 
  delay(80);
  playNote(250, 160); // Two medium beeps
}

void victoryChime() {
  // A rising 4-note melody to celebrate finishing a work cycle
  playNote(956, 52);  
  delay(20);
  playNote(758, 66);  
  delay(20);
  playNote(638, 78);  
  delay(20);
  playNote(478, 262); 
}

// Add an 'X' to the bottom of the screen to track completed Pomodoros
void updateReps() {
  reps = (reps < 6) ? reps + 1 : 0; // Cap at 6 reps, then reset to 0

  oled.setFont(FONT6X8);
  
  // Wipe the previous X's clean
  oled.setCursor(2, 7);
  printSpaces(13); 
  
  oled.setCursor(2, 7); 
  
  // Draw the new total number of X's
  for (unsigned int i = 0; i < reps; i++) {
    oled.write('X'); 
    oled.write(' ');
  }
  
  oled.setFont(FONT8X16);
}
