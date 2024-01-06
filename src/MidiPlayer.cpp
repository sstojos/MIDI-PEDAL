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
  
  //Initialize  currentlyPlaying array with no notes playing
  currentlyPlaying[0] = 0;

  Log.trace(F("MidiPlayer.init() function exit."CR));

}



boolean MidiPlayer::isNotePlaying() {
   Log.trace(F("MidiPlayer.isNotePlaying() function called."CR));
   Log.trace(F("\tchecking if currenltyPlaying[0] is 0."CR));
  if (currentlyPlaying[0] == 0) {
    return false;
  } else {
    return true;
  }
  Log.trace(F("MidiPlayer.isNotePlaying() function exit."CR));
}

int MidiPlayer::getLastPlayedPedal() {
  Log.trace(F("MidiPlayer.getLastNote() function called."CR));
  return currentlyPlaying[1];
  Log.trace(F("MidiPlayer.getLastNote() function exit."CR));
}

// plays a MIDI note ON based on
// note index, octave, channel and velocity 
void MidiPlayer::playNoteOn(int index, int octave, int channel, int velocity) {
  Log.trace(F("MidiPlayer.playNotedOn(index,octave,channel,velocity) function called. Sending midi note on for pedal index: %d, for ocatve: %d, on channel: %d, with velocity: %d"CR), index, octave, channel, velocity);

  midiSerial->write(noteOnCommand[channel-1]);  //noteOn command is 144 for channel 1
  midiSerial->write(getMidiNote(index, octave)); //pitch
  midiSerial->write(velocity);
  
  Log.trace(F("MidiPlayer.playNoteOn(index,octave,channel,velocity) function exit."CR));
}

// called by pedalOn function when pedal is in note mode
void MidiPlayer::pedalOnNoteMode(int index) {
  
  Log.trace(F("MidiPlayer.pedalOnNoteMode() function called."CR));

  Log.trace(F("Check that Note is not playing or request note is not for a last note played. Is note playing is %d, and last played pedal %d"CR), isNotePlaying(), getLastPlayedPedal());
  if (!isNotePlaying() || getLastPlayedPedal() != index)  {

    //first stop all notes
    stopNotes();

    //play note requested last
    playNoteOn(index, mp_settings->getOctave(), mp_settings->getChannelNote(), mp_settings->getVelocity());

    //add note played to currenltyPlaying array
    addNoteToCurrentlyPlayingArray(index, index, mp_settings->getChannelNote(), mp_settings->getOctave());

    // check if two note shousld be played 
    if (mp_settings->getDoubleNote() == true ) {
      //play requested note one octave higher
      playNoteOn(index, mp_settings->getOctave()+1, mp_settings->getChannelNote(), mp_settings->getVelocity());
      //add note played to currenltyPlaying array
      addNoteToCurrentlyPlayingArray(index, index, mp_settings->getChannelNote(), mp_settings->getOctave()+1);
    }

  } else {
    //first stop all notes
    stopNotes();
  }
  
  Log.trace(F("\tChecking if there are anything to print on screen."CR));
  if(isNotePlaying()) {
    //there are more notes playing - show last one on screen
    Log.trace(F("\tThere are more notes to play. Rendering the screen with last playing note in queue ..."CR));
    //TODO: screenPointer->renderNote(getLastNote());

    renderNote(noteFullName[getLastPlayedPedal()]);

  } else {
    //there are no more notes playing - clear the screen
    Log.trace(F("\tThere are no more notes playing - clear the screen"CR));
    renderNote("");
  }

  Log.trace(F("MidiPlayer.pedalOnNoteMode( function exit."CR));
}

// called by pedalOn function when pedal is in chord mode
void MidiPlayer::pedalOnChordMode(int index) {
  
  Log.trace(F("MidiPlayer.pedalOnChordMode() function called."CR));

  // ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// | total notes playing | pedal index | chordset | channel | note1 octave | note1 index | note2 octave | note2 index | ... | note10 octave | note 10 index |
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  // check that pedal is not already playing this pedal index from the same chord set- if yes do not play it again
  Log.trace(F("MidiPlayer.pedalOnChordMode(). Check should chord be play by checking that device is not playing or if playing that last pedal index is not the same. isPlaying is %d and lastPlayingPedal is %d, last chord set was %d and index is "CR), isNotePlaying(), getLastPlayedPedal(), currentlyPlaying[2], index);
  if (!isNotePlaying() || getLastPlayedPedal() != index || mp_settings->getChordSet() != currentlyPlaying[2]) {

    //always stopNotes first for chords - do not allow simultaniun pedal presses
    stopNotes();

    Log.trace(F("MidiPlayer.pedalOnChordMode(). Number of notes that should be played is %d "CR), mp_settings->chordsSetPointer[index][0]);
    for (int i = 0; i < mp_settings->chordsSetPointer[index][0]; i++)
    {
      //play note
      playNoteOn(mp_settings->chordsSetPointer[index][i*2+2], mp_settings->chordsSetPointer[index][i*2+1], mp_settings->getChannelChord(), mp_settings->getVelocity());
      //add note1 played to cache
      addNoteToCurrentlyPlayingArray(index, mp_settings->chordsSetPointer[index][i*2+2], mp_settings->getChannelChord(), mp_settings->chordsSetPointer[index][i*2+1]);
    }

  } else {
    //same pedal is called - stopNotes only
    stopNotes();
  }

  Log.trace(F("\tChecking if there are anything to print on screen."CR));
  if(isNotePlaying()) {
    renderChord(index);
  } else {
    renderChord(13);
  }

  Log.trace(F("MidiPlayer.pedalOnChordMode( function exit."CR));
}

