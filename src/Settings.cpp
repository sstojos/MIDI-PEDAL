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

#include <Preferences.h>

String snapshot_name;

Preferences preferences;

/*
 * initialize settings
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

bool Settings::getFactoryReset() {

  preferences.begin("config", false);
  int factory_reset = preferences.getInt("factory_reset", 1);
  preferences.end();

  if (factory_reset==1) {
    return true;
  } else {
    return false;
  }

}

void Settings::setFactoryReset(bool factory_reset) {

  preferences.begin("config", false);
  if (factory_reset) {
    preferences.putInt("factory_reset", 1);
  } else {
    preferences.putInt("factory_reset", 0);
  }
  preferences.end();

}

bool Settings::init()
{
	Log.trace(F("Settings.init() function call. Settings instance id: %d"CR), this);

  if (getFactoryReset()) {
    Log.trace(F("Settings.init() Device need factory reset"CR));

    Log.trace(F("Settings.init() Trying to format SPIFFS ..."CR));
    if (!SPIFFS.format()) {
      Log.trace(F("Settings.init() Could not format SPIFFS. Stopping everything"CR));
      return false;
    }
    Log.trace(F("Settings.init() SPIFFS formated."CR));

    Log.trace(F("Settings.init() Trying to mount SPIFFS ..."CR));
    if (!SPIFFS.begin(false)) {
      Log.trace(F(" Settings.init() Could not mount SPIFFS. Stopping everything"CR));
      return false;
    }
    Log.trace(F("Settings.init() SPIFFS mounted"CR));

    Log.trace(F("Settings.init() Initializing Settings C object data ..."CR));
    velocity=127;
    state=1;
    noteOff=true;
    snapshot_name = "/00/00-default";

    // configure default settings for the note playing
    octave=1;
    channelNote=1;
    doubleNote=false;

    // configure default settngs for chord playing
    channelChord=1;
    chordSet = 1;

    //Initialize chordsSet 1 to have root note C (0) be ionin mode (1) in octave 1
    setModeChords(1,0,1,1);
    //Initialize chordsSet 2 to have root note C# (1) be ionin mode (1) in octave 1
    setModeChords(2,1,1,1);
    //Initialize chordsSet 3 to have root note D (2) be ionin mode (1) in octave 1
    setModeChords(3,2,1,1);

    chordsSetPointer=chordsSetOne;

    Log.trace(F("Settings.init() Settings C object data initialized."CR));

    Log.trace(F("Settings.init() Trying to create empty patch files inside all banks ..."CR));
    for (int i=0; i<16; i++) {
      for (int j=0; j<4; j++) {
        if (!saveEmptySnapShotInternal(SPIFFS, i,j)) {
          Log.trace(F("Settings.init() Empty patch files not created. Exiting init() "CR));
          return false;
        }
      }
    }
    Log.trace(F("Settings.init() Empty patch files created."CR));

    Log.trace(F("Settings.init() Trying to rename /00/00-empty patch to /00/00-default ..."CR));
    if (!renameSnapShotInternal(SPIFFS, "/00/00-empty", "/00/00-default")) {
      Log.trace(F("Settings.init()  Could not rename /00/00-empty patch file to /00/00-default. Exiting init()"CR));
      return false;
    }
    Log.trace(F("Settings.init() /00/00-empty patch tofile renamed to /00/00-default."CR));
  
    Log.trace(F("Settings.init() Trying to save settings into /00/00-default ..."CR));
    if (!saveSnapshotInternal(SPIFFS, "/00/00-default")) {
      Log.trace(F("Settings.init() Could not save settings into /00/00-default. Exiting init()."CR));
      return false;
    }
    Log.trace(F("Settings.init() Settings saved inside the /00/00-default file."CR));

    Log.trace(F("Settings.init() Setting factory reset to false."CR));
    setFactoryReset(false);
    
    Log.trace(F("Settings.init() Sucesfuly initilized !!! Settings instance id: %d"CR), this);
    return true;

  } else {
    Log.trace(F("Device was initialize previously - Load default settings from /00/00-defaul file"CR));

    Log.trace(F("Settings.init() Trying to mount SPIFFS ..."CR));
    if (!SPIFFS.begin(false)) {
      Log.trace(F(" Settings.init() Could not mount SPIFFS. Exiting init()."CR));
      return false;
    }
    Log.trace(F("Settings.init() SPIFFS mounted"CR));

    Log.trace(F("Settings.init() Trying to load settings from /00/00-default ..."CR));

    if (!loadSnapshotInternal(SPIFFS, "/00/00-default")) {
      Log.trace(F(" Settings.init() Could not load settings from /00/00=default. Exiting init()."CR));
      return false;
    }
    
    Log.trace(F("Settings.init() Sucesfuly initilized !!! Settings instance id: %d"CR), this);
    return true;
  }

};

void Settings::configChord(int chordSet, int rootNote, int chordType) {

  Log.trace(F("Settings.configChords() function call with chordSet  %d, rootNote %d and chordType %d"CR), chordSet, rootNote, chordType);

  int myChordType = (chordType-chordType%10)/10;
  int chordVariant = 0;
  if (chordType != 0) {
    chordVariant = chordType%10-1;
  }


  // clean approriate chordset and set pointer to it
  switch (chordSet)
  {
  case 1:
    chordsSetPointer = chordsSetOne;
    break;
  case 2:
    chordsSetPointer = chordsSetTwo;
    break;
  case 3:
    chordsSetPointer = chordsSetThree;
    break;
  default:
    break;
  }

  //store the chord type in the ChordSet array
  chordsSetPointer[rootNote][21]=chordType;
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// | total notes playing | note1 octave | note1 index | note2 octave | note2 index | ... | note10 octave | note 10 index | chordType
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  switch (myChordType) {
    case 0:
      // if chord type is 0 set  to a signle note
      // set total number of notes that will be played
      chordsSetPointer[rootNote][0] = 1;
      // set a root note first - it is not in chord config
      chordsSetPointer[rootNote][1] = getChordsSetOctave(chordSet);
      chordsSetPointer[rootNote][2] = rootNote;
      break;
    case 1:
      // set total number of notes that will be played
      chordsSetPointer[rootNote][0] = chordMajorConfigs[chordVariant][0];
      // set a root note first - it is not in chord config
      chordsSetPointer[rootNote][1] = getChordsSetOctave(chordSet);
      chordsSetPointer[rootNote][2] = rootNote;
      // set the rest of the notes in the chord
      for (int i=1; i < chordMajorConfigs[chordVariant][0]; i++) {
        chordsSetPointer[rootNote][i*2+1] = getChordsSetOctave(chordSet);
        chordsSetPointer[rootNote][i*2+2] = rootNote+chordMajorConfigs[chordVariant][i];
      }
      break;
    case 2:
      // set total number of notes that will be played
      chordsSetPointer[rootNote][0] = chordMinorConfigs[chordVariant][0];
      // set a root note first - it is not in chord config
      chordsSetPointer[rootNote][1] = getChordsSetOctave(chordSet);
      chordsSetPointer[rootNote][2] = rootNote;
      // set the rest of the notes in the chord
      for (int i=1; i < chordMinorConfigs[chordVariant][0]; i++) {
        chordsSetPointer[rootNote][i*2+1] = getChordsSetOctave(chordSet);
        chordsSetPointer[rootNote][i*2+2] = rootNote+chordMinorConfigs[chordVariant][i];
      }
      break;
    case 3:
      // set total number of notes that will be played
      chordsSetPointer[rootNote][0] = chordDominantConfigs[chordVariant][0];
      // set a root note first - it is not in chord config
      chordsSetPointer[rootNote][1] = getChordsSetOctave(chordSet);
      chordsSetPointer[rootNote][2] = rootNote;
      // set the rest of the notes in the chord
      for (int i=1; i < chordDominantConfigs[chordVariant][0]; i++) {
        chordsSetPointer[rootNote][i*2+1] = getChordsSetOctave(chordSet);
        chordsSetPointer[rootNote][i*2+2] = rootNote+chordDominantConfigs[chordVariant][i];
      }
      break;
    case 4:
      // set total number of notes that will be played
      chordsSetPointer[rootNote][0] = chordDiminishedConfigs[chordVariant][0];
      // set a root note first - it is not in chord config
      chordsSetPointer[rootNote][1] = getChordsSetOctave(chordSet);
      chordsSetPointer[rootNote][2] = rootNote;
      // set the rest of the notes in the chord
      for (int i=1; i < chordDiminishedConfigs[chordVariant][0]; i++) {
        chordsSetPointer[rootNote][i*2+1] = getChordsSetOctave(chordSet);
        chordsSetPointer[rootNote][i*2+2] = rootNote+chordDiminishedConfigs[chordVariant][i];
      }
      break;
    case 5:
      // set total number of notes that will be played
      chordsSetPointer[rootNote][0] = chordSuspendedConfigs[chordVariant][0];
      // set a root note first - it is not in chord config
      chordsSetPointer[rootNote][1] = getChordsSetOctave(chordSet);
      chordsSetPointer[rootNote][2] = rootNote;
      // set the rest of the notes in the chord
      for (int i=1; i < chordSuspendedConfigs[chordVariant][0]; i++) {
        chordsSetPointer[rootNote][i*2+1] = getChordsSetOctave(chordSet);
        chordsSetPointer[rootNote][i*2+2] = rootNote+chordSuspendedConfigs[chordVariant][i];
      }
      break;
    case 6:
      // set total number of notes that will be played
      chordsSetPointer[rootNote][0] = chordAugmentedConfigs[chordVariant][0];
      // set a root note first - it is not in chord config
      chordsSetPointer[rootNote][1] = getChordsSetOctave(chordSet);
      chordsSetPointer[rootNote][2] = rootNote;
      // set the rest of the notes in the chord
      for (int i=1; i < chordAugmentedConfigs[chordVariant][0]; i++) {
        chordsSetPointer[rootNote][i*2+1] = getChordsSetOctave(chordSet);
        chordsSetPointer[rootNote][i*2+2] = rootNote+chordAugmentedConfigs[chordVariant][i];
      }
      break;
  }
  
  Log.trace(F("Settings.configChords() function Exit"CR));
}

void Settings::setModeChords(int chordSet, int rootNote, int mode, int myOctave) {
  Log.trace(F("generating the chords for chords set: %d, root note: %d, mode: %d and octave: %d"CR), chordSet, rootNote, mode, myOctave);

  // set settings array
  chordSetSettings[chordSet-1][0] = rootNote;
  chordSetSettings[chordSet-1][1] = mode;
  chordSetSettings[chordSet-1][2] = myOctave;

  // clean approriate chordset and set pointer to it
  switch (chordSet)
  {
  case 1:
    chordsSetPointer = chordsSetOne;
    break;
  case 2:
    chordsSetPointer = chordsSetTwo;
    break;
  case 3:
    chordsSetPointer = chordsSetThree;
    break;
  default:
    break;
  }

  // set all pedals to play a single note
  for (int i = 0; i < 13; i++)
  {
    chordsSetPointer[i][0] = 1; // set number of notes to be one
    chordsSetPointer[i][1]  = myOctave; // set octave for note 1 in the chord
    chordsSetPointer[i][2]  = i; // set note to be the same as pedalIndex
    chordsSetPointer[i][21] = 0; // set chord type sa undefined - this is not a chord
  }

  int offset = mode-1;

  // -----------------------------------------------------------------------------------------------------------------------------------
	// | total notes playing | note1 octave | note1 index | note2 octave | note2 index | ... | note10 octave | note 10 index | chordType
	// -----------------------------------------------------------------------------------------------------------------------------------

  // set the first of seven chords in the mode
  int tempPedalIndex = rootNote;
  chordsSetPointer[tempPedalIndex][0] = 3; // when programatically generating chords we set only three notes
  chordsSetPointer[tempPedalIndex][1] = myOctave; // set octave for note 1 in the chord
  chordsSetPointer[tempPedalIndex][2] =tempPedalIndex; // set note index for note 1 in the chord
  chordsSetPointer[tempPedalIndex][3] = myOctave; // set octave for note 2 in the chord
  chordsSetPointer[tempPedalIndex][4] = chordsSetPointer[tempPedalIndex][2] + ionianScale[0 + offset] + ionianScale[1 + offset]; // set note index for note 2 in the chord
  chordsSetPointer[tempPedalIndex][5] = myOctave; // set octave for note 3 in the chord
  chordsSetPointer[tempPedalIndex][6] = chordsSetPointer[tempPedalIndex][4] + ionianScale[2 + offset] + ionianScale[3 + offset]; // set note index for note 3 in the chord
  // evaluate what is the chord type (major, major or dimished) ad store it
  chordsSetPointer[tempPedalIndex][21] = evalChordType(tempPedalIndex, chordSet);

  // set the second of seven chords in the mode
	//               10      +2
  tempPedalIndex = (rootNote+ionianScale[0 + offset])%12;
  chordsSetPointer[tempPedalIndex][0] = 3; // when programatically generating chords we set only three notes
  chordsSetPointer[tempPedalIndex][1] = myOctave; // set octave for note 1 in the chord
  chordsSetPointer[tempPedalIndex][2] = tempPedalIndex; // set note index for note 1 in the chord
  chordsSetPointer[tempPedalIndex][3] = myOctave; // set octave for note 2 in the chord
  chordsSetPointer[tempPedalIndex][4] = chordsSetPointer[tempPedalIndex][2]  + ionianScale[1 + offset] + ionianScale[2 + offset]; // set note index for note 2 in the chord
  chordsSetPointer[tempPedalIndex][5] = myOctave; // set octave for note 3 in the chord
  chordsSetPointer[tempPedalIndex][6] = chordsSetPointer[tempPedalIndex][4] + ionianScale[3 + offset] + ionianScale[4 + offset]; // set note index for note 3 in the chord
  // evaluate what is the chord type (major, major or dimished) ad store it
  chordsSetPointer[tempPedalIndex][21] = evalChordType(tempPedalIndex, chordSet);

  // set the third of seven chords in the mode
  //               10      +2                      +2                      =14
  tempPedalIndex = (rootNote+ionianScale[0 + offset]+ionianScale[1 + offset])%12;
  chordsSetPointer[tempPedalIndex][0] = 3; // when programatically generating chords we set only three notes
  chordsSetPointer[tempPedalIndex][1] = myOctave; // set octave for note 1 in the chord
  chordsSetPointer[tempPedalIndex][2] = tempPedalIndex; // set note index for note 1 in the chord
  chordsSetPointer[tempPedalIndex][3] = myOctave; // set octave for note 2 in the chord
  chordsSetPointer[tempPedalIndex][4] = chordsSetPointer[tempPedalIndex][2] + ionianScale[2 + offset] + ionianScale[3 + offset]; // set note index for note 2 in the chord
  chordsSetPointer[tempPedalIndex][5] = myOctave; // set octave for note 3 in the chord
  chordsSetPointer[tempPedalIndex][6] = chordsSetPointer[tempPedalIndex][4] + ionianScale[4 + offset] + ionianScale[5 + offset]; // set note index for note 3 in the chord
  // evaluate what is the chord type (major, major or dimished) ad store it
  chordsSetPointer[tempPedalIndex][21] = evalChordType(tempPedalIndex, chordSet);

  // set the fourth of seven chords in the mode
  //                10      +2                      +2                      +1                      =15
  tempPedalIndex = (rootNote+ionianScale[0 + offset]+ionianScale[1 + offset]+ionianScale[2 + offset])%12;
  chordsSetPointer[tempPedalIndex][0] = 3; // when programatically generating chords we set only three notes
  chordsSetPointer[tempPedalIndex][1] = myOctave; // set octave for note 1 in the chord
  chordsSetPointer[tempPedalIndex][2] = tempPedalIndex; // set note index for note 1 in the chord
  chordsSetPointer[tempPedalIndex][3] = myOctave; // set octave for note 2 in the chord
  chordsSetPointer[tempPedalIndex][4] = chordsSetPointer[tempPedalIndex][2] + ionianScale[3 + offset] + ionianScale[4 + offset]; // set note index for note 2 in the chord
  chordsSetPointer[tempPedalIndex][5] = myOctave; // set octave for note 3 in the chord
  chordsSetPointer[tempPedalIndex][6] = chordsSetPointer[tempPedalIndex][4] + ionianScale[5 + offset] + ionianScale[6 + offset]; // set note index for note 3 in the chord
  // evaluate what is the chord type (major, major or dimished) ad store it
  chordsSetPointer[tempPedalIndex][21] = evalChordType(tempPedalIndex, chordSet);

   // set the fifth of seven chords in the mode
  tempPedalIndex = (rootNote+ionianScale[0 + offset]+ionianScale[1 + offset]+ionianScale[2 + offset]+ionianScale[3 + offset])%12;
  chordsSetPointer[tempPedalIndex][0] = 3; // when programatically generating chords we set only three notes
  chordsSetPointer[tempPedalIndex][1] = myOctave; // set octave for note 1 in the chord
  chordsSetPointer[tempPedalIndex][2] = tempPedalIndex; // set note index for note 1 in the chord
  chordsSetPointer[tempPedalIndex][3] = myOctave; // set octave for note 2 in the chord
  chordsSetPointer[tempPedalIndex][4] = chordsSetPointer[tempPedalIndex][2] + ionianScale[4 + offset] + ionianScale[5 + offset]; // set note index for note 2 in the chord
  chordsSetPointer[tempPedalIndex][5] = myOctave; // set octave for note 3 in the chord
  chordsSetPointer[tempPedalIndex][6] = chordsSetPointer[tempPedalIndex][4] + ionianScale[6 + offset] + ionianScale[7 + offset]; // set note index for note 3 in the chord
  // evaluate what is the chord type (major, major or dimished) ad store it
  chordsSetPointer[tempPedalIndex][21] = evalChordType(tempPedalIndex, chordSet);

  // set the sixth of seven chords in the mode
  //               10      +2                      +2                      +1                      +2                      +2                       =19
  tempPedalIndex = (rootNote+ionianScale[0 + offset]+ionianScale[1 + offset]+ionianScale[2 + offset]+ionianScale[3 + offset]+ionianScale[4 + offset])%12;
  chordsSetPointer[tempPedalIndex][0] = 3; // when programatically generating chords we set only three notes
  chordsSetPointer[tempPedalIndex][1] = myOctave; // set octave for note 1 in the chord
  chordsSetPointer[tempPedalIndex][2] = tempPedalIndex; // set note index for note 1 in the chord
  chordsSetPointer[tempPedalIndex][3] = myOctave; // set octave for note 2 in the chord
  chordsSetPointer[tempPedalIndex][4] = chordsSetPointer[tempPedalIndex][2] + ionianScale[5 + offset] + ionianScale[6 + offset]; // set note index for note 2 in the chord
  chordsSetPointer[tempPedalIndex][5] = myOctave; // set octave for note 3 in the chord
  chordsSetPointer[tempPedalIndex][6] = chordsSetPointer[tempPedalIndex][4] + ionianScale[7 + offset] + ionianScale[8 + offset]; // set note index for note 3 in the chord
  // evaluate what is the chord type (major, major or dimished) ad store it
  chordsSetPointer[tempPedalIndex][21] = evalChordType(tempPedalIndex, chordSet);

  // set the seventh of seven chords in the mode
  //                10      +2                      +2                      +1                      +2                      +2                      +2
  tempPedalIndex = (rootNote+ionianScale[0 + offset]+ionianScale[1 + offset]+ionianScale[2 + offset]+ionianScale[3 + offset]+ionianScale[4 + offset]+ionianScale[5 + offset])%12;;
  chordsSetPointer[tempPedalIndex][0] = 3; // when programatically generating chords we set only three notes
  chordsSetPointer[tempPedalIndex][1] = myOctave; // set octave for note 1 in the chord
  chordsSetPointer[tempPedalIndex][2] = tempPedalIndex; // set note index for note 1 in the chord
  chordsSetPointer[tempPedalIndex][3] = myOctave; // set octave for note 2 in the chord
  chordsSetPointer[tempPedalIndex][4] = chordsSetPointer[tempPedalIndex][2] + ionianScale[6 + offset] + ionianScale[7 + offset]; // set note index for note 2 in the chord
  chordsSetPointer[tempPedalIndex][5] = myOctave; // set octave for note 3 in the chord
  chordsSetPointer[tempPedalIndex][6] = chordsSetPointer[tempPedalIndex][4] + ionianScale[8 + offset] + ionianScale[9 + offset]; // set note index for note 3 in the chord
  // evaluate what is the chord type (major, major or dimished) ad store it
  chordsSetPointer[tempPedalIndex][21] = evalChordType(tempPedalIndex, chordSet);

  // check if both C pedals are the same - if not make them
  if ( chordsSetPointer[0][0] != chordsSetPointer[12][0]) {
    // one is a chord and one is a single note
    if (chordsSetPointer[0][0] == 3) {
      // top C need to be set with the same chord as bottom C but Octave higher
      chordsSetPointer[12][0] = chordsSetPointer[0][0];
      chordsSetPointer[12][1] = chordsSetPointer[0][1];
      chordsSetPointer[12][2] = chordsSetPointer[0][2]+12;
      chordsSetPointer[12][3] = chordsSetPointer[0][3];
      chordsSetPointer[12][4] = chordsSetPointer[0][4]+12;
      chordsSetPointer[12][5] = chordsSetPointer[0][5];
      chordsSetPointer[12][6] = chordsSetPointer[0][6]+12;
      chordsSetPointer[12][21] = chordsSetPointer[0][21];
      
    } else {
      // bottom C need ot be set with the same chord as top C but Octave lower
      chordsSetPointer[0][0] = chordsSetPointer[12][0];
      chordsSetPointer[0][1] = chordsSetPointer[12][1];
      chordsSetPointer[0][2] = chordsSetPointer[12][2]-12;
      chordsSetPointer[0][3] = chordsSetPointer[12][3];
      chordsSetPointer[0][4] = chordsSetPointer[12][4]-12;
      chordsSetPointer[0][5] = chordsSetPointer[12][5];
      chordsSetPointer[0][6] = chordsSetPointer[12][6]-12;
      chordsSetPointer[0][21] = chordsSetPointer[12][21];
    }
  }


  Log.trace(F("ChordSet 2-D array is set to following: "CR));
  for (int i = 0; i < 13; i++)
  { 
    Log.trace(F("{ "));
    for (int j = 0; j < 22; j++)
    {
        Log.trace(F("%d "), chordsSetPointer[i][j]);
    }
    Log.trace(F(" }"CR));
  }

}

//return the chord type (0 unknown, 11 major, 21 minor, 41 diminished)
//can be used only on generated chords
int Settings::evalChordType(int pedalIndex, int chordSet) {
  Log.trace(F("MidiPlayer.evalChordType() function called."CR));

  // set chord set pointer
  switch (chordSet)
  {
  case 1:
    chordsSetPointer = chordsSetOne;
    break;
  case 2:
    chordsSetPointer = chordsSetTwo;
    break;
  case 3:
    chordsSetPointer = chordsSetThree;
    break;
  default:
    break;
  }

  //  chordsSetPointer[pedalIndex][x]
  // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// | total notes playing | note1 octave | note1 index | note2 octave | note2 index | ... | note10 octave | note 10 index |
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

  if (chordsSetPointer[pedalIndex][0] < 3) {
    return 0; //unknown chord - less then 3 notes
  } else {
    if (chordsSetPointer[pedalIndex][4]-chordsSetPointer[pedalIndex][2] == 4) {
        return 11; // major chord
    } else  if (chordsSetPointer[pedalIndex][6]-chordsSetPointer[pedalIndex][4] == 4) {
        return 21; // minor chord
    } else {
        return 41; //diminished chord
    }
  } 
  Log.trace(F("MidiPlayer.evalChordType() function exit."CR));
}


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
 * set Channel Note
 */
