/*
 * Settings.cpp
 *
 *  Created on: 3 Feb 2017
 *      Author: sstoja00
 *
 */
#ifndef SETTINGS_H_
#include "Settings.h"
#endif

#ifndef ARDUINOLOG_H_
#define ARDUINOLOG_H_
#include "ArduinoLog.h"
#endif

/*
 * initialize settings
 */
/*
* TODO: implement to read some of this from EPROM
*/

// Define the static Singleton pointer
Settings * Settings::inst_ = NULL;

Settings * Settings::getInstance() {
	Log.trace(F("Settings.getInstance() function call."CR));
	if (inst_ == NULL) {
		Log.trace(F("\tThere is no settings instance created - creating the instance."CR));
		inst_ = new Settings();
   	} else {
		Log.trace(F("\tThere is already instance  - sending existing one."CR));
	}

	Log.trace(F("Settings.getInstance() function exit."CR));
	return(inst_);
}



void Settings::init()
{
	Log.trace(F("Settings.init() function call. Settings instance id: %d"CR), this);
	velocity=127;
	octave=1;
	channel=1;
	noteOff=true;
	doubleNote=false;
	chordPlaying= false;
	Log.trace(F("Settings.init() function exit. Settings instance id: %d"CR), this);
};


/*
 * set velocity
 */
void Settings::setVelocity(int newVelocity)
{
	Log.trace(F("Settings.setVelocity() function call. Settings instance id: %d"CR), this);
	velocity=newVelocity;
	Log.trace(F("Settings.setVelocity() function exit. Settings instance id: %d"CR), this);
};

/*
 * set Octave
 */
void Settings::setOctave(int newOctave)
{
	Log.trace(F("Settings.setOctave() function call. Settings instance id: %d"CR), this);
	octave=newOctave;
	Log.trace(F("Settings.setOctave() function exit. Settings instance id: %d"CR), this);
};



/*
 * set Channel
 */
void Settings::setChannel(int newChannel)
{
	Log.trace(F("Settings.setChannel() function call. Settings instance id: %d"CR), this);
	channel=newChannel;
	Log.trace(F("Settings.setChannel() function exit. Settings instance id: %d"CR), this);
};


/*
 * wet note off settings
 */
void Settings::setNoteOff(bool newNoteOff)
{
	Log.trace(F("Settings.setNoteOff() function call. Settings instance id: %d"CR), this);
	noteOff=newNoteOff;
	Log.trace(F("Settings.setNoteOff() function exit. Settings instance id: %d"CR), this);
}

/*
 * set double note settings
 */
void Settings::setDoubleNote(bool newDoubleNote)
{
	Log.trace(F("Settings.setDoubleNote() function call. Settings instance id: %d"CR), this);
	doubleNote=newDoubleNote;
	Log.trace(F("Settings.setDoubleNote() function exit. Settings instance id: %d"CR), this);
}

/*
 * toggle double note settings
 */
void Settings::toggleDoubleNote()
{
	Log.trace(F("Settings.toggleDoubleNote() function call. Settings instance id: %d"CR), this);
	doubleNote=!doubleNote;
	Log.trace(F("Settings.toggleDoubleNote() function exit. Settings instance id: %d"CR), this);
};

/*
 * toggle note off  settings
 */
void Settings::toggleNoteOff()
{
	Log.trace(F("Settings.toggleNoteOff() function call. Settings instance id: %d"CR), this);
	noteOff=!noteOff;
	Log.trace(F("Settings.toggleNoteOff() function exit. Settings instance id: %d"CR), this);
};

/*
 * get Velocity
 */
int Settings::getVelocity()
{
	Log.trace(F("Settings.getVelocity() function call. Settings instance id: %d"CR), this);
	return velocity;
	Log.trace(F("Settings.getVelocity() function exit. Settings instance id: %d"CR), this);
}

/*
 * get Octave
 */
int Settings::getOctave()
{
	Log.trace(F("Settings.getOctave() function call. Settings instance id: %d"CR), this);
	return octave;
	Log.trace(F("Settings.getOctave() function exit. Settings instance id: %d"CR), this);
}


/*
 * get Note Off
 */
bool Settings::getNoteOff()
{
	Log.trace(F("Settings.getNoteOff() function call. Settings instance id: %d"CR), this);
	return noteOff;
	Log.trace(F("Settings.getNoteOff() function exit. Settings instance id: %d"CR), this);
}


/*
 * get Docubel Note
 */
bool Settings::getDoubleNote()
{
	Log.trace(F("Settings.getDoubleNote() function call. Settings instance id: %d"CR), this);
	return doubleNote;
	Log.trace(F("Settings.getDoubleNote() function exit. Settings instance id: %d"CR), this);
}

/*
 * get Channel
 */
int Settings::getChannel()
{
	Log.trace(F("Settings.getChannel() function call. Settings instance id: %d"CR), this);
	return channel;
	Log.trace(F("Settings.getChannel() function exit. Settings instance id: %d"CR), this);
}

char Settings::getChordType(int type)
{
	Log.trace(F("Settings.getChordType() function call. Settings instance id: %d"CR), this);
	return chordType[type];
	Log.trace(F("Settings.getChordType() function exit. Settings instance id: %d"CR), this);
}

bool Settings::getChordPlaying()
{
	Log.trace(F("Settings.getChordPlaying() function call. Settings instance id: %d"CR), this);
	return chordPlaying;
	Log.trace(F("Settings.getChordPlaying() function exit. Settings instance id: %d"CR), this);

}

void Settings::setChordPlaying(bool myChordPlaying)
{
	Log.trace(F("Settings.setChordPlaying() function call. Settings instance id: %d"CR), this);
	chordPlaying = myChordPlaying;
	Log.trace(F("Settings.setChordPlaying() function exit. Settings instance id: %d"CR), this);
}


