#include <TinyWireM.h>
#include <Tiny4kOLED.h>

#define BUTTON_PIN A3 // The analog pin reading the voltage

// Your current thresholds
#define THRESHOLD_1 81    
#define THRESHOLD_2 228   
#define THRESHOLD_3 337   
#define THRESHOLD_4 419   
#define THRESHOLD_5 600   
#define THRESHOLD_NONE 800 

uint8_t lastButton = 255; // Set to an impossible number to force a first-draw

void setup() {
  // Give the OLED a fraction of a second to fully power on at 8MHz
  delay(200);

  // Setup OLED
  oled.begin(128, 64, sizeof(tiny4koled_init_defaults), tiny4koled_init_defaults);
  oled.enableChargePump();
  oled.setRotation(1);
  oled.setFont(FONT8X16);
  oled.on();
  oled.clear();

  // Draw static text
  oled.setCursor(0, 0);
  oled.print(F("Button Tester"));
  oled.setCursor(0, 3);
  oled.print(F("Btn: "));
  oled.setCursor(0, 6);
  oled.print(F("ADC: "));
}

// Your exact button reading function
uint8_t readDPad() {
  int adcValue = analogRead(BUTTON_PIN);

  if (adcValue > THRESHOLD_NONE) return 0; 
  if (adcValue < THRESHOLD_1)    return 1; 
  if (adcValue < THRESHOLD_2)    return 2; 
  if (adcValue < THRESHOLD_3)    return 3; 
  if (adcValue < THRESHOLD_4)    return 4; 
  if (adcValue < THRESHOLD_5)    return 5; 

  return 0; 
}

void loop() {
  uint8_t currentButton = readDPad();
  int rawAdc = analogRead(BUTTON_PIN); // Read the raw voltage value for debugging

  // Only update the screen if the button state changes to prevent flickering
  if (currentButton != lastButton) {
    
    // Print the Button Number
    oled.setCursor(40, 3);
    if (currentButton == 0) {
      oled.print(F("None"));
    } else {
      oled.print(currentButton);
      oled.print(F("   ")); // Print spaces to clear the leftover letters from "None"
    }

    lastButton = currentButton;
  }

  // Always update the raw ADC value continuously so you can see voltage drift
  oled.setCursor(40, 6);
  oled.print(rawAdc);
  oled.print(F("  ")); // Clear leftover digits if the number shrinks from 1023 to 0

  delay(50); // A small delay to act as physical button debounce
}