void Settings::setChannelNote(int newChannel)
{
	Log.trace(F("Settings.setChannelNote() function call. Settings instance id: %d"CR), this);
	channelNote=newChannel;
	Log.trace(F("Settings.setChannelNote() function exit. Settings instance id: %d"CR), this);
};

/*
 * set Channel Chord
 */
void Settings::setChannelChord(int newChannel)
{
	Log.trace(F("Settings.setChannelChord() function call. Settings instance id: %d"CR), this);
	channelChord=newChannel;
	Log.trace(F("Settings.setChannelChord() function exit. Settings instance id: %d"CR), this);
};


/*
 * set note off settings
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
	Log.trace(F("Settings.getVelocity() function exit. Settings instance id: %d"CR), this);
	return velocity;
}

/*
 * get Octave
 */
int Settings::getOctave()
{
	Log.trace(F("Settings.getOctave() function call. Settings instance id: %d"CR), this);
	Log.trace(F("Settings.getOctave() function exit. Settings instance id: %d"CR), this);
	return octave;
}


/*
 * get Note Off
 */
bool Settings::getNoteOff()
{
	Log.trace(F("Settings.getNoteOff() function call. Settings instance id: %d"CR), this);
	Log.trace(F("Settings.getNoteOff() function exit. Settings instance id: %d"CR), this);
	return noteOff;
}


