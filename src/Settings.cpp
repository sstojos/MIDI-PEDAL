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

String snapshot_name;

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

void Settings::init()
{
	Log.trace(F("Settings.init() function call. Settings instance id: %d"CR), this);

    // init Settings data - configure default settings for the whole device
    velocity=127;
    state=1;
    noteOff=true;
    snapshot_name = "Default";

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

     
    Log.trace(F("Settings.init() function exit. Settings instance id: %d"CR), this);

}

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
 * get Double Note
 */
bool Settings::getDoubleNote()
{
	Log.trace(F("Settings.getDoubleNote() function call. Settings instance id: %d"CR), this);
	return doubleNote;
	Log.trace(F("Settings.getDoubleNote() function exit. Settings instance id: %d"CR), this);
}

/*
 * get Channel Note
 */
int Settings::getChannelNote()
{
	Log.trace(F("Settings.getChannelNote() function call. Settings instance id: %d"CR), this);
	return channelNote;
	Log.trace(F("Settings.getChannelNote() function exit. Settings instance id: %d"CR), this);
}

/*
 * get Channel Chord
 */
int Settings::getChannelChord()
{
	Log.trace(F("Settings.getChannelChord() function call. Settings instance id: %d"CR), this);
	return channelChord;
	Log.trace(F("Settings.getChannelChord() function exit. Settings instance id: %d"CR), this);
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

