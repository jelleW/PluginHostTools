/*
  ==============================================================================

    MidiInputProcessor.cpp
    Created: 21 Oct 2015 12:48:26pm
    Author:  weste

  ==============================================================================
*/

#include "MidiInputProcessor.h"

MidiInputProcessor::MidiInputProcessor()
{
	collector.reset(AudioGraphManager::getInstance()->getSampleRate());
}

MidiInputProcessor::~MidiInputProcessor()
{
	collector.reset(AudioGraphManager::getInstance()->getSampleRate());
}

bool MidiInputProcessor::acceptsMidi() const
{
	return false;
}

bool MidiInputProcessor::producesMidi() const
{
	return true;
}

const String MidiInputProcessor::getName() const
{
	return String();
}

void MidiInputProcessor::prepareToPlay(double sampleRate, int estimatedSamplesPerBlock)
{
}

void MidiInputProcessor::releaseResources()
{
}

const String MidiInputProcessor::getInputChannelName(int channelIndex) const
{
	return String();
}

const String MidiInputProcessor::getOutputChannelName(int channelIndex) const
{
	return String();
}

bool MidiInputProcessor::isInputChannelStereoPair(int index) const
{
	return false;
}

bool MidiInputProcessor::isOutputChannelStereoPair(int index) const
{
	return false;
}

bool MidiInputProcessor::silenceInProducesSilenceOut() const
{
	return false;
}

double MidiInputProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

AudioProcessorEditor * MidiInputProcessor::createEditor()
{
	return nullptr;
}

bool MidiInputProcessor::hasEditor() const
{
	return false;
}

int MidiInputProcessor::getNumPrograms()
{
	return 0;
}

int MidiInputProcessor::getCurrentProgram()
{
	return 0;
}

void MidiInputProcessor::setCurrentProgram(int index)
{
}

const String MidiInputProcessor::getProgramName(int index)
{
	return String();
}

void MidiInputProcessor::changeProgramName(int index, const String & newName)
{
}

void MidiInputProcessor::getStateInformation(juce::MemoryBlock & destData)
{
}

void MidiInputProcessor::setStateInformation(const void * data, int sizeInBytes)
{
}

MidiMessageCollector* MidiInputProcessor::getMidiMessageCollector()
{
	return &collector;
}

void MidiInputProcessor::processBlock(AudioSampleBuffer &buffer, MidiBuffer &midiMessages)
{
	midiMessages.clear();
	collector.removeNextBlockOfMessages(midiMessages, this->getSampleRate());
}