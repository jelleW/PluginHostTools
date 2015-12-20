#ifndef PLUGINMANAGER_H_INCLUDED
#define PLUGINMANAGER_H_INCLUDED

#include <iostream>
#include <map>

#include "../../JuceLibraryCode/JuceHeader.h"

#pragma once
class PluginManager
{

// STATIC STUFF
public:
	static PluginManager* getInstance();
	static void clear();
	~PluginManager();

	class PluginEditor : public DocumentWindow
	{
	public:
		static PluginEditor* getPluginEditor(AudioProcessorGraph::Node* pluginNode);
		static void clear();

		PluginEditor(String name, AudioProcessorGraph::Node* pluginNode);
		~PluginEditor();
		void closeButtonPressed() override;

	private:
		ScopedPointer<AudioProcessorEditor> editor;
		AudioProcessorGraph::Node* pluginNode;
		float getDesktopScaleFactor() const override { return 1.0f; }

		static std::map<int, PluginEditor*> nodeEditors;
	};

private:
	static PluginManager* instance;

// MEMBER STUFF
	PluginManager();

public:
	AudioPluginInstance* getNewPluginInstance(int n, double sampleRate, double blockSize);
	AudioPluginInstance* getNewPluginInstance(String identifierString, double sampleRate, double blockSize);
	AudioPluginInstance* getNewPluginInstanceFromDesc(PluginDescription* desc, double sampleRate, double blockSize);


	PluginDescription* getPluginDescription(int n);
	PluginDescription* getPluginDescription(String identifierString);

	PluginEditor* getPluginEditor(AudioProcessorGraph::Node* pluginNode);

	AudioPluginFormatManager formatManager;
	std::vector<String> getKnownPluginNames();

	ScopedPointer<AudioProcessorGraph::Node> outputNode;

private:
	std::vector<AudioPluginInstance*> pluginInstances;

	class KnownPluginScanner
	{
	public:
		KnownPluginScanner(AudioPluginFormatManager* _formatManager);
		~KnownPluginScanner();

		void addPluginFolder(String folder, bool searchSubFolders = true);
		void removePluginFolder(String folder);

		std::map<String, bool> getPluginFolders();

		void rescan();

		KnownPluginList* getKnownPluginList();

	private:
		std::map<String, bool> pluginFolders;
		void ScanFolder(String folder, AudioPluginFormat &format, bool searchSubFolders = true, bool dontRescanIfAlreadyInList = true);

		XmlElement* createXmlPluginDirectories() const;
		XmlElement* createXmlPluginDirectory(String name, bool rec) const;

		bool loadSavedPluginDirectories();

		KnownPluginList knownPluginList;
		AudioPluginFormatManager* formatManager;
		KnownPluginList::SortMethod pluginSortMethod;
	};

	PluginManager::KnownPluginScanner knownPluginScanner;

public:
	PluginManager::KnownPluginScanner* getPluginScanner();
};

#endif // PLUGINMANAGER_H_INCLUDED