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
 * set Channel Facade
 */
void setChannel(int newChannel)
{
	facade_settings->setChannel(newChannel);
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
 * set Chord Playing Facade
 */
void setChordPlaying(bool myChordPlaying) {
    return facade_settings->setChordPlaying(myChordPlaying);
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
 * get Channel Facade
 */
int getChannel() {
    return facade_settings->getChannel();
}

/*
 * get Chord Type Facade
 */
char getChordType(int type)  {
    return facade_settings->getChordType(type);
}

/*
 * get Chord Playing Facade
 */
bool getChordPlaying() {
    return facade_settings->getChordPlaying();
}