/*
 * get Double Note
 */
bool Settings::getDoubleNote()
{
	Log.trace(F("Settings.getDoubleNote() function call. Settings instance id: %d"CR), this);
	Log.trace(F("Settings.getDoubleNote() function exit. Settings instance id: %d"CR), this);
	return doubleNote;
}

/*
 * get Channel Note
 */
int Settings::getChannelNote()
{
	Log.trace(F("Settings.getChannelNote() function call. Settings instance id: %d"CR), this);
	Log.trace(F("Settings.getChannelNote() function exit. Settings instance id: %d"CR), this);
	return channelNote;
}

/*
 * get Channel Chord
 */
int Settings::getChannelChord()
{
	Log.trace(F("Settings.getChannelChord() function call. Settings instance id: %d"CR), this);
	Log.trace(F("Settings.getChannelChord() function exit. Settings instance id: %d"CR), this);
	return channelChord;
}




void Settings::setState(int index)
{
	Log.trace(F("Settings.setStatus() function call. Settings instance id: %d"CR), this);
	state = index;
	Log.trace(F("Settings.setStatus() function exit. Settings instance id: %d"CR), this);
}

int Settings::getState()
{
	Log.trace(F("Settings.getStatus() function call. Settings instance id: %d"CR), this);
	Log.trace(F("Settings.getStatus() function exit. Settings instance id: %d"CR), this);
	return state;
}

