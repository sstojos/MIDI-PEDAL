/*
 * Settings Facade
 *
 * Created on: 3 Feb 2017
 *      Author: sstoja00
 */
#ifndef SETTINGS_FACADE_H_
#define  SETTINGS_FACADE_H_

#ifndef ARDUINO_H_
#define ARDUINO_H_
#include <Arduino.h>
#endif

#include <stdbool.h>

#pragma once
#ifdef __cplusplus
extern "C"
{
#endif


     void setFactoryReset(bool);

     int getOctave();
     void setOctave(int);

     int getVelocity();
     void setVelocity(int);

     int getChannelNote();
     void setChannelNote(int);

     int getChannelChord();
     void setChannelChord(int);

     bool getNoteOff();
     void setNoteOff(bool);
     void toggleNoteOff();

     bool getDoubleNote();
     void setDoubleNote(bool);
     void toggleDoubleNote();

     int getState();
     void setState(int);

     int getChordSet();
     void setChordSet(int);
     int (*getChordsSetPointer())[22];

     int getChordsSetRootNote(int);
     int getChordsSetMode(int);
     int getChordsSetOctave(int);

     void setChordsSetRootNote(int, int);
     void setChordsSetMode(int, int);
     void setChordsSetOctave(int, int);

	void configChord(int,int,int);

     void listSnapshots();
     bool loadSnapshot(const char *);
     bool saveAsSnapshot(const char *);
     void saveSnapshot();

     const char * getShapshotName();

#pragma once
#ifdef __cplusplus
}
#endif
#endif

