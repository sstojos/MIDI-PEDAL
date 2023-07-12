# MIDI-PEDAL Arduino Project

This is a project for creating easy configurable midi-pedal that can play notes, chords and midi commands.

The goal of this project is to create a fully customizable midi pedal that can be built inexpensively  with commodity hardware readily available. 

It is designed around ESP32 S3 platform,  specifically for device with capacitive touch screen ESP32-8048S043C that can be purchased from aliexpress for less than 20$.
It supports bare bone 13 note midi pedal PD/3 hardware that can be purchased from thoman [here](https://www.thomann.de/gb/doepfer_mbp25_electronic_with_1_pedal.htm).

This repository maintains source code using Visual Studio Code Platformio.

UI is developed using the lvgl library which  can run natively on the desktop.
If you want to work on UI design in windows please use MIDI-PEDAL-UI-WIN repository that is designed to run out of box inside Code::Blocks on windows.

ESP32 S3 device ESP32-8048S043C will need additional hardware for connecting to bare bone midi pedal. 
Schematics and PCB design of this hardware can be found in a dedicated repository called ESP32-8048S043C-EXPANSION-BOARD. 
Alternatively, you can contact me to purchase already built hardware that you just need to plugin. 
Go to the ESP32-8048S043C-EXPANSION-BOARD repository to find out more information about the minimum hardware that needs to be built  for a working midi pedal.

 