void Settings::setChordSet(int myChordSet)
{
	Log.trace(F("Settings.setChordSet() function call. Settings chordset to: %d"CR), myChordSet);

	chordSet = myChordSet;
  //set also chodsSetPointer
  switch (chordSet)
  {
  case 1:
  //  chordsSetOne[13][22] = {0};
    chordsSetPointer = chordsSetOne;
    break;
  case 2:
	//  chordsSetTwo[13][22] = {0};
    chordsSetPointer = chordsSetTwo;
    break;
  case 3:
	//  chordsSetThree[13][22] = {0};
    chordsSetPointer = chordsSetThree;
    break;
  default:
    break;
  }
  
	Log.trace(F("Settings.setChordSet() function."CR));
}

int Settings::getChordSet()
{
	Log.trace(F("Settings.getChordSet() function call."CR));
	Log.trace(F("Settings.getChordSet() function exit."CR));
	return chordSet;
}

int (*Settings::getChordsSetPointer())[22]
{
	Log.trace(F("Settings.getChordSetPointer() function call."CR));
  switch (getChordSet())
  {
  case 1:
    chordsSetPointer = chordsSetOne;
    break;
  case 2:
    chordsSetPointer = chordsSetTwo;
    break;
  case 3:
    chordsSetPointer = chordsSetThree;
    break;
  default:
    break;
  }
	Log.trace(F("Settings.getChordSetPointer() function exit."CR));
	return chordsSetPointer;
}

int Settings::getChordsSetRootNote(int chordsSet) {
	Log.trace(F("Settings.getChordsSetRootNote() function call."CR));
	Log.trace(F("Settings.getChordsSetRootNote() function exit."CR));
  return chordSetSettings[chordSet-1][0];
}

int Settings::getChordsSetMode(int chordsSet) {
	Log.trace(F("Settings.getChordsSetMode() function call."CR));
	Log.trace(F("Settings.getChordsSetMode() function exit."CR));
  return chordSetSettings[chordSet-1][1];
}

