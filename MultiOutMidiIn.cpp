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
	if(listenersMap.find(name) == listenersMap.end())
		listenersMap[name] = callback;
}

void MultiOutMidiIn::addListener(MidiInputCallback* callback)
{
	listeners.push_back(callback);
}

void MultiOutMidiIn::handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message)
{
	for (std::map<String, MidiInputCallback*>::iterator it = listenersMap.begin(); it != listenersMap.end(); ++it)
		it->second->handleIncomingMidiMessage(source, message);

	for (std::vector<MidiInputCallback*>::iterator it2 = listeners.begin(); it2 != listeners.end(); ++it2)
		((MidiInputCallback*)*it2)->handleIncomingMidiMessage(source, message);
}