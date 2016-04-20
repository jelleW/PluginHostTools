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

void MidiManager::addActiveDeviceListener(Listener * listener)
{
	midiDeviceListeners.push_back(listener);
}

MidiManager::MidiManager()
	: midiDevicesListenerThread(&MidiManager::midiDevicesListener, this)
{
	availableMidiInputDevices = MidiInput::getDevices();
	//midiInputDevices = MidiInput::getDevices();
	midiOutputDevices = MidiOutput::getDevices();
}

std::thread MidiManager::midiDevicesListener()
{
	return {};
	//StringArray midiIn,
	while (true)
	{
		//if()
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

MidiManager::~MidiManager()
{	
	inputList.clear();
	midiInputDevices.clear();
	inputInstances.clear();

	midiDevicesListenerThread.join();
}

int MidiManager::getInputIndexFromName(String name)
{
	for (int i = 0; i < midiInputDevices.size(); i++)
		if (midiInputDevices[i] == name)
			return i;
	return -1;
}

int MidiManager::getOutputIndexFromName(String name)
{
	for (int i = 0; i < midiOutputDevices.size(); i++)
		if (midiOutputDevices[i] == name)
			return i;
	return -1;
}

int MidiManager::getCurrentInputDeviceIndexFromName(String name)
{
	for (auto it = availableMidiInputDevices.begin(); it != availableMidiInputDevices.end(); ++it)
		if (*it == name)
			return it - availableMidiInputDevices.begin();
	return -1;
}

MultiOutMidiIn* MidiManager::getInput(String name)
{
	int index = getInputIndexFromName(name);

	if (index == -1)
	{
		index = midiInputDevices.size();
		midiInputDevices.add(name);
		return inputList[index] = new MultiOutMidiIn(name);
	}

	return getInput(getInputIndexFromName(name));
}

MultiOutMidiIn* MidiManager::getInput(int index)
{
	if (index == -1)
		return nullptr;

	if (inputList.find(index) != inputList.end())
		return inputList[index];

	MultiOutMidiIn* midiInput = new MultiOutMidiIn(index);

	inputList[index] = midiInput;

	return midiInput;
}

MidiOutput* MidiManager::getOutput(String name)
{
	return getOutput(getOutputIndexFromName(name));
}

MidiOutput* MidiManager::getOutput(int index)
{
	if (index == -1)
		return nullptr;

	if (outputList.find(index) != outputList.end())
		return outputList[index];

	MidiOutput* midiOutput= MidiOutput::openDevice(index);

	if(midiOutput != nullptr)
		outputList[index] = midiOutput;

	return midiOutput;
}

bool MidiManager::inputExists(String name)
{
	return std::find(availableMidiInputDevices.begin(), availableMidiInputDevices.end(), name) != availableMidiInputDevices.end();
}

StringArray MidiManager::getActiveInputDevices()
{
	StringArray activeInputDevs;

	for each (String devName in midiInputDevices)
	{
		if (std::find(availableMidiInputDevices.begin(), availableMidiInputDevices.end(), devName) != availableMidiInputDevices.end())
			activeInputDevs.add(devName);
	}

	return activeInputDevs;
}

StringArray MidiManager::getInactiveInputDevices()
{
	StringArray inactiveInputDevs;

	for (auto it = midiInputDevices.begin(); it = midiInputDevices.end(); ++it)
		if (std::find(availableMidiInputDevices.begin(), availableMidiInputDevices.end(), *it) == availableMidiInputDevices.end())
			inactiveInputDevs.add(*it);
	
	availableMidiInputDevices.clear();

	return inactiveInputDevs;
}

bool MidiManager::outputExists(String name)
{
	return getOutputIndexFromName(name) != -1;
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

