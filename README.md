# SonarCTL MIDI

This is a simple MIDI controller firmware that uses a Pro Micro (ATmega32U4).

## Requirements 
- **ATmega32u4 MCU**:
  - You can use any MCU that uses the CPU as long as it matches a [Pro Micro](https://a.co/d/9XTsa5C) pin layout (if you're not editing the code.)
  - If you're going to edit the code, please do so carefully. The Pro Micro is easy to "brick" and I recommend wiring up a reset button using the RST and GND pin, for easy bootloader mode entry.
- **Slide Potentiometers/Faders**:
  - You need 5 10k ohm slide potentiometers. I used this [one](https://a.co/d/2hbaZEC), but you can choose one that is purely a slider with no PCB.
- **Other materials**
  - There are only two main components for this build, the MCU and the potentiometers. Now it's up to you how you want to wire them up.
  - You can use a breadboard or directly solder the wires to the pins.