int Settings::getChordsSetOctave(int chordsSet) {
	Log.trace(F("Settings.getChordsSetOctave() function call."CR));
	Log.trace(F("Settings.getChordsSetOctave() function exit."CR));
  return chordSetSettings[chordSet-1][2];
}

void Settings::setChordsSetRootNote(int chordsSet, int rootNote) {
	Log.trace(F("Settings.setChordsSetRootNote() function call."CR));
  setModeChords(chordSet, rootNote, getChordsSetMode(chordSet), getChordsSetOctave(chordsSet));
	Log.trace(F("Settings.setChordsSetRootNote() function exit."CR));
}

void Settings::setChordsSetMode(int chordsSet, int mode) {
	Log.trace(F("Settings.setChordsSetMode() function call."CR));
  setModeChords(chordSet, getChordsSetRootNote(chordsSet), mode, getChordsSetOctave(chordsSet));
	Log.trace(F("Settings.setChordsSetMode() function exit."CR));
}

void Settings::setChordsSetOctave(int chordsSet, int octave) {
	Log.trace(F("Settings.setChordsSetOctave() function call."CR));
  // set settings array
  chordSetSettings[chordsSet-1][2] = octave;

  // clean approriate chordset and set pointer to it
  switch (chordsSet)
  {
  case 1:
  //  chordsSetOne[13][22] = {0};
    chordsSetPointer = chordsSetOne;
    break;
  case 2:
	//  chordsSetTwo[13][22] = {0};
    chordsSetPointer = chordsSetTwo;
    break;
  case 3:
	//  chordsSetThree[13][22] = {0};
    chordsSetPointer = chordsSetThree;
    break;
  default:
    break;
  }

  // chnage octave for all notes in all chords inside chord set array
  // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// | total notes playing | note1 octave | note1 index | note2 octave | note2 index | ... | note10 octave | note 10 index | chordType
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  for (int i = 0; i < 13; i++) {
    for (int j=0; j < chordsSetPointer[i][0]; j++) {
      chordsSetPointer[i][j*2+1] = octave;
    }
  }

	Log.trace(F("Settings.setChordsSetOctave() function exit."CR));
}

void Settings::listSnapshots() {
	Log.trace(F("Settings.listSnapshots() function call."CR));
	Log.trace(F("Settings.listSnapshots() function exit."CR));
  listSnapShotsInternal(SPIFFS, "/", 1);
}

void Settings::listSnapShotsInternal(fs::FS &fs, const char * dirname, uint8_t levels){
    Log.trace(F("Listing directory: %s"CR), dirname);

    File root = fs.open(dirname);
    if(!root){
        Log.trace(F("- failed to open directory"CR));
        return;
    }
    if(!root.isDirectory()){
        Log.trace(F(" - not a directory"CR));
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Log.trace(F("  DIR : %s"CR), file.name());
            if(levels){
                listSnapShotsInternal(fs, file.path(), levels -1);
            }
        } else {
            Log.trace(F("  FILE: %s, SIZE:"CR), file.name());
        }
        file = root.openNextFile();
    }
}


bool Settings::loadSnapshot(const char * path) {
	Log.trace(F("Settings.loadSnapshots() function call."CR));
	Log.trace(F("Settings.loadSnapshots() function exit."CR));
  return loadSnapshotInternal(SPIFFS, path);
}

