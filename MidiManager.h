/*
  ==============================================================================

    MidiManager.h
    Created: 21 Oct 2015 1:48:59pm
    Author:  weste

  ==============================================================================
*/

#ifndef MIDIMANAGER_H_INCLUDED
#define MIDIMANAGER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "MultiOutMidiIn.h"
#include "MidiInputProcessor.h"

#include <map>

class MidiManager
{
public:
	static MidiManager* getInstance();
	static void clear();

private:
	static MidiManager* instance;

public:
	~MidiManager();
	MidiInputProcessor* getNewMidiInputProcessorInstance();

	bool inputExists(String name);

	MultiOutMidiIn* getInput(String name);
	MultiOutMidiIn* getInput(int index);

	void start();
	void stop();

	/*
	MidiInput* getInput(String name);
	MidiInput* getInput(int index);*/

	int getIndexFromName(String name);

private:
	MidiManager();
	
	StringArray midiInputDevices;
	std::map<int, MultiOutMidiIn*> inputList;
	
	std::vector<MidiInputProcessor*> inputInstances;


};



#endif  // MIDIMANAGER_H_INCLUDED