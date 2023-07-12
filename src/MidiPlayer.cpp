#ifndef MIDIPLAYER_H_
#include "MidiPlayer.h"
#endif


void MidiPlayer::init() {
  Log.trace(F("MidiPlayer.init() function call."CR));

  // init note full name array
  noteFullName[0] = note0;
  noteFullName[1] = note1;
  noteFullName[2] = note2;
  noteFullName[3] = note3;
  noteFullName[4] = note4;
  noteFullName[5] = note5;
  noteFullName[6] = note6;
  noteFullName[7] = note7;
  noteFullName[8] = note8;
  noteFullName[9] = note9;
  noteFullName[10] = note10;
  noteFullName[11] = note11;
  noteFullName[12] = note12;


  //midiSerial = new SoftwareSerial(3, 15); // RX, TX
  midiSerial = new SoftwareSerial(18, 17); // (RX, TX) RX is not connected to MIDI, TX is connected only

  midiSerial->begin(31250);
  
  //Initialize notesPlying with no notes playing
  notesPlaying[0] = 0;

  // set last playing chord pedal index to 13 which is not possible
  lastPlayingChordPedalIndex = 13;

  //Initialize with ionin mode in C (1 is C pedal, and 1 is first ionian mode)
  setModeChords(10,1);

  Log.trace(F("MidiPlayer.init() function exit."CR));

}

