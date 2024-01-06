/*
 * Settings
 *
 * Created on: 3 Feb 2017
 *      Author: sstoja00
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

using namespace std;



class Settings {

	/* 
	MIDI pedal state is actually the screen ID that is currenlty displayed
	It is important to know this becasue based on the screen pedals have different functions.
	
	1: Note playing
		11: Note settings
	
	2: Chord playing
		21: Chord settings
		22: Chord advanced settings

	3: Program/Command playing
		31: P/C settings
	*/

	int state ;

	/*
	Chord Type name describe chord type name  that is playing
	There are 6 main groups of chord types:
		1. Major
			11. Major
			12. Major 6
			13. Major 7
			14. Major 9
			15. Major 11
			16. Major 13
		2. Minor
			21. Minor
			22. Minor 6
			23. Minor 7
			24. Minor (Maj7)
			25. Minor 9
			26. Minor 11
		3. Dominant
			31. Dominant 7
			32. Dominant 9
			33. Dominant 11
			34. Dominant 13
			35. Dom 7 Sus 4
		4. Diminished
			41. Diminished
			42. Dimiminished 7
			43. Half Dimiminished 7
		5. Suspended
			51. Sus2
			52. Sus 4
		6. Augmented
			61. Augmented
			61. Augmented 7
	*/ 

	const char *chordTypeNames[6][6] = { 
		{"Major", "Major 6", "Major 7", "Major 9", "Major 11", "Major 13"},
		{"Minor", "Minor 6", "Minor 7", "Minor (Maj7)", "Minor 9", "Minor 11"},
		{"Dominant 7", "Dominant 9", "Dominant 11", "Dominant 13", "Dom 7 Sus 4", " "},
		{"Diminished", "Diminished 7", "Half Diminished 7", " ", " ", " "},
		{"Sus 2", "Sus 4", " ", " ", " ", " "},
		{"Augmented", "Augmented 7", " ", " ", " ", " "}
	};

	// chord configuration arrays
	// -------------------------------------------------------------------------------------------------------------------------------------------
	// | total number of notes in chord | first note interval after root note | second note interval after root note  | .. | fourth note interval
	// -------------------------------------------------------------------------------------------------------------------------------------------
	const int chordMajorConfigs[6][7] = {
											{3, 4, 7, 0, 0, 0, 0 }, // Major    (R-3-5)
											{4, 4, 7, 9, 0, 0, 0 }, // Major 6  (R-3-5-6)
											{4, 4, 7, 11,0, 0, 0 }, // Major 7  (R-3-5-7)
											{5, 4, 7, 11,14,0, 0 }, // Major 9  (R-3-5-7-9)
											{6, 4, 7, 11,14,17,0 }, // Major 11 (R-3-5-7-9-11)
											{7, 4, 7, 11,14,17,19}  // Major 13 (R-3-5-7-9-11-13)
										};

	const int chordMinorConfigs[6][6] = {
											{3, 3, 7, 0, 0, 0 }, // Minor         (R-b3-5)
											{4, 3, 7, 9, 0, 0 }, // Minor 6       (R-b3-5-6)
											{4, 3, 7, 10,0, 0 }, // Minor 7       (R-b3-5-b7)
											{4, 3, 7, 11,0, 0 }, // Minor (Maj7)  (R-b3-5-7)
											{5, 3, 7, 10,14,0 }, // Minor 9       (R-b3-5-b7-9)
											{6, 3, 7, 10,14,17}  // Minor 11      (R-b3-5-b7-9-11)
										};

	const int chordDominantConfigs[5][7] = {
											{4, 4, 7, 10,0, 0, 0 }, // Dominant 7  (R-3-5-b7)
											{5, 4, 7, 10,14,0, 0 }, // Dominant 9  (R-3-5-b7-9)
											{6, 4, 7, 10,14,17,0 }, // Dominant 11 (R-3-5-b7-9-11)
											{7, 4, 7, 10,14,17,19}, // Dominant 13 (R-3-5-b7-9-11-13)
											{4, 5, 7, 10,0, 0, 0 }  // Dom 7 Sus 4 (R-4-5-b7)
										};

	const int chordDiminishedConfigs[3][4] = {
											{3, 3, 6, 0 }, // Diminished        (R-b3-b5)
											{4, 3, 6, 9 }, // Diminished 7      (R-b3-b5-bb7)
											{4, 3, 6, 10}  // Half Diminished 7 (R-b3-b5-b7)
										};

	const int chordSuspendedConfigs[2][3] = {
											{3, 2, 7}, // Sus 2 (R-2-5)
											{3, 5, 7}  // Sus 4 (R-4-5)

										};

	const int chordAugmentedConfigs[2][4] = {
											{3, 4, 8, 0 }, // Augmented   (R-3-#5)
											{4, 4, 8, 10}  // Augmented 7 (R-3-#5-b7)
										};
 	
	int octave;
	int velocity;
	int channelNote;
	int channelChord;

	// current selected chordset
	int chordSet;

	/* array that holds settings for each chord set
	 	- rootNote
	 	- mode ( Ionian, Dorian, Phrygian, Lydian, Mixolydian, Aeolian, Locrian )
	 	- Octave
	*/
	int chordSetSettings[3][3];


	bool noteOff;
	bool doubleNote;

//	const char chordType[3] = { ' ', 'm', 'd'};


	//major scale represented as number of intervals  between each note - for two octaves so that all 7 chords for all modes can be retrived by the same index
	int ionianScale[16] =     {2,2,1,2,2,2,1,2,2,1,2,2,2,1,2,2};
    //                         C,D,E,F,G,A,B,C,D,E,F,G,A,B,C

	// settings for chord sets
	// used for storing chord notes for each pedal
	// there could be three chord set (three  harmonies) which can be circled through by pressing chord foor switch
	// the first array in matrix  is array of pedal indexes
	// the secord array is an array that define all notes for playing the chord for pedal at a single index and chord type for display
	// ----------------------------------------------------------------------------------------------------------------------------------
	// | total notes playing | note1 octave | note1 index | note2 octave | note2 index | ... | note10 octave | note 10 index | chordType
	// ----------------------------------------------------------------------------------------------------------------------------------
	int chordsSetOne[13][22] = {0};
	int chordsSetTwo[13][22] = {0};
	int chordsSetThree[13][22] = {0};


	private:

		static Settings * inst_;   // The one, single instance of settings
		//this is used to set note and mode for chord playing
		void setModeChords(int, int, int, int);
		//get a chord type 0: major, 1: minor and 2: diminished
		int evalChordType(int, int);

	public:
	 	int (*chordsSetPointer)[22];
	    static Settings * getInstance();

		void init();
	
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

};

#endif

