# Pocket Pomodoro 

## Overview 
Here's the instructions and resources to build your very own Pomodoro! In this repository you'll find a BOM (Bill of Materials), Instruction Set, 3D Files, and PCB designs. 

![Overview picture of all parts.](./PartsExploded.jpg)

# Instructions
The instructions are setup into multiple subgroups in order to better organize the project and make it easier to setup.

1. [What You'll Need](#what-youll-need)
2. [Assembling the PCB](#assembling-the-pcb) (Steps 1–7)
3. [Wiring the Battery Holder](#wiring-the-battery-holder) (Steps 8–11)
4. [Final Assembly](#final-assembly) (Steps 12–20)

Click any photo to open it full size.

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
3. Put a strip of electrical tape over the display to hold it in place and flip it over.
4. Look at the board from the side to check that the display sits level on the spacer, then solder the 4 pins.

<img src="./Images/03-oled-header-spacer.jpg" width="300" alt="Header strip laid flat as a spacer under the display location"> <img src="./Images/04-oled-placed.jpg" width="300" alt="OLED display placed on the board">

<img src="./Images/05-oled-pins-bent.jpg" width="300" alt="Bending the display pins on the back of the board with pliers"> <img src="./Images/06-oled-level-check.jpg" width="300" alt="Side view showing the display sitting level on the spacer">

### Step 3: Set the display jumpers (JP1 and JP2)
This step can be ignored if your OLED pin polarity matches the PCB!
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

## Wiring the Battery Holder
The three AAA batteries in the holder are connected in series. You'll run the **+** side through the slide switch and then send both wires up to the PCB.

### Step 8: Gather the enclosure parts
You'll need the top case, battery holder, black mounting plate, 5 button caps, 4 M3 × 15 mm screws, the slide switch, red and black wire, and 3 AAA batteries.

Press the Keystone battery contacts into the battery holder if they aren't in already (they're already in place in the photo).

<img src="./Images/14-enclosure-parts.jpg" width="300" alt="All enclosure parts laid out: case, battery holder, mounting plate, screws, button caps, switch, wires, and batteries">

### Step 9: Wire the positive (+) contact to the switch
1. Strip both ends of a red wire. Solder one end to the tab of the **flat (+)** end contact.
2. Route the wire around the outside of the battery tray toward the switch slot.
3. Push the slide switch into its slot and solder the red wire to one of the switch's **outer** pins.

<img src="./Images/15-battery-positive-wire.jpg" width="300" alt="Holding a red wire to the positive battery contact tab"> <img src="./Images/16-route-positive-wire.jpg" width="300" alt="Red wire routed around the battery tray to the slide switch">

<img src="./Images/17-positive-wire-to-switch.jpg" width="300" alt="Red wire soldered to an outer pin of the slide switch">

### Step 10: Add the switch output and negative (−) wire
1. Solder a second red wire to the **middle** pin of the switch. This wire carries power to the PCB, and only while the switch is on.
2. Solder the black wire to the tab of the **spring (−)** end contact.

<img src="./Images/18-switch-wiring.jpg" width="300" alt="Slide switch with the battery wire on an outer pin and the output wire on the middle pin"> <img src="./Images/19-battery-negative-wire.jpg" width="300" alt="Black wire soldered to the negative battery contact tab">

### Step 11: Feed the wires through the holder
Pass the red wire from the switch's middle pin and the black wire up through the slot in the battery holder.

<img src="./Images/20-wires-through-slot.jpg" width="300" alt="Red and black wires looped up through the slot in the battery holder">

## Final Assembly

### Step 12: Add the mounting plate
Set the black mounting plate on top of the battery holder, feeding the red and black wires through the hole in the plate.

<img src="./Images/21-mounting-plate.jpg" width="300" alt="Black mounting plate lowered onto the battery holder with the wires fed through">

### Step 13: Solder the battery wires to the PCB
On the back of the PCB, find the two pads labeled **Batt in**:

| Wire | Pad |
|---|---|
| Red (from the switch's middle pin) | **TP1** |
| Black (from the − contact) | **TP2** |

Double-check this before soldering. Reversed power can damage the parts.

<img src="./Images/22-solder-batt-in.jpg" width="300" alt="Soldering the battery wires to the Batt in pads"> <img src="./Images/23-batt-in-closeup.jpg" width="300" alt="Close-up of the red and black wires soldered to the Batt in pads">

### Step 14: Mount the PCB
Fold the PCB down onto the mounting plate, front side up, and tuck the wires underneath.

<img src="./Images/24-pcb-onto-plate.jpg" width="300" alt="Lowering the PCB onto the mounting plate"> <img src="./Images/25-pcb-onto-plate-back.jpg" width="300" alt="Back of the PCB with the wires as it's lowered onto the plate">

### Step 15: Insert the ATtiny85
Push the ATtiny85 into its socket with its notch (or dot) on the same side as the socket's notch. Make sure all 8 legs go into the socket and none fold underneath.

> **Note:** The ATtiny85 has to be programmed with the Pocket Pomodoro firmware before it goes in.

<!-- TODO: link the firmware and flashing instructions once they're in the repo -->

<img src="./Images/26-insert-attiny85.jpg" width="300" alt="Inserting the ATtiny85 into its socket with tweezers">

### Step 16: Test it
Put the 3 AAA batteries in, matching the **+** and **−** markings, and slide the switch on. The display should show the day, date, and time, plus the **Work** timer. Try each button.

<img src="./Images/27-power-test.jpg" width="300" alt="Powered-on board showing the time and Work timer on the display">

If the display stays blank, check these in order:
1. The switch is on and the batteries are the right way round.
2. Red goes to **TP1** and black goes to **TP2** (Step 13).
3. JP1 and JP2 match your display's pin order (Step 3).
4. The ATtiny85 is the right way round with all legs in the socket (Step 15).
5. The display's 4 solder joints are good (Step 2).

### Step 17: Add the button caps
Turn the top case upside down and drop a button cap into each of the 5 button holes from the inside.

<img src="./Images/28-button-caps.jpg" width="300" alt="Placing button caps into the top case from the inside">

### Step 18: Put the case on
Hold the battery holder, plate, and PCB together and slide them into the top case. The display should line up with the window and the buttons with the caps.

<img src="./Images/29-stack-into-case.jpg" width="300" alt="Sliding the assembled stack into the top case">

### Step 19: Screw it together
1. Put the 4 M3 × 15 mm screws through the corner holes of the battery holder.
2. Before tightening the last ones, check that the switch and wires are seated and nothing is pinched.
3. Tighten all 4 with the 2.5 mm hex key. Stop when they're snug so you don't strip the plastic.

<img src="./Images/30-insert-screws.jpg" width="300" alt="Inserting an M3 screw into a corner of the battery holder"> <img src="./Images/31-tighten-screws.jpg" width="300" alt="Tightening a screw with a hex key">

<img src="./Images/32-check-wiring.jpg" width="300" alt="Checking that the switch and wires are seated under the battery tray"> <img src="./Images/33-remaining-screws.jpg" width="300" alt="Inserting the remaining screws">

### Step 20: Done!
Slide the switch on and start your first Pomodoro.

<img src="./Images/34-finished-front.jpg" width="300" alt="Finished Pocket Pomodoro showing the Work timer"> <img src="./Images/35-mk1-vs-mk2.jpg" width="300" alt="Finished Mk2 Pocket Pomodoro next to the original Mk1 board">