void MidiPlayer::setModeChords(int rootNote, int mode) {
  Log.trace(F("generating the chords for root note: %d and mode: %d"CR), rootNote, mode);

  //  clear all notes in chords
  for(int i=0; i < 3; i++) {
    modeChord1[i] = 0;
    modeChord2[i] = 0;
    modeChord3[i] = 0;
    modeChord4[i] = 0;
    modeChord5[i] = 0;
    modeChord6[i] = 0;
    modeChord7[i] = 0;
  }

  //clear the 13 pedal triggers 
	for(int i=0; i < 13; i++) { chordPedalTrigger[i] = 0; }

  int offset = mode-1;

  modeChord1[0] = rootNote;
  modeChord1[1] = modeChord1[0] + ionianScale[0 + offset] + ionianScale[1 + offset];
  modeChord1[2] = modeChord1[1] + ionianScale[2 + offset] + ionianScale[3 + offset];
  chordPedalTrigger[rootNote] = 1;
  if (rootNote == 0) { chordPedalTrigger[12] = 1; }

	//int ionianScale[16] =     {2,2,1,2,2,2,1,2,2,1,2,2,2,1,2,2};
  //                         C,D,E,F,G,A,B,C,D,E,F,G,A,B,C
  //              10      +2
  modeChord2[0] = (rootNote+ionianScale[0 + offset])%12;
  modeChord2[1] = modeChord2[0] + ionianScale[1 + offset] + ionianScale[2 + offset];
  modeChord2[2] = modeChord2[1] + ionianScale[3 + offset] + ionianScale[4 + offset];
  chordPedalTrigger[(rootNote+ionianScale[0 + offset])%12]=2;
  if ((rootNote+ionianScale[0 + offset])%12 == 0) { chordPedalTrigger[12] = 2; }

	//int ionianScale[16] =     {2,2,1,2,2,2,1,2,2,1,2,2,2,1,2,2};
  //                         C,D,E,F,G,A,B,C,D,E,F,G,A,B,C
  //              10      +2                      +2                      =14
  modeChord3[0] = (rootNote+ionianScale[0 + offset]+ionianScale[1 + offset])%12;
  modeChord3[1] = modeChord3[0] + ionianScale[2 + offset] + ionianScale[3 + offset];
  modeChord3[2] = modeChord3[1] + ionianScale[4 + offset] + ionianScale[5 + offset];
  chordPedalTrigger[(rootNote+ionianScale[0 + offset]+ionianScale[1 + offset])%12]=3;
  if ((rootNote+ionianScale[0 + offset]+ionianScale[1 + offset])%12 == 0) { chordPedalTrigger[12] = 3; }

	//int ionianScale[16] =     {2,2,1,2,2,2,1,2,2,1,2,2,2,1,2,2};
  //                         C,D,E,F,G,A,B,C,D,E,F,G,A,B,C
  //              10      +2                      +2                      +1                      =15
  modeChord4[0] = (rootNote+ionianScale[0 + offset]+ionianScale[1 + offset]+ionianScale[2 + offset])%12;
  modeChord4[1] = modeChord4[0] + ionianScale[3 + offset] + ionianScale[4 + offset];
  modeChord4[2] = modeChord4[1] + ionianScale[5 + offset] + ionianScale[6 + offset];
  chordPedalTrigger[(rootNote+ionianScale[0 + offset]+ionianScale[1 + offset]+ionianScale[2 + offset])%12]=4;
  if ((rootNote+ionianScale[0 + offset]+ionianScale[1 + offset]+ionianScale[2 + offset])%12 == 0) { chordPedalTrigger[12] = 4; }

	//int ionianScale[16] =     {2,2,1,2,2,2,1,2,2,1,2,2,2,1,2,2};
  //                         C,D,E,F,G,A,B,C,D,E,F,G,A,B,C
  //              10      +2                      +2                      +1                      +2                      =17
  modeChord5[0] = (rootNote+ionianScale[0 + offset]+ionianScale[1 + offset]+ionianScale[2 + offset]+ionianScale[3 + offset])%12;
  modeChord5[1] = modeChord5[0] + ionianScale[4 + offset] + ionianScale[5 + offset];
  modeChord5[2] = modeChord5[1] + ionianScale[6 + offset] + ionianScale[7 + offset];
  chordPedalTrigger[(rootNote+ionianScale[0 + offset]+ionianScale[1 + offset]+ionianScale[2 + offset]+ionianScale[3 + offset])%12]=5;
  if ((rootNote+ionianScale[0 + offset]+ionianScale[1 + offset]+ionianScale[2 + offset]+ionianScale[3 + offset])%12 == 0) { chordPedalTrigger[12] = 5; }

	//int ionianScale[16] =     {2,2,1,2,2,2,1,2,2,1,2,2,2,1,2,2};
  //                         C,D,E,F,G,A,B,C,D,E,F,G,A,B,C
  //              10      +2                      +2                      +1                      +2                      +2                       =19
  modeChord6[0] = (rootNote+ionianScale[0 + offset]+ionianScale[1 + offset]+ionianScale[2 + offset]+ionianScale[3 + offset]+ionianScale[4 + offset])%12;
  modeChord6[1] = modeChord6[0] + ionianScale[5 + offset] + ionianScale[6 + offset];
  modeChord6[2] = modeChord6[1] + ionianScale[7 + offset] + ionianScale[8 + offset];
  chordPedalTrigger[(rootNote+ionianScale[0 + offset]+ionianScale[1 + offset]+ionianScale[2 + offset]+ionianScale[3 + offset]+ionianScale[4 + offset])%12]=6;
  if ((rootNote+ionianScale[0 + offset]+ionianScale[1 + offset]+ionianScale[2 + offset]+ionianScale[3 + offset]+ionianScale[4 + offset])%12 == 0) { chordPedalTrigger[12] = 6; }

	//int ionianScale[16] =     {2,2,1,2,2,2,1,2,2,1,2,2,2,1,2,2};
  //                         C,D,E,F,G,A,B,C,D,E,F,G,A,B,C
  //              10      +2                      +2                      +1                      +2                      +2                      +2
  modeChord7[0] = (rootNote+ionianScale[0 + offset]+ionianScale[1 + offset]+ionianScale[2 + offset]+ionianScale[3 + offset]+ionianScale[4 + offset]+ionianScale[5 + offset])%12;
  modeChord7[1] = modeChord7[0] + ionianScale[6 + offset] + ionianScale[7 + offset];
  modeChord7[2] = modeChord7[1] + ionianScale[8 + offset] + ionianScale[9 + offset];
  chordPedalTrigger[(rootNote+ionianScale[0 + offset]+ionianScale[1 + offset]+ionianScale[2 + offset]+ionianScale[3 + offset]+ionianScale[4 + offset]+ionianScale[5 + offset])%12]=7;
  if ((rootNote+ionianScale[0 + offset]+ionianScale[1 + offset]+ionianScale[2 + offset]+ionianScale[3 + offset]+ionianScale[4 + offset]+ionianScale[5 + offset])%12 == 0) { chordPedalTrigger[12] = 7; }

}

boolean MidiPlayer::isNotePlaying() {
   Log.trace(F("MidiPlayer.isNotePlaying() function called."CR));
   Log.trace(F("\tchecking if notesPlaying[0] is 0."CR));
  if (notesPlaying[0] == 0) {
    return false;
  } else {
    return true;
  }
  Log.trace(F("MidiPlayer.isNotePlaying() function exit."CR));
}