void MidiPlayer::pedalOn(int index) {
  Log.trace(F("MidiPlayer.pedalOn( function entry with index: %d"CR), index);
  switch (mp_settings->getState())
  {
  case 1:
    pedalOnNoteMode(index);
    break;
  case 2:
    pedalOnChordMode(index);
    break;
  
  default:
    break;
  }
  Log.trace(F("MidiPlayer.pedalOn( function exit."CR));
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// | total notes playing | pedal index | chordset | channel | note1 octave | note1 index | note2 octave | note2 index | ... | note10 octave | note 10 index |
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MidiPlayer::addNoteToCurrentlyPlayingArray(int pedalIndex, int noteIndex, int channel, int octave) {
    Log.trace(F("MidiPlayer.addNoteToCurrentlyPlayingArray() function entry witn pedalIndex: %d, noteIndex: %d,channel: %d and octave: %d"CR), pedalIndex, noteIndex, channel, octave);
    //add note played to currenltyPlaying array
    currentlyPlaying[0] = currentlyPlaying[0] + 1;
    currentlyPlaying[1] = pedalIndex;
    currentlyPlaying[2] = mp_settings->getChordSet();
    currentlyPlaying[3] = channel;
    currentlyPlaying[(currentlyPlaying[0]*2)+2] =  octave;
    currentlyPlaying[(currentlyPlaying[0]*2)+3] =  noteIndex;
}

// plays a MIDI note Off based on
// note index, octave, channel and velocity 
void MidiPlayer::playNoteOff(int index, int octave, int channel) {
  Log.trace(F("MidiPlayer.playNoteOff(index, octave, channel) function called. Sending midi note off for pedal index: %d, for octave: %d, on channel: %d"CR), index, octave, channel);
  midiSerial->write(noteOffCommand[channel-1]); //noteOff command is 128 for channel 1
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
    Log.trace(F("\tStopping all notes. "CR));
    stopNotes();
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
        renderChord(13);
    }
  }

  Log.trace(F("MidiPlayer.pedalOffChordMode(index) function exit."CR));
}

//called by pedal when releases 
void MidiPlayer::pedalOff(int index) {
  Log.trace(F("MidiPlayer.pedalOff(index) function called. Sending midi note off for pedal index: %d"CR), index);
  switch (mp_settings->getState())
  {
  case 1:
    pedalOffNoteMode(index);
    break;
  case 2:
    pedalOffChordMode(index);
    break;
  default:
    break;
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
  //make sure that octave never goes over 5
  if(myOctave > 5) {myOctave = 5;}
 
  //adjust index to 0-12 range
  myIndex = index%12;

 	switch (myOctave) {
		case -4:
		  return octaveM2[myIndex];
      break;
    case -3:
      return octaveM1[myIndex];
      break;
    case -2:
      return octave0[myIndex];
      break;
    case -1:
      return octave1[myIndex];
      break;
    case 0:
      return octave2[myIndex];
      break;
    case 1:
      return octave3[myIndex];
      break;
    case 2:
      return octave4[myIndex];
      break;
    case 3:
      return octave5[myIndex];
      break;
    case 4:
      return octave6[myIndex];
      break;
    case 5:
      return octave7[myIndex];
      break;
    default:
      return octave3[myIndex];
    break;
  }
  Log.trace(F("MidiPlayer.getMidiNote(index,octave) function exit."CR));
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// | total notes playing | pedal index | chordset | channel | note1 octave | note1 index | note2 octave | note2 index | ... | note10 octave | note 10 index |
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// this function is used when device is going into mode that shoudl notplay anything 
void MidiPlayer::stopNotes() {
  Log.trace(F("MidiPlayer.stopNotes() function call."CR));
  if (isNotePlaying()) {
    for (int i = 4; i <= currentlyPlaying[0]*2+2; i = i + 2) {
      // do it with previous octave settings and channel settings
      playNoteOff(currentlyPlaying[i+1], currentlyPlaying[i], currentlyPlaying[3]);
    }
    currentlyPlaying[0] = 0;
  }
  Log.trace(F("MidiPlayer.stopNotes() function exit."CR));
}

