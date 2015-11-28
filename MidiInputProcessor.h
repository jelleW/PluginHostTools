/*
  ==============================================================================

    MidiInputProcessor.h
    Created: 21 Oct 2015 12:48:26pm
    Author:  weste

  ==============================================================================
*/

#ifndef MIDIINPUTPROCESSOR_H_INCLUDED
#define MIDIINPUTPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioGraphManager.h"

class MidiInputProcessor : public AudioProcessor
{
public:
	MidiInputProcessor();
	~MidiInputProcessor();

	MidiMessageCollector* getMidiMessageCollector();
	void processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages) override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;

private:
	MidiMessageCollector collector;

	// Inherited via AudioProcessor
	virtual const String getName() const override;
	virtual void prepareToPlay(double sampleRate, int estimatedSamplesPerBlock) override;
	virtual void releaseResources() override;
	virtual const String getInputChannelName(int channelIndex) const override;
	virtual const String getOutputChannelName(int channelIndex) const override;
	virtual bool isInputChannelStereoPair(int index) const override;
	virtual bool isOutputChannelStereoPair(int index) const override;
	virtual bool silenceInProducesSilenceOut() const override;
	virtual double getTailLengthSeconds() const override;
	virtual AudioProcessorEditor * createEditor() override;
	virtual bool hasEditor() const override;
	virtual int getNumPrograms() override;
	virtual int getCurrentProgram() override;
	virtual void setCurrentProgram(int index) override;
	virtual const String getProgramName(int index) override;
	virtual void changeProgramName(int index, const String & newName) override;
	virtual void getStateInformation(juce::MemoryBlock & destData) override;
	virtual void setStateInformation(const void * data, int sizeInBytes) override;
};



#endif  // MIDIINPUTPROCESSOR_H_INCLUDED