int MidiPlayer::getLastNote() {
  Log.trace(F("MidiPlayer.getLastNote() function called."CR));
  return notesPlaying[notesPlaying[0]*2+1];
  Log.trace(F("MidiPlayer.getLastNote() function exit."CR));
}

int MidiPlayer::getLastChordPedalIndex() {
  Log.trace(F("MidiPlayer.getLastChord() function called."CR));
  return lastPlayingChordPedalIndex;
  Log.trace(F("MidiPlayer.getLastChord() function exit."CR));
}

//return the root note of the chord
int MidiPlayer::getChordRootNote() {
  Log.trace(F("MidiPlayer.getChordRootNote(() function called."CR));

  switch (chordPedalTrigger[lastPlayingChordPedalIndex]) {
    case 1:
      return modeChord1[0];
      break;
    case 2:
      return modeChord2[0];
      break;
    case 3:
      return modeChord3[0];
      break;
    case 4:
      return modeChord4[0];
      break;
    case 5:
      return modeChord5[0];
      break;
    case 6:
      return modeChord6[0];
      break;
    case 7:
      return modeChord7[0];
      break;
    default:
      return 0;
  }

  Log.trace(F("MidiPlayer.getChordRootNote(() function exit."CR));
}

//return the chord type (0 major, 1 minor, 2 diminished)
int MidiPlayer::getChordType() {
  Log.trace(F("MidiPlayer.getChordType() function called."CR));

  switch (chordPedalTrigger[lastPlayingChordPedalIndex]) {
    case 1:
      if (modeChord1[1]-modeChord1[0] == 4) {
          return 0;
      } else  if (modeChord1[2]-modeChord1[1] == 4) {
          return 1;
      } else {
          return 2;        
      }
      break;
    case 2:
      if (modeChord2[1]-modeChord2[0] == 4) {
          return 0;
      } else  if (modeChord2[2]-modeChord2[1] == 4) {
          return 1;
      } else {
          return 2;        
      }
      break;
    case 3:
      if (modeChord3[1]-modeChord3[0] == 4) {
          return 0;
      } else  if (modeChord3[2]-modeChord3[1] == 4) {
          return 1;
      } else {
          return 2;        
      }
      break;
    case 4:
      if (modeChord4[1]-modeChord4[0] == 4) {
          return 0;
      } else  if (modeChord4[2]-modeChord4[1] == 4) {
          return 1;
      } else {
          return 2;        
      }
      break;
    case 5:
      if (modeChord5[1]-modeChord5[0] == 4) {
          return 0;
      } else  if (modeChord5[2]-modeChord5[1] == 4) {
          return 1;
      } else {
          return 2;        
      }
      break;
    case 6:
      if (modeChord6[1]-modeChord6[0] == 4) {
          return 0;
      } else  if (modeChord6[2]-modeChord6[1] == 4) {
          return 1;
      } else {
          return 2;        
      }
      break;
    case 7:
      if (modeChord7[1]-modeChord7[0] == 4) {
          return 0;
      } else  if (modeChord7[2]-modeChord7[1] == 4) {
          return 1;
      } else {
          return 2;        
      }
      break;
    default:
      return 0;
  }
  
  Log.trace(F("MidiPlayer.getChordType() function exit."CR));
}

// plays a MIDI note ON based on
// note index, octave, channel and velocity 
void MidiPlayer::playNoteOn(int index, int octave, int channel, int velocity) {
  Log.trace(F("MidiPlayer.playNotedOn(index,octave,channel,velocity) function called. Sending midi note on for pedal index: %d, for ocatve: %d, on channel: %d, with velocity: %d"CR), index, octave, channel, velocity);
  midiSerial->write(noteOnCommand[mp_settings->getChannel()-1]);  //noteOn command is 144 for channel 1
  midiSerial->write(getMidiNote(index, octave)); //pitch
  midiSerial->write(velocity);
  Log.trace(F("MidiPlayer.playNoteOn(index,octave,channel,velocity) function exit."CR));
}

