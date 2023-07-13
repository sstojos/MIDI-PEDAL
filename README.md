# MIDI-PEDAL Project

![Application screenshot](/assets/images/screen-shot.png)

## Introduction

This is a project for creating easy configurable midi-pedal that can play notes, chords and midi programs/commands.

The goal of this project is to create a fully customizable midi pedal that can be built inexpensively  with commodity hardware readily available. 

## Minimum Prototype Hardware requirements

Application is designed to run on ESP32 S3 platform, specifically for device with 4.3" capacitive touch screen ESP32-8048S043C with resolution of 800x480 that can be purchased from aliexpress for less than 20$.

Specification of this device can be found [here](assets/ESP32-8048S043 Specifications-EN.pdf).

It supports bare bone pedal hardware Doepfer Bass Pedal Fatar PD/3.

![Doepfer fatar pd/3](/assets/images/doepfer-fatar-pd3.jpg)

ESP32 S3 diplay device has only 7 GPIOs. That is not enough to control 13 midi note pedal that requires 10 GPIOs plus additional serial TX output pin for MIDi out.

Theefore there is a need for i2c mulitplexer such as this PCF 8575.

![PCF8575](/assets/images/PCF-8575-i2c-expander.jpg)

All these components need to be connected together that can be done by using a simple 4x6cm prototype PCB board like this:



## Production ready  hardware 

Schematics and PCB design of production ready hardware can be found in a dedicated repository called ESP32-8048S043C-EXPANSION-BOARD. 

You can use this repository for producting PCB at home 

Alternaorvely you can contact me to purchase already built hardware that you just need to plugin in together.


## Software design

This repository maintains source code using Visual Studio Code Platformio.

UI is developed using the lvgl library which  can run natively on the desktop.
If you want to work on UI design in windows please use MIDI-PEDAL-UI-WIN repository that is designed to run out of box inside Code::Blocks on windows.


 



