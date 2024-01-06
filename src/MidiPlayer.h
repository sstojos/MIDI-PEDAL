#ifndef MIDIPLAYER_H_
#define MIDIPLAYER_H_

#ifndef SETTINGS_H_
#include "Settings.h"
#endif

#ifndef TEST
#include <SoftwareSerial.h>
#endif

#ifndef ARDUINOLOG_H_
#define ARDUINOLOG_H_
#include "ArduinoLog.h"
#endif

#ifndef SCREEN_H
#include "Screen.h"
#endif


class MidiPlayer {

#ifndef TEST
	//set MIDI serial output
	SoftwareSerial* midiSerial;
#endif

	// configure array that will store note name pointers
	char * noteFullName[13];

	// configure chars for single note
	char note0[2] = { 'C', '\0' };
	char note1[3] = { 'C', '#', '\0' };
	char note2[2] = { 'D', '\0' };
	char note3[3] = { 'D', '#', '\0' };
	char note4[2] = { 'E', '\0' };
	char note5[2] = { 'F', '\0' };
	char note6[3] = { 'F', '#', '\0' };
	char note7[2] = { 'G', '\0' };
	char note8[3] = { 'G', '#', '\0' };
	char note9[2] = { 'A', '\0' };
	char note10[3] = { 'A', '#', '\0' };
	char note11[2] = { 'B', '\0' };
	char note12[2] = { 'C', '\0' };

	// configure chars for minor chord names
	char minor0[3] = { 'C', 'm', '\0' };
	char minor1[4] = { 'C', '#', 'm', '\0' };
	char minor2[3] = { 'D', 'm','\0' };
	char minor3[4] = { 'D', '#', 'm', '\0' };
	char minor4[3] = { 'E', 'm', '\0' };
	char minor5[3] = { 'F', 'm', '\0' };
	char minor6[4] = { 'F', '#', 'm', '\0' };
	char minor7[3] = { 'G', 'm', '\0' };
	char minor8[4] = { 'G', '#', 'm', '\0' };
	char minor9[3] = { 'A', 'm', '\0' };
	char minor10[4] = { 'A', '#', 'm', '\0' };
	char minor11[3] = { 'B', 'm', '\0' };
	char minor12[3] = { 'C', 'm', '\0' };

	//MIDI command for note pitch 
    const int octaveM2[13] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C};
	const int octaveM1[13] = {0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18};
	const int  octave0[13] = {0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24};
	const int  octave1[13] = {0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30};
	const int  octave2[13] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C};
	const int  octave3[13] = {0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48};
	const int  octave4[13] = {0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54};
	const int  octave5[13] = {0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60};
	const int  octave6[13] = {0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C};
	const int  octave7[13] = {0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78};

	//MIDI command for note on and note off for each channel
	const int noteOffCommand[16] = {0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F};
	const int noteOnCommand[16] = {0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F};

	Settings * mp_settings = Settings::getInstance();


	// currentlyPlaying is an array that stores notes that are played that need to be at some point stopped 
	// MIDI foot pedal can play only one pedal at any  time
	// single pedal can play a chord with maximum 10 notes for most complex chords
	// chord set exist for midi player to know when to play chord of the same pedal but from a differetn set instead of stoppign existing playing. Here is the case
	// 		note off is switched off 
	//		pedal is currenlty playing
	//		chord set is chnaged after pedal is played
	//		the same pedal is pressed again
	// in this case chord from different set should be played insetad of stoping all notes only
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// | total notes playing | pedal index | chordset | channel | note1 octave | note1 index | note2 octave | note2 index | ... | note10 octave | note 10 index |
	// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	int currentlyPlaying[24] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	private:
		int getNoteOnCommand(int);
		void addNoteToCurrentlyPlayingArray(int, int, int, int);
		// play note based on note index, octave, channel and velocity
		void playNoteOn(int, int, int, int);
		// turn off note basewd on note index, octave, and channel
		void playNoteOff(int, int, int);
		// pedal on called in note mode
		void pedalOnNoteMode(int);
		// pedal on called in chord mode
		void pedalOnChordMode(int);
		// pedal release called in note mode
		void pedalOffNoteMode(int);
		// pedal release called in chord mode
		void pedalOffChordMode(int);
		// check if anything is playing
		boolean isNotePlaying();
		// check what is the last note sent to MIDI out
		int getLastPlayedPedal();
		//get a MIDI NOTE from index
		int getMidiNote(int, int);

	public:

		// this is used by MIDI PEDAL when PEDAL starts to initialize MIDI PLAYER data
		void init();
		
		// this is used by MIDI PEDAL when pedal is pressed
		void pedalOn(int);

		// this is used by MIDI PEDAL when pedal is released
		void pedalOff(int);

		//this is used by MIDI PEDAL to stop anythign playing when required (e.g. when swiching betwen playing modes or to configuration screens)
		void stopNotes();

};

#endif