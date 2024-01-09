# MIDI-PEDAL Project

![Application screenshot](/assets/images/screen-shot.png)

## Introduction

This is a project for creating easy configurable midi pedal that can play midi notes, chords and programs/commands.

The goal of this project is to create a fully customizable midi pedal that uses affordable and inexpensive hardware readily available.

## Minimum Prototype Hardware requirements

Application is designed to run on ESP32 S3 platform, specifically for device with 4.3" capacitive touch screen such as ESP32-8048S043C with resolution of 800x480 that can be purchased from aliexpress for less than 20$.

Specification of this device can be found [here](assets/ESP32-8048S043 Specifications-EN.pdf).

It is designe to connect to bare bone pedal hardware Doepfer Bass Pedal Fatar PD/3.

![Doepfer fatar pd/3](/assets/images/doepfer-fatar-pd3.jpg)

ESP32 S3 display device has only 7 GPIOs. That is not enough to control 13 note foot pedal that require 10 GPIOs plus additional pin for serial TX for MIDi out as well as additional 4 GPIO pins for foor controll switches.

Therefore, in addition to ESP32 S3 hardware there is a need  for i2c GPIO expander such as PCF 8575.

![PCF8575](/assets/images/PCF-8575-i2c-expander.jpg)

All these components need to be connected together with all pull up resistors and MIDI out TX and 3V3 resistors for protection. 

This  can be done by using a simple 4x6cm prototype PCB board like this:

![prototype front](/assets/images/pcb-prototype-front.jpg) 
![prototype back](/assets/images/pcb-prototype-back.jpg)

## Production ready  hardware 

![expansion board pcb](assets/images/expansion-board-PCB.png)

Schematics and PCB design of production ready hardware can be found in a dedicated repository called ESP32-8048S043C-EXPANSION-BOARD. These files are created using KiCad application.

Alternatively, you can use this schematics and PCB design for producing PCB at home or you can simple re-order my latest order from PCBWay.

Finnaly, you can always contact me to check if there are still available PCBs to sell for reasonable price. I usually order new PCBs every few months in small quantities for testing new prototypes so often there are few left.
  
## 3D enclosure design

Design of 3D printable parts for Doepfer Fatar PD3 pedal can be found inside Doepfer Fatar PD3 enclosure repository. They are created using the Openscad application.

## Software design

This repository maintains source code using Visual Studio Code Platformio.

UI is developed using the lvgl library which  can run natively on the desktop.
If you want to work on UI design in windows please use MIDI-PEDAL-UI-WIN repository that is designed to run out of box inside Code::Blocks on windows.

## Zynthian synthisizer

This pedal can also be fully playable pedal using open source zynthian synth. 

Zynthian is a compelx and functionally rich synthisizer. It can be purchased as standalone device directly from https://zynthian.org/ and connecetd simple by MIDI cable from this MIDI pedal.

It is stronly recommended to buy zynthian standalone synth directly from zynthian because it has proffesional grade audio inputs and outputs as well as much more controll buttons for easy of use. 

However if you want to save the cost and still have very functional bass pedal you can build zynthian using stojos zynthian minimal hardware instructions for this pedal here. In this case zynthian can be  integral part of the pedal and not a seperata device. Instructions how to enclose stojos zynthian mini can be found inside 3D enclosure design repository.  





 