bool Settings::loadSnapshotInternal(fs::FS &fs, const char * path) {
    Log.trace(F("Reading file: %s"CR), path);

    Log.trace(F("Trying to load snapshotfile  into Settings: %s"CR), path);

    File file = fs.open(path);
    if(!file || file.isDirectory()){
        Log.trace(F("- failed to open file for reading"CR));
        return false;
    }

    // read file until carriage return
    int tempVelocity[3];
    int velocityDigitIndex = 0;
    int velocityDigit;
    bool nextReadRequired = true;

    while(nextReadRequired) {
      velocityDigit = file.read();
      if (velocityDigit != 10) { // it is not carriage return
          tempVelocity[velocityDigitIndex] = velocityDigit-48;
          velocityDigitIndex++;
      } else { // it is carige return so stop reading velocity
        nextReadRequired= false;
      }

      if (velocityDigitIndex>3) {
        Log.trace(F("Something is wrong reading velocity 0 - there should be only three digits. Exiting loading snapshot.  Number of characters is: %d"CR), velocityDigitIndex);
        file.close();
        return false;
      }
    }
    switch (velocityDigitIndex) {
      case 0:
        velocity=0;
        break;
      case 1:
        velocity = tempVelocity[0];
        break;
      case 2:
        velocity = tempVelocity[0]*10 + tempVelocity[1];
        break;
      case 3:
        velocity = tempVelocity[0]*100 + tempVelocity[1]*10 + tempVelocity[2];
        break;
    }

    if ( velocity>127 || velocity <0 ) {
        Log.trace(F("Somethign is wrong reading velocity - velocity shold be in 0 - 127 range. Exiting loading snapshot."CR));
        file.close();
        return false;
    } 
    Log.trace(F("Finished loading velocity. Velocity is %d"CR), velocity);
  
    // ---------------------
    // load state
    // ---------------------
    Log.trace(F("Trying to load  state."CR));
    state = file.read()-48;
    if (state<1 || state>3) {
        Log.trace(F("Somethign is wrong reading state - state should be in range  1 - 3. Exiting loading snapshot."CR));
        file.close();
        return false;
    }
    //go to next line by reading carriage return
    if (file.read()!=10) {
        Log.trace(F("Somethign is wrong reading state. Exiting loading snapshot."CR));
        file.close();
        return false;
    }
    Log.trace(F("Finished loading state State is %d"CR), state);

    // ---------------------
    // load note off settings  - it is saved as 0 or 1
    // ---------------------
    Log.trace(F("Trying to load note off settings"CR));
    int intNoteOff = 10;
    intNoteOff = file.read()-48;

    if (intNoteOff !=0 && intNoteOff !=1) {
        Log.trace(F("Something is wrong reading note off - note off should be 0 or 1. Exiting loading snapshot. Note Off is: %d"CR), intNoteOff);
        file.close();
        return false;
    }

    if(intNoteOff == 1) { // noteoff is on - load it as true
      noteOff=true;
    }
    if(intNoteOff == 0) { // noteoff is off - load it as false
      noteOff=false;
    }
    //go to next line by reading carriage return
    if (file.read()!=10) {
        Log.trace(F("Somethign is wrong reading note off - no carriage retrun at the end. Exiting loading snapshot."CR));
        file.close();
        return false;
    }
    Log.trace(F("Finished loading note off settings. Note Off is %T"CR), noteOff);

    // ---------------------
    // load octave  - octave range is from -4 to 5 - it is written in the file as a signle custom char
    // ---------------------
    Log.trace(F("Trying to load octave  settings"CR));
    octave = file.read()-48;

    if (octave < -4 || octave  > 5) {
        Log.trace(F("Somethign is wrong reading octave - octave should be in range -4 to 5. Exiting loading snapshot."CR));
        file.close();
        return false;
    }
    //go to next line by reading carriage return
    if (file.read()!=10) {
        Log.trace(F("Somethign is wrong reading octave - no carriage retrun at the end. Exiting loading snapshot."CR));
        file.close();
        return false;
    }
    Log.trace(F("Finished loading octave settings. Octave is %d"CR), octave);

    // ---------------------
    // load channel note settings  - channelNote range is from 1 to 16 - it is written as a sinlle custom char
    // ---------------------
    Log.trace(F("Trying to load channel for note playing settings"CR));
    channelNote = file.read()-48;

    if (channelNote < 1 || channelNote  > 16) {
        Log.trace(F("Somethign is wrong reading channel note - channel note should be in range 1 to 16. Exiting loading snapshot."CR));
        file.close();
        return false;
    }
    //go to next line by reading carriage return
    if (file.read()!=10) {
        Log.trace(F("Somethign is wrong reading channel note - no carriage return at the end. Exiting loading snapshot."CR));
        file.close();
        return false;
    }
    Log.trace(F("Finished loading channel note settings. Channel note is %d"CR), channelNote);

    // ---------------------
    // load double note settings  - doubleNote is saves as 0 or 1
    // ---------------------
    Log.trace(F("Trying to load double note settings"CR));
    int intDoubleNote = 10;
    intDoubleNote = file.read()-48;

    if (intDoubleNote !=0 && intDoubleNote !=1) {
        Log.trace(F("Something is wrong reading double note - double note should be 0 or 1. Exiting loading snapshot."CR));
        file.close();
        return false;
    }

    if(intDoubleNote == 1) { // double note is on - load it as true
      doubleNote=true;
    }
    if(intNoteOff == 0) { // double note is off - load it as false
      doubleNote=false;
    }
    //go to next line by reading carriage return
    if (file.read()!=10) {
        Log.trace(F("Somethign is wrong reading double note - no carriage retrun at the end. Exiting loading snapshot."CR));
        file.close();
        return false;
    }
    Log.trace(F("Finished loading double note settings. Double Note is %T"CR), doubleNote);

    // ---------------------
    // load channel chord  - channel chord range is from 1 to 16 - it is written as a single custom char 
    // ---------------------
    Log.trace(F("Trying to load channel for chord playing settings"CR));
    channelChord = file.read()-48;

    if (channelChord < 1 || channelChord  > 16) {
        Log.trace(F("Somethign is wrong reading channel chord - channel chord should be in range 1 to 16. Exiting loading snapshot."CR));
        file.close();
        return false;
    }
    //go to next line by reading carriage return
    if (file.read()!=10) {
        Log.trace(F("Something is wrong reading channel note - no carriage return at the end. Exiting loading snapshot."CR));
        file.close();
        return false;
    }
    Log.trace(F("Finished loading channel chord settings. Channel Chord is %d"CR), channelChord);

    // ---------------------
    // load chord set overall settings  - chordSetSettings[3][3] is a 3x3 matrix where each row is for specific chord set and defines  
    // ---------------------
    Log.trace(F("Trying to load chord set overall settings"CR));
    for(int i=0; i<3; i++) {
      for(int j=0; j<3; j++) {
        chordSetSettings[i][j] = file.read()-48;
      }
      if(file.read() != 10) { 
          Log.trace(F("Load chord set overall settings - carrriage return is not in the right place"CR));
          file.close();
          return false;
      }
    }
    Log.trace(F("Finished loading chord set overall settings"CR));

    // ---------------------
    // load what chord set (chordSet) is selected   - there are 1 - 3 chord sets 
    // ---------------------
    Log.trace(F("Trying to load what chord set is selected "CR));
    chordSet = file.read()-48;

    if (chordSet < 1 || chordSet  > 3) {
        Log.trace(F("Somethign is wrong reading selcted chord set - Selected chord set should be in range 1 to 3. Exiting loading snapshot."CR));
        file.close();
        return false;
    }
    //go to next line by reading carriage return
    if (file.read()!=10) {
        Log.trace(F("Somethign is wrong reading chord set slection  - no carriage return at the end. Exiting loading snapshot."CR));
        file.close();
        return false;
    }
    Log.trace(F("Finished loading selected chord set. Selecetd Chord set is %d"CR), chordSet);

    // ---------------------
    // load chord set detailed settings  for each chord set
    //  - chordsSetOne[13][22] is a 13x22 matrix where each row is for specific pedal index
    //  - chordsSetTwo[13][22] is a 13x22 matrix where each row is for specific pedal index
    //  - chordsSetThree[13][22] is a 13x22 matrix where each row is for specific pedal index
    // ----------------------------------------------------------------------------------------------------------------------------------
    // | total notes playing | note1 octave | note1 index | note2 octave | note2 index | ... | note10 octave | note 10 index | chordType
    // ----------------------------------------------------------------------------------------------------------------------------------
    Log.trace(F("Trying to load chord set one detailed settings"CR));
    for(int i=0; i<13; i++) {
      for(int j=0; j<22; j++) {
        chordsSetOne[i][j] = file.read()-48;
      }
      if(file.read() != 10) { 
          Log.trace(F("Failed to Load chord set one detailed settings - carrriage return is not in the right place"CR));
          file.close();
          return false;
      }
    }
    Log.trace(F("Finished loading chord set one detailed settings"CR));
    Log.trace(F("Trying to load chord set two detail  settings"CR));
    for(int i=0; i<13; i++) {
      for(int j=0; j<22; j++) {
        chordsSetTwo[i][j] = file.read()-48;
      }
      if(file.read() != 10) { 
          Log.trace(F("Failed to Load chord set two detailed  settings - carrriage return is not in the right place"CR));
          file.close();
          return false;
      }
    }
    Log.trace(F("Finished loading chord set two detailed settings"CR));
    Log.trace(F("Trying to load chord set three detail  settings"CR));
    for(int i=0; i<13; i++) {
      for(int j=0; j<22; j++) {
        chordsSetThree[i][j] = file.read()-48;
      }
      if(file.read() != 10) { 
          Log.trace(F("Failed to Load chord set three detailed settings - carrriage return is not in the right place"CR));
          file.close();
          return false;
      }
    }
    Log.trace(F("Finished loading chord set three detailed settings"CR));

    // set snapshot name
    snapshot_name = path;

    file.close();

    Log.trace(F("Finished loading settings from snapshot: %s"CR), path);
    Log.trace(F("Octave is: %d"CR), getOctave());
		Log.trace(F("Velovity is: %d"CR), getVelocity());
		Log.trace(F("Channel Note is: %d"CR), getChannelNote());
		Log.trace(F("Channel Chord is: %d"CR), getChannelChord());
		Log.trace(F("Note Off is: %T"CR), getNoteOff());
		Log.trace(F("Double Note is: %T"CR), getDoubleNote());
		Log.trace(F("State is: %d"CR), getState());
		Log.trace(F("Chord Set selected is: %d"CR), getChordSet());

		Log.trace(F("Chord Set One Root Note is: %d"CR), getChordsSetRootNote(1));
		Log.trace(F("Chord Set One Mode is: %d"CR), getChordsSetMode(1));
		Log.trace(F("Chord Set One Octave: %d"CR), getChordsSetOctave(1));

		Log.trace(F("Chord Set Two Root Note is: %d"CR), getChordsSetRootNote(2));
		Log.trace(F("Chord Set Two Mode is: %d"CR), getChordsSetMode(2));
		Log.trace(F("Chord Set Two Octave: %d"CR), getChordsSetOctave(2));

		Log.trace(F("Chord Set Three Root Note is: %d"CR), getChordsSetRootNote(3));
		Log.trace(F("Chord Set Three Mode is: %d"CR), getChordsSetMode(3));
		Log.trace(F("Chord Set Three Octave: %d"CR), getChordsSetOctave(3));

    Log.trace(F("ChordSet One array is set to following: "CR));
    for (int i = 0; i < 13; i++)
    { 
      Log.trace(F("{ "));
      for (int j = 0; j < 22; j++)
      {
          Log.trace(F("%d "), chordsSetOne[i][j]);
      }
      Log.trace(F(" }"CR));
    }

    for (int i = 0; i < 13; i++)
    { 
      Log.trace(F("{ "));
      for (int j = 0; j < 22; j++)
      {
          Log.trace(F("%d "), chordsSetTwo[i][j]);
      }
      Log.trace(F(" }"CR));
    }

    for (int i = 0; i < 13; i++)
    { 
      Log.trace(F("{ "));
      for (int j = 0; j < 22; j++)
      {
          Log.trace(F("%d "), chordsSetThree[i][j]);
      }
      Log.trace(F(" }"CR));
    }

    Log.trace(F("Settings.init() exit."CR));
    return true;

}

