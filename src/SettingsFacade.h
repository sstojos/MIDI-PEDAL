/*
 * Settings Facade
 *
 * Created on: 3 Feb 2017
 *      Author: sstoja00
 */
#ifndef SETTINGS_FACADE_H_
#define  SETTINGS_FACADE_H_

#include <stdbool.h>

#pragma once
#ifdef __cplusplus
extern "C"
{
#endif

     void setVelocity(int);
     void setOctave(int);
     void setChannel(int);
     void setNoteOff(bool);
     void setDoubleNote(bool);
     void toggleDoubleNote();
     void toggleNoteOff();
     void setChordPlaying(bool);
     int getVelocity();
     int getOctave();
     bool getNoteOff();
     bool getDoubleNote();
     int getChannel();
     char getChordType(int);
     bool getChordPlaying();
#pragma once
#ifdef __cplusplus
}
#endif
#endif

