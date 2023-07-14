# MIDI-PEDAL Project

![Application screenshot](/assets/images/screen-shot.png)

## Introduction

This is a project for creating easy configurable midi pedal that can play midi notes, chords and programs/commands.

The goal of this project is to create a fully customizable midi pedal that can be built inexpensively  with commodity hardware readily available. 

## Minimum Prototype Hardware requirements

Application is designed to run on ESP32 S3 platform, specifically for device with 4.3" capacitive touch screen ESP32-8048S043C with resolution of 800x480 that can be purchased from aliexpress for less than 20$.

Specification of this device can be found [here](assets/ESP32-8048S043 Specifications-EN.pdf).

It supports bare bone pedal hardware Doepfer Bass Pedal Fatar PD/3.

![Doepfer fatar pd/3](/assets/images/doepfer-fatar-pd3.jpg)

ESP32 S3 display device has only 7 GPIOs. That is not enough to control 13 midi note pedal that require 10 GPIOs plus additional serial TX output pin for MIDi out.

Therefore, there is a need for i2c GPIO expander such as this PCF 8575.

![PCF8575](/assets/images/PCF-8575-i2c-expander.jpg)

All these components need to be connected together with all pull up resistors and MIDI out TX and 3V3 resistors for protection. 

This  can be done by using a simple 4x6cm prototype PCB board like this:

![prototype front](/assets/images/pcb-prototype-front.jpg) 
![prototype back](/assets/images/pcb-prototype-back.jpg)

## Production ready  hardware 

![expansion board pcb](assets/images/expansion-board-PCB.png)

Schematics and PCB design of production ready hardware can be found in a dedicated repository called ESP32-8048S043C-EXPANSION-BOARD. These files are created by KiCad application.

You can use this repository for getting required artifacts for producing PCB at home.

Alternatively, you can contact me to purchase:

- professionally fabricated PCB
* all required components that need to be soldered together
+ all cables and screws required to connect PCB to esp32 display
  
##3D enclosure design

Design of 3D printable parts for Doepfer Fatar PD3 pedal can be found inside Doepfer Fatar PD3 enclosure repository. They are created using the Openscad application.

## Software design

This repository maintains source code using Visual Studio Code Platformio.

UI is developed using the lvgl library which  can run natively on the desktop.
If you want to work on UI design in windows please use MIDI-PEDAL-UI-WIN repository that is designed to run out of box inside Code::Blocks on windows.


 



