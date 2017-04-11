/*
  ==============================================================================

    MidiManager.cpp
    Created: 21 Oct 2015 1:48:59pm
    Author:  weste

  ==============================================================================
*/

#include "MidiManager.h"

MidiManager* MidiManager::instance;

MidiManager* MidiManager::getInstance()
{
	if (MidiManager::instance == nullptr)
		MidiManager::instance = new MidiManager();
	return MidiManager::instance;
}

void MidiManager::clear()
{
	delete MidiManager::instance;
}

void MidiManager::addActiveDevicesChangeListener(MidiDevicesChangeListener* listener)
{
	midiDevicesChangeListeners.add(listener);
}

MidiManager::MidiManager()
	: midiDevicesListenerThread(&MidiManager::midiDevicesListener, this)
{
	availableMidiInputDevices = MidiInput::getDevices();
	availableMidiOutputDevices = MidiOutput::getDevices();
}

std::thread MidiManager::midiDevicesListener()
{
	while (searchForMidiDevices)
	{
		if (availableMidiInputDevices.size() != MidiInput::getDevices().size() || 
			availableMidiOutputDevices.size() != MidiOutput::getDevices().size())
		{
			availableMidiInputDevices = MidiInput::getDevices();
			availableMidiOutputDevices = MidiOutput::getDevices();
			
			const MessageManagerLock mmLock;
			for each (MidiDevicesChangeListener* listener in midiDevicesChangeListeners)
			{
				listener->activeMidiDevicesListChanged();
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
	}
	return{};
}

MidiManager::~MidiManager()
{	
	inputList.clear();
	inputInstances.clear();

	searchForMidiDevices = false;
	midiDevicesListenerThread.join();
}

int MidiManager::getInputIndexFromName(String name)
{
	for (int i = 0; i < availableMidiInputDevices.size(); i++)
		if (availableMidiInputDevices[i] == name)
			return i;
	return -1;
}

int MidiManager::getOutputIndexFromName(String name)
{
	for (int i = 0; i < availableMidiOutputDevices.size(); i++)
		if (availableMidiOutputDevices[i] == name)
			return i;
	return -1;
}

MultiOutMidiIn* MidiManager::getInput(String name)
{
	if (name.isEmpty())
		return nullptr;

	return getInput(getInputIndexFromName(name));
}

MultiOutMidiIn* MidiManager::getInput(int index)
{
	if (index == -1)
		return nullptr;

	if (inputList.find(availableMidiInputDevices[index]) != inputList.end())
		return inputList[availableMidiInputDevices[index]];

	MultiOutMidiIn* midiInput = new MultiOutMidiIn(index);

	inputList[availableMidiInputDevices[index]] = midiInput;

	return midiInput;
}

MidiOutput* MidiManager::getOutput(String name)
{
	int index = getOutputIndexFromName(name);

	return getOutput(index);
}

MidiOutput* MidiManager::getOutput(int index)
{
	if (index == -1)
		return nullptr;

	if (outputList.find(availableMidiOutputDevices[index]) != outputList.end())
		return outputList[availableMidiOutputDevices[index]];

	MidiOutput* midiOutput= MidiOutput::openDevice(index);

	if(midiOutput != nullptr)
		outputList[availableMidiOutputDevices[index]] = midiOutput;

	return midiOutput;
}

bool MidiManager::inputExists(String name)
{
	return std::find(availableMidiInputDevices.begin(), availableMidiInputDevices.end(), name) != availableMidiInputDevices.end();
}

bool MidiManager::outputExists(String name)
{
	return std::find(availableMidiOutputDevices.begin(), availableMidiOutputDevices.end(), name) != availableMidiOutputDevices.end();
}


StringArray MidiManager::getMidiInputDevices()
{
	return availableMidiInputDevices;
}

StringArray MidiManager::getMidiOutputDevices()
{
	return availableMidiOutputDevices;
}

StringArray MidiManager::getAvailableDevices()
{
	StringArray devs = availableMidiInputDevices;
	for each (String odev in availableMidiOutputDevices)
	{
		if (!inputExists(odev))
			devs.add(odev);
	}
	return devs;
}
/*
MidiInput* MidiManager::getInput(String name)
{
	return getInput(getIndexFromName(name));
}

MidiInput* MidiManager::getInput(int index)
{
	if (index == -1)
		return nullptr;

	if (inputList.find(index) == inputList.end())
		return nullptr;

	return inputList[index];
}*/

void MidiManager::start()
{
	for (auto it = inputList.begin(); it != inputList.end(); ++it)
		it->second->start();
}

void MidiManager::stop()
{
	for (auto it = inputList.begin(); it != inputList.end(); ++it)
		it->second->stop();
}

void MidiManager::restart()
{
	stop();
	start();
}

MidiInputProcessor* MidiManager::getNewMidiInputProcessorInstance()
{
	MidiInputProcessor* inputProcessor = new MidiInputProcessor();
	inputInstances.add(inputProcessor);

	return inputProcessor;
}