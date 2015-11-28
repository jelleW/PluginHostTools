#include "AudioGraphManager.h"
#include "Application.h"

#include <iostream>
#include <map>

#include "MidiManager.h"

AudioGraphManager* AudioGraphManager::instance;

AudioGraphManager* AudioGraphManager::getInstance()
{
	if (AudioGraphManager::instance == nullptr)
		AudioGraphManager::instance = new AudioGraphManager();
	return AudioGraphManager::instance;
}

AudioGraphManager* AudioGraphManager::getNonUniqueInstance()
{
	return new AudioGraphManager();
}

void AudioGraphManager::clear()
{
	delete AudioGraphManager::instance;
}

/// MEMBER STUFF ///

AudioGraphManager::AudioGraphManager()
{
	inputNode = graph.addNode(new AudioProcessorGraph::AudioGraphIOProcessor(AudioProcessorGraph::AudioGraphIOProcessor::audioInputNode));
	outputNode = graph.addNode(new AudioProcessorGraph::AudioGraphIOProcessor(AudioProcessorGraph::AudioGraphIOProcessor::audioOutputNode));
}

AudioGraphManager::~AudioGraphManager()
{
	graph.clear();
	graphPlayer.setProcessor(nullptr);
	deviceManager->removeAudioCallback(&graphPlayer);
}

void AudioGraphManager::init(AudioDeviceManager* _deviceManager)
{
	deviceManager = _deviceManager;
	graphPlayer.setProcessor(&graph);
	deviceManager->addAudioCallback(&graphPlayer);

	AudioDeviceManager::AudioDeviceSetup setup;
	deviceManager->getAudioDeviceSetup(setup);

	graph.prepareToPlay(setup.sampleRate, setup.bufferSize);
}

void AudioGraphManager::reset()
{
	for (int i = 0; i < graph.getNumConnections(); i++)
		graph.removeConnection(i);
	
	/*
	for (int i = 0; i < graph.getNumNodes(); i++)
		for (int j = 0; j < graph.getNumNodes(); j++)
			if (graph.isConnected(i, j))
				for (int ch1 = 0; ch1 < graph.getNode(i)->getProcessor()->getNumOutputChannels(); ch1++)
					for (int ch2 = 0; ch2 < graph.getNode(j)->getProcessor()->getNumInputChannels(); ch2++)
						graph.removeConnection(graph.getNode(i)->nodeId, ch1, graph.getNode(j)->nodeId, ch2);
						*/
}

int AudioGraphManager::createNode(AudioProcessor* instance)
{
	return (int)graph.addNode(instance)->nodeId;
}

int AudioGraphManager::createMidiReceivingNode(AudioProcessor* instance)
{
	int instanceNodeId = createNode(instance);
	
	MidiInputProcessor* midiInput = MidiManager::getInstance()->getNewMidiInputProcessorInstance();
	int midiInputNodeId = createNode(midiInput);

	getNode(instanceNodeId)->properties.set("MidiReceiver", midiInputNodeId);
	getNode(midiInputNodeId)->properties.set("IsMidiReceiver", true);

	connectMidiNodes(midiInputNodeId, instanceNodeId, -1);

	return instanceNodeId;
}

bool AudioGraphManager::isMidiReceivingNode(int nodeId)
{
	return getNode(nodeId)->properties.contains("MidiReceiver");
}

MidiMessageCollector* AudioGraphManager::getMidiMessageCollectorForNode(int nodeId)
{
	if (!isMidiReceivingNode(nodeId))
		return nullptr;

	return ((MidiInputProcessor*)(graph.getNodeForId((int)(getNode(nodeId)->properties["MidiReceiver"]))->getProcessor()))->getMidiMessageCollector();
}

AudioProcessorGraph::Node* AudioGraphManager::getNode(int nodeId)
{
	AudioProcessorGraph::Node* node = graph.getNodeForId(nodeId);
	if (node->properties.contains("IsMidiReceiver"))
		return nullptr;
	return node;
}

bool AudioGraphManager::connectNodes(int node1Id, int node2Id, std::map<int, std::vector<int>> channels)
{
	bool result = true;
	for (std::map<int, std::vector<int>>::iterator n1ChIt = channels.begin(); n1ChIt != channels.end(); ++n1ChIt)
		for (std::vector<int>::iterator n2ChIt = n1ChIt->second.begin(); n2ChIt != n1ChIt->second.end(); ++n2ChIt)
			result = result && graph.addConnection(node1Id, n1ChIt->first, node2Id, *n2ChIt);
	return result;
}

bool AudioGraphManager::connectMidiNodes(int node1Id, int node2Id, int channel)
{
	return graph.addConnection(node1Id, graph.midiChannelIndex, node2Id, graph.midiChannelIndex);
}

bool AudioGraphManager::connectNodeToInput(int nodeId, std::map<int, std::vector<int>> channels)
{
	return this->connectNodes(inputNode->nodeId, nodeId, channels);
}

bool AudioGraphManager::connectNodeToOutput(int nodeId, std::map<int, std::vector<int>> channels)
{
	return this->connectNodes(nodeId, outputNode->nodeId, channels);
}

int AudioGraphManager::getSampleRate()
{
	return graph.getSampleRate();
}

int AudioGraphManager::getBlockSize()
{
	return graph.getBlockSize();
}


std::map<int, std::vector<int>> AudioGraphManager::Channels::getMONO(int inputChannel, int leftOutputChannel)
{
	std::map<int, std::vector<int>> map;
	std::vector<int> v1;
	v1.push_back(leftOutputChannel);
	v1.push_back(leftOutputChannel + 1);
	map[inputChannel] = v1;

	return map;
}

std::map<int, std::vector<int>> AudioGraphManager::Channels::getSTEREO(int leftInputChannel, int leftOutputChannel)
{
	std::map<int, std::vector<int>> map;
	std::vector<int> v1;
	std::vector<int> v2;
	v1.push_back(leftOutputChannel);
	v2.push_back(leftOutputChannel + 1);
	map[leftInputChannel] = v1;
	map[leftInputChannel + 1] = v2;

	return map;
}