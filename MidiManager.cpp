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

MidiManager::MidiManager()
{
	midiInputDevices = MidiInput::getDevices();
	midiOutputDevices = MidiOutput::getDevices();
}

MidiManager::~MidiManager()
{	
	inputList.clear();
	midiInputDevices.clear();
	inputInstances.clear();
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

MultiOutMidiIn* MidiManager::getInput(String name)
{
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
	return getInputIndexFromName(name) != -1;
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

MidiInputProcessor* MidiManager::getNewMidiInputProcessorInstance()
{
	MidiInputProcessor* inputProcessor = new MidiInputProcessor();
	inputInstances.push_back(inputProcessor);

	return inputProcessor;
}