// called by pedalOn function when pedal is in note mode
void MidiPlayer::pedalOnNoteMode(int index) {
  
  Log.trace(F("MidiPlayer.pedalOnNoteMode() function called."CR));

  // check if Note Off is disabled in setings 
  // and that something is playing 
  // and that this is the same note that is already playing
  if (mp_settings->getNoteOff() == false && isNotePlaying() && getLastNote() == index) {

    //this is the same note that is already playing so this is command to stop the note 
    stopNotes();
    
  } else {
      // this is the command to play note

      // if note off settings is disabled stop anything playing before
      if (mp_settings->getNoteOff() == false) {
        stopNotes();
      }

      // check if there is condition to play two notes which is: 
      // - Two note is enabled in settings
      // - and Note Off is disabled in settings 
      // - and this is not in chordPlaying mode
      if (mp_settings->getDoubleNote() == true && mp_settings->getNoteOff() == false && !mp_settings->getChordPlaying()) {
              //play requested note one octave higher
              playNoteOn(index, mp_settings->getOctave()+1, mp_settings->getChannel(), mp_settings->getVelocity());
              //add note played to cache
              //we can turn it off in a case of swithing octaves or channels
              //while playing - otherwise note will be played contstantly
              addNotePlayingToCache(mp_settings->getOctave()+1, index);
      }

      //play note requested last
      playNoteOn(index, mp_settings->getOctave(), mp_settings->getChannel(), mp_settings->getVelocity());
      //add note played to cache
      //we can turn it off in a case of swithing octaves or channels
      //while playing - otherwise note will be played contstantly
      addNotePlayingToCache(mp_settings->getOctave(), index);

  }
  Log.trace(F("\tChecking if there are anything to print on screen."CR));
  if(isNotePlaying()) {
    //there are more notes playing - show last one on screen
    Log.trace(F("\tThere are more notes to play. Rendering the screen with last playign note in queue ..."CR));
    //TODO: screenPointer->renderNote(getLastNote());

    renderNote(noteFullName[getLastNote()]);

  } else {
    //there are no more notes playing - clear the screen
    //TODO: screenPointer->clearNote();
    renderNote("");
    
  }

  Log.trace(F("MidiPlayer.pedalOnNoteMode( function exit."CR));
}

// called by pedalOn function when pedal is in chord mode
void MidiPlayer::pedalOnChordMode(int index) {
  
  Log.trace(F("MidiPlayer.pedalOnChordMode() function called."CR));

  // check that something is playing 
  // and that this is the same chord that is already playing whcih means user want to stop playing it

  //always stopNotes first for chords - do not allow simultaniun pedal presses
  stopNotes();

  // check that pedal is not already playing this pedal index - if yes do nto play it again - it will be stopped by previous command 
  // and
  // check that there is anything to play  (chord assigned to that pedal index)
  if (lastPlayingChordPedalIndex != index && chordPedalTrigger[index] != 0) {
    int note1Index, note2Index, note3Index;
    switch (chordPedalTrigger[index]) {
      case 1:
        note1Index = modeChord1[0];
        note2Index = modeChord1[1];
        note3Index = modeChord1[2];
        break;
      case 2:
        note1Index = modeChord2[0];
        note2Index = modeChord2[1];
        note3Index = modeChord2[2];
        break;
      case 3:
        note1Index = modeChord3[0];
        note2Index = modeChord3[1];
        note3Index = modeChord3[2];
        break;
      case 4:
        note1Index = modeChord4[0];
        note2Index = modeChord4[1];
        note3Index = modeChord4[2];
        break;
      case 5:
        note1Index = modeChord5[0];
        note2Index = modeChord5[1];
        note3Index = modeChord5[2];
        break;
      case 6:
        note1Index = modeChord6[0];
        note2Index = modeChord6[1];
        note3Index = modeChord6[2];
        break;
      case 7:
        note1Index = modeChord7[0];
        note2Index = modeChord7[1];
        note3Index = modeChord7[2];
        break;
    }

    //play note1
    playNoteOn(note1Index, mp_settings->getOctave(), mp_settings->getChannel(), mp_settings->getVelocity());
    //add note1 played to cache
    addNotePlayingToCache(mp_settings->getOctave(), note1Index);
    //play note2
    playNoteOn(note2Index, mp_settings->getOctave(), mp_settings->getChannel(), mp_settings->getVelocity());
    //add note1 played to cache
    addNotePlayingToCache(mp_settings->getOctave(), note2Index);
    //play note3
    playNoteOn(note3Index, mp_settings->getOctave(), mp_settings->getChannel(), mp_settings->getVelocity());
    //add note1 played to cache
    addNotePlayingToCache(mp_settings->getOctave(), note3Index);

    //store what was last payed pedal index
      lastPlayingChordPedalIndex = index;
  } else {
    //lear last played note
    lastPlayingChordPedalIndex = 13;
  }

  Log.trace(F("\tChecking if there are anything to print on screen."CR));
  if(isNotePlaying()) {
    //pedal on triggered to play so render the chord
    //TODO: screenPointer->renderChord(getChordRootNote()%12, getChordType());
  } else {
    //command is used to stop playing teh chrod so clear the chord
    //TODO: screenPointer->clearChord();
  }

  Log.trace(F("MidiPlayer.pedalOnChordMode( function exit."CR));
}

