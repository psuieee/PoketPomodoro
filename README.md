# Pocket Pomodoro 

## Overview 
Here's the instructions and resources to build your very own Pomodoro! In this repository you'll find a BOM (Bill of Materials), Instruction Set, 3D Files, and PCB designs. 

![Overview picture of all parts.](./PartsExploded.jpg)

# Instructions
The instructions are setup into multiple subgroups in order to better organize the project and make it easier to setup.

## What You'll Need

### Parts
- **Electronics:** everything is listed, with quantities and links, in the [BOM](./BOM/BOM%20-%20Pomodoro%20Timer.csv).
- **3D-printed parts:**
  - Top case (with the screen window, 5 button holes, and buzzer grille)
  - Battery holder (the base, with a slot for the slide switch)
  - PCB mounting plate (the black plate that sits between the battery holder and the PCB)
  - 5 button caps
- **Wire:** two short red wires and one short black wire (cut-down jumper wires work fine)

### Tools
- Soldering iron and solder (this build used 0.8 mm 60/40 rosin-core)
- Flush cutters
- Wire strippers
- Needle-nose pliers
- Tweezers
- Electrical tape
- 2.5 mm hex key (for the M3 socket-head screws)
- Recommended: a silicone work mat and a fume extractor

> **Safety:** 60/40 solder contains lead. Work somewhere ventilated and wash your hands when you're done.

## Assembling the PCB
Every part goes in from the **front** of the board (the side with the white labels) and gets soldered on the **back**.

### Step 1: Lay out your parts
Check that you have everything for the board: the PCB, OLED display, 8-pin IC socket, ATtiny85, DS3231 RTC module, 5 push buttons, passive buzzer, one 10k resistor, and four 2k resistors.

Look over the bare board before you start. Every part has its outline and name printed on it, so you can always check where something goes.

<img src="./Images/01-pcb-parts-layout.jpg" width="300" alt="All PCB parts laid out on a work mat"> <img src="./Images/02-bare-pcb.jpg" width="300" alt="Front of the bare Poket Pomodoro Mk2 PCB">

### Step 2: Solder the OLED display
1. Lay a strip of male header pins flat on the board where the bottom edge of the display will rest. It's only a spacer that keeps the display level, so it doesn't get soldered.
2. Put the display's 4 pins through the holes labeled **GND VCC SCL SDA**. The labels on the display should match the labels on the board.
3. Flip the board over and bend the pins slightly with needle-nose pliers so the display stays put. A strip of electrical tape over the display also helps hold it down.
4. Look at the board from the side to check that the display sits level on the spacer, then solder the 4 pins.

<img src="./Images/03-oled-header-spacer.jpg" width="300" alt="Header strip laid flat as a spacer under the display location"> <img src="./Images/04-oled-placed.jpg" width="300" alt="OLED display placed on the board">

<img src="./Images/05-oled-pins-bent.jpg" width="300" alt="Bending the display pins on the back of the board with pliers"> <img src="./Images/06-oled-level-check.jpg" width="300" alt="Side view showing the display sitting level on the spacer">

### Step 3: Set the display jumpers (JP1 and JP2)
JP1 and JP2, just left of the display, choose which display pin gets power and which gets ground. Each one is a row of three pads. Bridge the middle pad to one side with a blob of solder:

| Your display's pin order | JP1 | JP2 |
|---|---|---|
| **GND VCC SCL SDA** (the display in the BOM) | middle → **GND\*** | middle → **VCC\*** |
| **VCC GND SCL SDA** | middle → **VCC** | middle → **GND** |

You can see the bridged jumpers on the finished board in the photo in [Step 6](#step-6-solder-the-resistors).

### Step 4: Solder the IC socket
Put the 8-pin socket in the **ATtiny 85-20P** outline. Its notch goes on the same side as the notch in the outline. Solder it in.

Don't put the ATtiny85 in yet. It goes in near the end, in [Step 15](#step-15-insert-the-attiny85).

<img src="./Images/07-ic-socket.jpg" width="300" alt="8-pin IC socket next to the board"> <img src="./Images/08-ic-socket-placed.jpg" width="300" alt="IC socket placed in the ATtiny85 footprint">

### Step 5: Solder the buzzer, RTC module, and buttons
1. **Buzzer:** the underside of the buzzer is marked **+**. Put that pin in the hole marked **+** on the board.
2. **RTC module:** line its pins up with the **VCC SDA SCL NC GND** labels in the **DS3231 RTC** outline.
3. **Buttons:** press one into each of **SW1–SW5** until it sits flat against the board.
4. Solder everything.

<img src="./Images/09-remaining-pcb-parts.jpg" width="300" alt="Buzzer, RTC module, and buttons next to the board"> <img src="./Images/10-buzzer-polarity.jpg" width="300" alt="Underside of the buzzer showing its plus marking">

<img src="./Images/11-buzzer-rtc-buttons-placed.jpg" width="300" alt="Buzzer, RTC module, and buttons placed on the board">

### Step 6: Solder the resistors
Resistors can go in either way round. The values are printed next to each outline on the board:

| Position | Value | Color bands (5-band, 1%) |
|---|---|---|
| R1 | 10k | brown, black, black, red, brown |
| R2, R3, R4, R5 | 2k | red, black, black, brown, brown |

If you're not sure which is which, check them with a multimeter before soldering.

<img src="./Images/12-resistors-placed.jpg" width="300" alt="Finished front of the PCB with all resistors in place">

### Step 7: Trim the leads
Flip the board over and cut the extra leads just above each solder joint with flush cutters. While you're there, check every joint for solder bridges, especially on the RTC module and the IC socket.

<img src="./Images/13-trim-leads.jpg" width="300" alt="Trimming component leads on the back of the board with flush cutters">

