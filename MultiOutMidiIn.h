/*
  ==============================================================================

    MultiOutMidiIn.h
    Created: 24 Oct 2015 5:35:06pm
    Author:  weste

  ==============================================================================
*/

#ifndef MULTIOUTMIDIIN_H_INCLUDED
#define MULTIOUTMIDIIN_H_INCLUDED

#include "../../JuceLibraryCode/JuceHeader.h"
#include "<vector.h>"

class MultiOutMidiIn
	: public MidiInputCallback
{
public:
	MultiOutMidiIn(String _devName);
	MultiOutMidiIn(int index);

	~MultiOutMidiIn();

	virtual void handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message) override;
	void addListener(String name, MidiInputCallback* callback);
	
	String getDevName();

	void start();
	void stop();

private:
	std::map<String, MidiInputCallback*> listeners;
	String devName;

	MidiInput* midiInput;
};



#endif  // MULTIOUTMIDIIN_H_INCLUDED