bool Settings::saveSnapshot(const char * path) {
	Log.trace(F("Settings.saveSnapshots() function call."CR));
	Log.trace(F("Settings.saveSnapshots() function exit."CR));
  return saveSnapshotInternal(SPIFFS, path);
}

bool Settings::saveSnapshotInternal(fs::FS &fs, const char * path) {
    Log.trace(F("Writing file: %s"CR), path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Log.trace(F("- failed to open file for writing"CR));
        return false;
    }

    Log.trace(F("Trying to save current Settings to snapshot file : %s"CR), path);
    
    // ---------------------
    // writte velocity - velocity is intiger from 0 to 127 - writte it as 1 to 3  characters that represent each digit
    // ---------------------
    Log.trace(F("Trying to writte velocity: %d"CR), velocity);
    if (velocity < 10) { //velocity has only one digit
      if(file.print((char)(velocity+48))){
           Log.trace(F("First velocity digit written: %d"CR), velocity+48);
      } else {
          Log.trace(F("- write failed"CR));
          file.close();
          return false;
      }
    } else {
      if (velocity < 100) { //velocity has two digits
        if(file.print((char)((velocity-velocity%10)/10+48))) { // writte first digit
            Log.trace(F("First velocity digit written: %d"CR), (velocity-velocity%10)/10+48);
        } else {
            Log.trace(F("- write failed"CR));
            file.close();
            return false;
        }
        if(file.print((char)(velocity%10+48))) { // writte second digit
            Log.trace(F("Second velocity digit written: %d"CR), velocity%10+48);
        } else {
            Log.trace(F("- write failed"CR));
            file.close();
            return false;
        }
      } else { // velocity has 3 digits
        if(file.print((char)(1+48))) { // writte first digit
            Log.trace(F("First velocity digit written: %d"CR), 1+48);
        } else {
            Log.trace(F("- write failed"CR));
            file.close();
            return false;
        }
        if(file.print((char)(((velocity%100)-(velocity%100)%10)/10+48))) { // writte second digit
            Log.trace(F("Second velocity digit written: %d"CR), ((velocity%100)-(velocity%100)%10)/10+48);
        } else {
            Log.trace(F("- write failed"CR));
            file.close();
            return false;
        }
        if(file.print((char)(velocity%10+48))) { // writte third digit
            Log.trace(F("Third velocity digit written: %d"CR), velocity%10+48);
        } else {
            Log.trace(F("- write failed"CR));
            file.close();
            return false;
        }
      }
    }
    //finish writting velocity by entering new line
    if(file.print("\n")) { 
        Log.trace(F("- file written"CR));
    } else {
        Log.trace(F("- write failed"CR));
        file.close();
        return false;
    }
    Log.trace(F("Finished writting velocity"CR));

    // ---------------------
    // writte state  - save only playable states - not configuration states
    // ---------------------
    Log.trace(F("Trying to writte state: %d"CR), state);
    if(state<10) { // state is playabel state and has only one digit
        if(file.print((char)(state+48))) { 
            Log.trace(F("First digit written: %d"CR), state+48);
        } else {
            Log.trace(F("- write failed"CR));
            file.close();
            return false;
        }
    } else { // we are in configuration state so save playabel state only
        if( file.print((char)((state-state%10)/10+48)) ) { 
            Log.trace(F("First digit written: %d"CR), (state-state%10)/10+48);
        } else {
            Log.trace(F("- write failed"CR));
            file.close();
            return false;
        }
    }
    //finish writting state by entering new line
    if(file.print("\n")) { 
        Log.trace(F("- file written"CR));
    } else {
        Log.trace(F("- write failed"CR));
        file.close();
        return false;
    }
    Log.trace(F("Finished writting state"CR));

    // ---------------------
    // writte note off settings  - save it as 0 or 1
    // ---------------------
    Log.trace(F("Trying to writte note off settings"CR));
    if(noteOff) { // noteoff is on - save ot as 1
        if(file.print((char)(1+48))) { 
            Log.trace(F("First digit written: %d"CR), 1+48);
        } else {
            Log.trace(F("- write failed"CR));
            file.close();
            return false;
        }
    } else { // noteoff is off - save it as 0
        if(file.print((char)(0+48))) { 
            Log.trace(F("First digit written: %d"CR), 0+48);
        } else {
            Log.trace(F("- write failed"CR));
            file.close();
            return false;
        }
    }
    //finish writting noteOff by entering new line
    if(file.print("\n")) { 
        Log.trace(F("- file written"CR));
    } else {
        Log.trace(F("- write failed"CR));
        file.close();
        return false;
    }
    Log.trace(F("Finished writting note off settings"CR));

    // ---------------------
    // writte octave  - octave range is from -4 to 5 - writte it as a signle custom char where 
    // ---------------------
    Log.trace(F("Trying to writte octave settings"CR));
      if(file.print((char)(octave+48))) { 
          Log.trace(F("First digit written: %d"CR), octave+48);
      } else {
          Log.trace(F("- write failed"CR));
          file.close();
          return false;
      }
    //finish writting octave by entering new line
    if(file.print("\n")) { 
        Log.trace(F("- file written"CR));
    } else {
        Log.trace(F("- write failed"CR));
        file.close();
        return false;
    }
    Log.trace(F("Finished writting octave settings"CR));

    // ---------------------
    // writte channel note  - channel note range is from 1 to 16 - writte it as a signle custom char where 
    // ---------------------
    Log.trace(F("Trying to writte channel note settings"CR));
      if(file.print((char)(channelNote+48))) { 
          Log.trace(F("First digit written: %d"CR), channelNote+48);
      } else {
          Log.trace(F("- write failed"CR));
          file.close();
          return false;
      }
    //finish writting channel note by entering new line
    if(file.print("\n")) { 
        Log.trace(F("- file written"CR));
    } else {
        Log.trace(F("- write failed"CR));
        file.close();
        return false;
    }
    Log.trace(F("Finished writting channel note settings"CR));

    // ---------------------
    // writte double note settings  - save it as 0 or 1
    // ---------------------
    Log.trace(F("Trying to writte double note settings"CR));
    if(doubleNote) { // doubleNote is on - save ot as 1
        if(file.print((char)(1+48))) { 
            Log.trace(F("First digit written: %d"CR), 1+48);
        } else {
            Log.trace(F("- write failed"CR));
            file.close();
            return false;
        }
    } else { // doubleNote is off - save ot as 0
        if(file.print((char)(0+48))) { 
            Log.trace(F("First digit written: %d"CR), 0+48);
        } else {
            Log.trace(F("- write failed"CR));
            file.close();
            return false;
        }
    }
    //finish writting double note by entering new line
    if(file.print("\n")) { 
        Log.trace(F("- file written"CR));
    } else {
        Log.trace(F("- write failed"CR));
        file.close();
        return false;
    }
    Log.trace(F("Finished writting doubleNote settings"CR));

    // ---------------------
    // writte channel chord  - channel chord range is from 1 to 16 - writte it as a single custom char 
    // ---------------------
    Log.trace(F("Trying to writte channel note settings"CR));
    if(file.print((char)(channelChord+48))) { 
        Log.trace(F("First digit written: %d"CR), channelChord+48);
    } else {
        Log.trace(F("- write failed"CR));
        file.close();
        return false;
    }
    //finish writting channel chord by entering new line
    if(file.print("\n")) { 
        Log.trace(F("- file written"CR));
    } else {
        Log.trace(F("- write failed"CR));
        file.close();
        return false;
    }
    Log.trace(F("Finished writting channel chord settings"CR));

    // ---------------------
    // writte chord set overall settings  - chordSetSettings[3][3] is a 3x3 matrix where each row is for specifci chord set and defines  
    //  - rootNote
    //  - mode ( Ionian, Dorian, Phrygian, Lydian, Mixolydian, Aeolian, Locrian )
    //  - Octave
    // ---------------------
    Log.trace(F("Trying to writte chord set overall settings"CR));
    for(int i=0; i<3; i++) {
      for(int j=0; j<3; j++) {
        if(file.print((char)(chordSetSettings[i][j]+48))) { 
            Log.trace(F("First digit written: %d"CR), chordSetSettings[i][j]+48);
        } else {
            Log.trace(F("- write failed"CR));
            file.close();
            return false;
        }
      }
      if(file.print("\n")) { 
          Log.trace(F("- file written"CR));
      } else {
          Log.trace(F("- write failed"CR));
          file.close();
          return false;
      }
    }
    Log.trace(F("Finished writting chord set overall settings"CR));

    // ---------------------
    // writte what chord set is selected   - there are 1 - 3 chord sets 
    // ---------------------
    Log.trace(F("Trying to writte selected chord set"CR));
      if(file.print((char)(chordSet+48))) { 
          Log.trace(F("First digit written: %d"CR), chordSet+48);
      } else {
          Log.trace(F("- write failed"CR));
          file.close();
          return false;
      }
    //finish writting selected chord set  by entering new line
    if(file.print("\n")) { 
        Log.trace(F("- file written"CR));
    } else {
        Log.trace(F("- write failed"CR));
        file.close();
        return false;
    }
    Log.trace(F("Finished writting selected chord set "CR));

    // ---------------------
    // writte chord set detailed settings  
    //  - chordsSetOne[13][22] is a 13x22 matrix where each row is for specific pedal index
    //  - chordsSetTwo[13][22] is a 13x22 matrix where each row is for specific pedal index
    //  - chordsSetThree[13][22] is a 13x22 matrix where each row is for specific pedal index
    // ----------------------------------------------------------------------------------------------------------------------------------
    // | total notes playing | note1 octave | note1 index | note2 octave | note2 index | ... | note10 octave | note 10 index | chordType
    // ----------------------------------------------------------------------------------------------------------------------------------
    Log.trace(F("Trying to writte chord set one details settings"CR));
    for(int i=0; i<13; i++) {
      for(int j=0; j<22; j++) {
        if(file.print((char)(chordsSetOne[i][j]+48))) { 
            Log.trace(F("First digit written: %d"CR), chordsSetOne[i][j]+48);
        } else {
            Log.trace(F("- write failed"CR));
            file.close();
            return false;
        }
      }
      if(file.print("\n")) { 
          Log.trace(F("- file written"CR));
      } else {
          Log.trace(F("- write failed"CR));
          file.close();
          return false;
      }
    }
    Log.trace(F("Finished writting chord set one detailed settings"CR));
    Log.trace(F("Trying to writte chord set two details settings"CR));
    for(int i=0; i<13; i++) {
      for(int j=0; j<22; j++) {
        if(file.print((char)(chordsSetTwo[i][j]+48))) { 
            Log.trace(F("First digit written: %d"CR), chordsSetTwo[i][j]+48);
        } else {
            Log.trace(F("- write failed"CR));
            file.close();
            return false;
        }
      }
      if(file.print("\n")) { 
          Log.trace(F("- file written"CR));
      } else {
          Log.trace(F("- write failed"CR));
          file.close();
          return false;
      }
    }
    Log.trace(F("Finished writting chord set two detailed settings"CR));
    Log.trace(F("Trying to writte chord set three details settings"CR));
    for(int i=0; i<13; i++) {
      for(int j=0; j<22; j++) {
        if(file.print((char)(chordsSetThree[i][j]+48))) { 
            Log.trace(F("First digit written: %d"CR), chordsSetThree[i][j]+48);
        } else {
            Log.trace(F("- write failed"CR));
            file.close();
            return false;
        }
      }
      if(file.print("\n")) { 
          Log.trace(F("- file written"CR));
      } else {
          Log.trace(F("- write failed"CR));
          file.close();
          return false;
      }
    }
    Log.trace(F("Finished writting chord set three detailed settings"CR));

    file.close();

    return true;
}

