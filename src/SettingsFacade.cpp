/*
 * Settings.cpp
 *
 *  Created on: 3 Feb 2017
 *      Author: sstoja00
 *
 */
#ifndef SETTINGS_FACADE_H_
#include "SettingsFacade.h"
#endif

#ifndef SETTINGS_H_
#include "Settings.h"
#endif

Settings * facade_settings = Settings::getInstance();


/*
 * set Velocity Facade
 */
void setVelocity(int arg) {

    facade_settings->setVelocity(arg);
}

/*
 * set Octave Facade
 */
void setOctave(int newOctavem)
{
    facade_settings->setOctave(newOctavem);
};



/*
 * set Channel Note Facade
 */
void setChannelNote(int newChannel)
{
	facade_settings->setChannelNote(newChannel);
};

/*
 * set Channel Chord Facade
 */
void setChannelChord(int newChannel)
{
	facade_settings->setChannelChord(newChannel);
};


/*
 * wet note off settings Facade
 */
void setNoteOff(bool newNoteOff)
{
	facade_settings->setNoteOff(newNoteOff);
}

/*
 * set double note settings Facade
 */
void setDoubleNote(bool newDoubleNote)
{
    facade_settings->setDoubleNote(newDoubleNote);
}

/*
 * toggle double note settings Facade
 */
void toggleDoubleNote()
{
    facade_settings->toggleDoubleNote();
};

/*
 * toggle note off  settings Facade
 */
void toggleNoteOff()
{
    facade_settings->toggleNoteOff();
};

/*
 * get Status Facade
 */
int getState() {
    return facade_settings->getState();
}


/*
 * set Status  Facade
 */
void setState(int index) {
    return facade_settings->setState(index);
}

/*
 * set Chord Set Facade
 */
void setChordSet(int index) {
    return facade_settings->setChordSet(index);
}

/*
 * get Velocity Facade
 */
int getVelocity() {
    return facade_settings->getVelocity();
}

/*
 * get Octave Facade
 */
int getOctave() {
    return facade_settings->getOctave();
}


/*
 * get Note Off Facade
 */
bool getNoteOff() {
    return facade_settings->getNoteOff();
}


/*
 * get Docuble Note Facade
 */
bool getDoubleNote() {
    return facade_settings->getDoubleNote();
}

/*
 * get Channel Note Facade
 */
int getChannelNote() {
    return facade_settings->getChannelNote();
}

/*
 * get Channel Chord Facade
 */
int getChannelChord() {
    return facade_settings->getChannelChord();
}

/*
 * get Chord Set Facade
 */
int getChordSet() {
    return facade_settings->getChordSet();
}

int (*getChordsSetPointer())[22] {
    return facade_settings->getChordsSetPointer();
}

int getChordsSetRootNote(int chordsSet) {
  return facade_settings->getChordsSetRootNote(chordsSet);
}

int getChordsSetMode(int chordsSet) {
  return facade_settings->getChordsSetMode(chordsSet);
}

int getChordsSetOctave(int chordsSet) {
  return facade_settings->getChordsSetOctave(chordsSet);
}

void setChordsSetRootNote(int chordsSet, int rootNote) {
  return facade_settings->setChordsSetRootNote(chordsSet, rootNote);
}

void setChordsSetMode(int chordsSet, int mode) {
  return facade_settings->setChordsSetMode(chordsSet, mode);
}

void setChordsSetOctave(int chordsSet, int octave) {
  return facade_settings->setChordsSetOctave(chordsSet, octave);
}

/*
 * set Octave Facade
 */
void configChord(int chordSet, int rootNote, int chordType)
{
    facade_settings->configChord(chordSet, rootNote,chordType);
};