void MidiPlayer::pedalOn(int index) {
  Log.trace(F("MidiPlayer.pedalOn( function entry witn index: %d"CR), index);
  if (mp_settings->getChordPlaying()) {
    pedalOnChordMode(index);
  } else {
    pedalOnNoteMode(index);
  }
  Log.trace(F("MidiPlayer.pedalOn( function exit."CR));
}

// plays a MIDI note Off based on
// note index, octave, channel and velocity 
void MidiPlayer::playNoteOff(int index, int octave, int channel) {
  Log.trace(F("MidiPlayer.playNoteOff(index, octave, channel) function called. Sending midi note off for pedal index: %d, for octave: %d, on channel: %d"CR), index, octave, channel);
  midiSerial->write(noteOffCommand[mp_settings->getChannel()-1]); //noteOff command is 128 for channel 1
  midiSerial->write(getMidiNote(index, octave)); //pitch
  midiSerial->write((byte) 0x00); //velocity (set to 0 for noteOff);
  Log.trace(F("MidiPlayer.playNoteOff(index,octave,channel) function exit."CR));
}

//called by pedal when release and when midi pedal is note playing mode
void MidiPlayer::pedalOffNoteMode(int index) {
   Log.trace(F("MidiPlayer.pedalOffNoteMode(index) function called. Sending midi note off for pedal index: %d"CR), index);

  // play note off only if it is enabled in settings
  if (mp_settings->getNoteOff() == true )  {
    Log.trace(F("\tPlaying Note Off is true. "CR));
    //ignore playing off if there is nothing to be played of
    //it could be that notes are forced to be played off by device - e.g.when switching playing modes by user while they inginted noteOn of the same note in disabkel not off mode
    if (isNotePlaying()) {
        playNoteOff(index, mp_settings->getOctave(), mp_settings->getChannel());

        //remove note played from cache
        //we can turn it off in a case of swithing octaves or channels
        //while playing - otherwise note will be played contstantly
        removeNotePlayingFromCache(index);
    }
  }

  //check if there are more notes playing
  Log.trace(F("\tChecking if there are no more notes to play."CR));
  if(isNotePlaying()) {
    //there are more notes playing - show last one on screen
    Log.trace(F("\tThere are more notes to play. Rendering the screen with last playing note in queue ..."CR));
    //screenPointer->renderNote(getLastNote());

   renderNote(noteFullName[getLastNote()]);

  } else {
    //there are not more notes playing - clear the screen
    //TODO: screenPointer->clearNote();

    renderNote("");

  }

  Log.trace(F("MidiPlayer.pedalOffNoteMode(index) function exit."CR));
}

//called by pedal when release and when midi pedal is chord  playing mode
void MidiPlayer::pedalOffChordMode(int index) {
   Log.trace(F("MidiPlayer.pedalOffChordMode(index) function called. Sending midi note off for pedal index: %d"CR), index);

  // play note off only if it is enabled in settings
  if (mp_settings->getNoteOff() == true )  {
    //it could be that notes are forced to be played off by device - e.g.when switching playing modes by user while they inginted noteOn of the same note in disable not off mode
    if (isNotePlaying()) {
        stopNotes();
        //TODO: screenPointer->clearNote();

        // lv_label_set_text(ui_NoteScrNoteLabel, " ");
        // lv_label_set_text(ui_NoteScrNoteSuffixLabel, " ");
        
    }
  }

  Log.trace(F("MidiPlayer.pedalOffChordMode(index) function exit."CR));
}

