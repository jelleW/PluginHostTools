#ifndef AUDIOGRAPHMANAGER_H_INCLUDED
#define AUDIOGRAPHMANAGER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiInputProcessor.h"
#include <iostream>
#include <map>
#include <memory>

class AudioGraphManager
{
// Singleton Stuff
public:
	static AudioGraphManager* getInstance();
	static AudioGraphManager* getNonUniqueInstance();
	static void clear();

	~AudioGraphManager();

private:
	static AudioGraphManager* instance;

// Member stuff
	AudioGraphManager();

public:
	void init(AudioDeviceManager* deviceManager);

	void reset();

	int createNode(AudioProcessor* instance);

	AudioProcessorGraph::Node* getNode(int nodeId);

	bool connectNodes(int node1Id, int node2Id, std::map<int, std::vector<int>> channels);
	bool connectMidiNodes(int node1Id, int node2Id, int channel); // -1 == omni

	bool connectNodeToInput(int nodeId, std::map<int, std::vector<int>> channels);
	bool connectNodeToOutput(int nodeId, std::map<int, std::vector<int>> channels);

	int createMidiReceivingNode(AudioProcessor* instance);
	bool isMidiReceivingNode(int nodeId);
	MidiMessageCollector* getMidiMessageCollectorForNode(int nodeId);

	int getSampleRate();
	int getBlockSize();

	static class Channels
	{
	public:
		static std::map<int, std::vector<int>> getMONO(int inputChannel = 0, int leftOutputChannel = 0);
		static std::map<int, std::vector<int>> getSTEREO(int leftInputChannel = 0, int leftOutputChannel = 0);
	};

private:
	AudioProcessorGraph graph;
	AudioProcessorPlayer graphPlayer;

	AudioProcessorGraph::Node* inputNode;
	AudioProcessorGraph::Node* outputNode;

	AudioProcessorGraph::Node* inputMidiNode;

	AudioDeviceManager* deviceManager;
	
	std::vector<String> midiInputDevices;

};

#endif // AUDIOGRAPHMANAGER_H_INCLUDED