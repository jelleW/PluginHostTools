/*
  ==============================================================================

    MultiOutMidiIn.cpp
    Created: 24 Oct 2015 5:35:06pm
    Author:  weste

  ==============================================================================
*/

#include "MultiOutMidiIn.h"
#include "MidiManager.h"

MultiOutMidiIn::MultiOutMidiIn(String _devName)
{
	devName = _devName;
	if (MidiManager::getInstance()->inputExists(devName))
		midiInput = MidiInput::openDevice(MidiManager::getInstance()->getInputIndexFromName(devName), this);
}

MultiOutMidiIn::MultiOutMidiIn(int index)
{
	midiInput = MidiInput::openDevice(index, this);
	devName = midiInput->getName();
}

MultiOutMidiIn::~MultiOutMidiIn()
{
	stop();
	delete midiInput;
}

void MultiOutMidiIn::start()
{
	midiInput->start();
}

void MultiOutMidiIn::stop()
{
	midiInput->stop();
}

void MultiOutMidiIn::addListener(MidiInputCallback* callback)
{
	listeners.push_back(callback);
}

void MultiOutMidiIn::handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message)
{
	for (int i = 0; i < listeners.size(); i++)
		listeners[i]->handleIncomingMidiMessage(source, message);
}