/*
  ==============================================================================

    MidiManager.h
    Created: 21 Oct 2015 1:48:59pm
    Author:  weste

  ==============================================================================
*/

#ifndef MIDIMANAGER_H_INCLUDED
#define MIDIMANAGER_H_INCLUDED

#include "../../JuceLibraryCode/JuceHeader.h"
#include "MultiOutMidiIn.h"
#include "MidiInputProcessor.h"

#include <map>
#include <thread>

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
	bool outputExists(String name);

	MultiOutMidiIn* getInput(String name);
	MultiOutMidiIn* getInput(int index);

	MidiOutput* getOutput(String name);
	MidiOutput* getOutput(int index);

	void start();
	void stop();

	void restart();

	/*
	MidiInput* getInput(String name);
	MidiInput* getInput(int index);*/

	int getInputIndexFromName(String name);
	int getOutputIndexFromName(String name);

	int getCurrentInputDeviceIndexFromName(String name);

	StringArray getActiveInputDevices();
	StringArray getInactiveInputDevices();

	class MidiDevicesChangeListener
	{
	public:
		virtual void activeMidiDevicesListChanged() = 0;
	};
	
	void addActiveDevicesChangeListener(MidiDevicesChangeListener* listener);

private:
	MidiManager();
	
	StringArray midiInputDevices;
	StringArray midiOutputDevices;

	StringArray availableMidiInputDevices, availablemidiOutputDevices;

	Array<MidiManager::MidiDevicesChangeListener*> midiDevicesChangeListeners;

	std::map<int, ScopedPointer<MultiOutMidiIn>> inputList;
	std::map<int, ScopedPointer<MidiOutput>> outputList;
	
	Array<MidiInputProcessor*> inputInstances;

	std::thread midiDevicesListener();
	std::thread midiDevicesListenerThread;
};



#endif  // MIDIMANAGER_H_INCLUDED