//called by pedal when releases 
void MidiPlayer::pedalOff(int index) {
  Log.trace(F("MidiPlayer.pedalOff(index) function called. Sending midi note off for pedal index: %d"CR), index);
  if (mp_settings->getChordPlaying()) {
    pedalOffChordMode(index);
  } else {
    pedalOffNoteMode(index);
  }
  Log.trace(F("MidiPlayer.pedalOff(index) function exit."CR));
}


int MidiPlayer::getMidiNote(int index, int octave) { 
  Log.trace(F("MidiPlayer.getMidiNote(index,octave) function call."CR));

  int myIndex, myOctave;
  myIndex = index;
  myOctave = octave;

  // there is a chance that index is outside 0-12 range 
  // becasue chord logic is calcualting chord notes indexes  by 
  // addition or substruction to/from root note

  //adjust ocatve for higher pedal index
  myOctave=octave + index/12;
  //make sure that octave never goes over 7
  if(myOctave > 7) {myOctave = 7;}
 
  //adjust index to 0-12 range
  myIndex = index%12;

 	switch (myOctave) {
		case -2:
		  return octaveM2[myIndex];
      break;
    case -1:
      return octaveM1[myIndex];
      break;
    case -0:
      return octave0[myIndex];
      break;
    case 1:
      return octave1[myIndex];
      break;
    case 2:
      return octave2[myIndex];
      break;
    case 3:
      return octave3[myIndex];
      break;
    case 4:
      return octave4[myIndex];
      break;
    case 5:
      return octave5[myIndex];
      break;
    case 6:
      return octave6[myIndex];
      break;
    case 7:
      return octave7[myIndex];
      break;
    default:
      return octave3[myIndex];
    break;
  }
  Log.trace(F("MidiPlayer.getMidiNote(index,octave) function exit."CR));
}


void MidiPlayer::removeNotePlayingFromCache(int index) {
  Log.trace(F("MidiPlayer.removeNotePlayingFromCache(index) function call."CR));
  //remove note and repackage array
  bool noteRemoved = false;

  if (isNotePlaying() ) {
    for (int i = 2; i <= notesPlaying[0]*2; i = i + 2) {
      if (noteRemoved) {
        Log.trace(F("\tMoving the note still playing from cache index: %d to cache index %d"CR), i, i-2);
        notesPlaying[i-2] = notesPlaying[i];
        notesPlaying[i-1] = notesPlaying[i+1];
      }
      if (index == notesPlaying[i+1]) {
        noteRemoved = true;
        Log.trace(F("\tFound a note inside cache index: %d"CR), i);
      }
    }
    //Reduce total number of notes playing for one
    Log.trace(F("\tReduce total number of notes playing for one"CR));
    notesPlaying[0] = notesPlaying[0]-1;
    Log.trace(F("\tTotal notes still playing in cache: %d"CR), notesPlaying[0]);
  }

  Log.trace(F("MidiPlayer.removeNotePlayingFromCache(index) function exit."CR));
}

void MidiPlayer::addNotePlayingToCache(int tempOctave, int tempIndex) {
  Log.trace(F("MidiPlayer.addNotePlayingToCache(index) function call."CR));
  Log.trace(F("\twith index: %d into cache index %d"CR), tempIndex, notesPlaying[0]*2+2);

  //If no notes are playing at the moment
  //store channel info as well
  if (notesPlaying[0] == 0) {
    Log.trace(F("\tNo notes are playing so storing the Channel as well to cache"CR));
    notesPlaying[1] = mp_settings->getChannel();
  }

  //increase the number of notes playing
  Log.trace(F("\tIncrease the number of notes playing for one"CR));
  notesPlaying[0]=notesPlaying[0] + 1;

  // add a note information (octave and index) to the end of the list
  notesPlaying[notesPlaying[0]*2] = tempOctave;
  notesPlaying[notesPlaying[0]*2+1] = tempIndex;

  Log.trace(F("MidiPlayer.addNotePlayingToCache(index) function exit."CR));
}

// this function is used when device is going into mode that shoudl notplay anything 
void MidiPlayer::stopNotes() {
  Log.trace(F("MidiPlayer.stopNotes() function call."CR));
  if (isNotePlaying()) {
    for (int i = 2; i <= notesPlaying[0]*2; i = i + 2) {
      // do it with previous octave settings and channel settings
      playNoteOff(notesPlaying[i+1], notesPlaying[i], notesPlaying[1]);
    }
    notesPlaying[0] = 0;
  }
  Log.trace(F("MidiPlayer.stopNotes() function exit."CR));
}