bool Settings::saveEmptySnapShotInternal(fs::FS &fs, int bank, int patch) {
	Log.trace(F("Settings.saveEmptySnapshotInternal() function call."CR));

    // delete file if exist
    deleteSnapshotInternal(fs, patchNames[bank][patch]);

    //writte the file
    File file = fs.open(patchNames[bank][patch], FILE_WRITE);

    if(!file){
        Log.trace(F("- failed to open file for writing"CR));
        return false;
    }

    if(file.print(" ")){
        Log.trace(F("- file written"CR));
    } else {
        Log.trace(F("- write failed"CR));
    }

    file.close();
	Log.trace(F("Settings.saveEmptySnapshotInternal() function exit."CR));
  return true;
}

bool Settings::renameSnapShotInternal(fs::FS &fs, const char * path1, const char * path2){
    Log.trace(F("Renaming file %s to %s"CR), path1, path2);
    if (fs.rename(path1, path2)) {
        Log.trace(F("- file renamed"CR));
        return true;
    } else {
        Log.trace(F("- rename failed"CR));
        return false;
    }
}

void Settings::deleteSnapshotInternal(fs::FS &fs, const char * path){
    Log.trace(F("Deleting file: %s"CR), path);
    if(fs.remove(path)){
        Log.trace(F("- file deleted"CR));
    } else {
        Log.trace(F("- delete failed"CR));
    }
}