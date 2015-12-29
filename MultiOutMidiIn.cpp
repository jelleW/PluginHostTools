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

String MultiOutMidiIn::getDevName()
{
	return devName;
}

void MultiOutMidiIn::start()
{
	midiInput->start();
}

void MultiOutMidiIn::stop()
{
	midiInput->stop();
}

void MultiOutMidiIn::addListener(String name, MidiInputCallback* callback)
{
	if(listeners.find(name) == listeners.end())
		listeners[name] = callback;
}

void MultiOutMidiIn::handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message)
{
	for (std::map<String, MidiInputCallback*>::iterator it = listeners.begin(); it != listeners.end(); ++it)
		it->second->handleIncomingMidiMessage(source, message);
}