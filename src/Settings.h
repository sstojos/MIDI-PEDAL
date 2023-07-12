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

	int octave;
	int velocity;
	int channel;
	bool noteOff;
	bool doubleNote;

	const char chordType[3] = { ' ', 'm', 'd'};

	//chord playing
	bool chordPlaying;

	private:

		static Settings * inst_;   // The one, single instance of settings

	public:
	    static Settings * getInstance();
		void init();
		void setScreenConfigLabels();
		void setOctave(int);
		void setVelocity(int);
		void setChannel(int);
		void setNoteOff(bool);
		void toggleNoteOff();
		void setDoubleNote(bool);
		void toggleDoubleNote();
		int getOctave();
		int getVelocity();
		int getChannel();
		bool getNoteOff();
		bool getDoubleNote();
		char getChordType(int type);
		bool getChordPlaying();
		void setChordPlaying(bool);
};

#endif

