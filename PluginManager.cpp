#include <windows.h>
#include "PluginManager.h"
#include "Application.h"

PluginManager* PluginManager::instance;

PluginManager* PluginManager::getInstance()
{
	if (PluginManager::instance == nullptr)
		PluginManager::instance = new PluginManager();
	return PluginManager::instance;
}

void PluginManager::clear()
{
	PluginManager::PluginEditor::clear();
	delete PluginManager::instance;
}

//////////////////////////////////////////

PluginManager::PluginManager()
	:knownPluginScanner(&formatManager)
{
	formatManager.addDefaultFormats();
}

PluginManager::~PluginManager()
{
	pluginInstances.clear();
}

PluginManager::KnownPluginScanner* PluginManager::getPluginScanner()
{
	return &knownPluginScanner;
}

AudioPluginInstance* PluginManager::getNewPluginInstance(int n, double sampleRate, double blockSize)
{
	return getNewPluginInstanceFromDesc(knownPluginScanner.getKnownPluginList()->getType(n), sampleRate, blockSize);
}

AudioPluginInstance* PluginManager::getNewPluginInstance(String identifierString, double sampleRate, double blockSize)
{
	return getNewPluginInstanceFromDesc(knownPluginScanner.getKnownPluginList()->getTypeForIdentifierString(identifierString), sampleRate, blockSize);
}

AudioPluginInstance* PluginManager::getNewPluginInstanceFromDesc(PluginDescription* desc, double sampleRate, double blockSize)
{
	if (desc == nullptr)
		return nullptr;
	String errorMessage;
	AudioPluginInstance* instance = formatManager.createPluginInstance(*desc, sampleRate, blockSize, errorMessage);
	pluginInstances.push_back(instance);

	return instance;
}

PluginDescription* PluginManager::getPluginDescription(int n)
{
	return knownPluginScanner.getKnownPluginList()->getType(n);
}

PluginDescription* PluginManager::getPluginDescription(String identifierString)
{
	return knownPluginScanner.getKnownPluginList()->getTypeForIdentifierString(identifierString);
}


PluginManager::PluginEditor* PluginManager::getPluginEditor(AudioProcessorGraph::Node* pluginNode)
{
	return PluginManager::PluginEditor::getPluginEditor(pluginNode);
}

std::vector<String> PluginManager::getKnownPluginNames()
{
	std::vector<String> knownNames(knownPluginScanner.getKnownPluginList()->getNumTypes());
	for (int i = 0; i < knownPluginScanner.getKnownPluginList()->getNumTypes(); i++)
		knownNames[i] = knownPluginScanner.getKnownPluginList()->getType(i)->name;
	return knownNames;
}

std::map<int, PluginManager::PluginEditor*> PluginManager::PluginEditor::nodeEditors;

PluginManager::PluginEditor* PluginManager::PluginEditor::getPluginEditor(AudioProcessorGraph::Node* pluginNode)
{
	if (PluginManager::PluginEditor::nodeEditors.find(pluginNode->nodeId) == PluginManager::PluginEditor::nodeEditors.end())
	{
		PluginManager::PluginEditor* editor = new PluginManager::PluginEditor(pluginNode->getProcessor()->getName(), pluginNode);
		PluginManager::PluginEditor::nodeEditors[pluginNode->nodeId] = editor;
		return editor;
	}
	else
		return PluginManager::PluginEditor::nodeEditors[pluginNode->nodeId];
}

void PluginManager::PluginEditor::clear()
{
	for (std::map<int, PluginManager::PluginEditor*>::iterator ii = PluginManager::PluginEditor::nodeEditors.begin(); ii != PluginManager::PluginEditor::nodeEditors.end(); ++ii)
	{
		delete (PluginEditor*) ii->second;
	}

	PluginManager::PluginEditor::nodeEditors.clear();
}

PluginManager::PluginEditor::PluginEditor(String name, AudioProcessorGraph::Node* _pluginNode)
	:DocumentWindow(name, Colours::lightblue, DocumentWindow::minimiseButton | DocumentWindow::closeButton)
{
	pluginNode = _pluginNode;
	editor = pluginNode->getProcessor()->createEditorIfNeeded();
	
	this->setContentNonOwned(editor, true);

	this->setResizable(true, false);
	//this->setVisible(true);
}

void PluginManager::PluginEditor::closeButtonPressed()
{
	this->setVisible(false);
}

PluginManager::PluginEditor::~PluginEditor()
{
}

/////////////////

PluginManager::KnownPluginScanner::KnownPluginScanner(AudioPluginFormatManager* _formatManager)
{
	formatManager = _formatManager;

	ScopedPointer<XmlElement> savedPluginList = Application::getAppProperties().getUserSettings()->getXmlValue("pluginList");
	
	if (savedPluginList != nullptr)
		knownPluginList.recreateFromXml(*savedPluginList);

	savedPluginList = nullptr;

	if (loadSavedPluginDirectories())
		rescan();
}

PluginManager::KnownPluginScanner::~KnownPluginScanner()
{
	ScopedPointer<XmlElement> savedPluginDirectories(createXmlPluginDirectories());
	if(savedPluginDirectories != nullptr)
	Application::getAppProperties().getUserSettings()->setValue("pluginDirectories", savedPluginDirectories);

	ScopedPointer<XmlElement> savedPluginList(knownPluginList.createXml());
	if(savedPluginList != nullptr)
		Application::getAppProperties().getUserSettings()->setValue("pluginList", savedPluginList);

	pluginFolders.clear();
	knownPluginList.clear();
}

KnownPluginList* PluginManager::KnownPluginScanner::getKnownPluginList()
{
	return &knownPluginList;
}

std::map<String, bool> PluginManager::KnownPluginScanner::getPluginFolders()
{
	return pluginFolders;
}

XmlElement* PluginManager::KnownPluginScanner::createXmlPluginDirectories() const
{
	XmlElement* const e = new XmlElement("PLUGINDIRECTORIES");

	for (auto it = pluginFolders.begin(); it != pluginFolders.end(); ++it)
		e->prependChildElement(createXmlPluginDirectory(it->first, it->second));
	return e;
}

XmlElement* PluginManager::KnownPluginScanner::createXmlPluginDirectory(String name, bool rec) const
{
	XmlElement* const e = new XmlElement("PLUGINDIRECTORY");
	e->setAttribute("DIR", name);
	e->setAttribute("RECURSIVE", rec ? "true" : false);

	return e;
}

bool PluginManager::KnownPluginScanner::loadSavedPluginDirectories()
{
	ScopedPointer<XmlElement> e = Application::getAppProperties().getUserSettings()->getXmlValue("PluginDirectories");

	if (e != nullptr)
	{
		XmlElement* child = e->getFirstChildElement();
		int n = 0;
		while (child != nullptr)
		{
			pluginFolders[child->getStringAttribute("DIR")] = child->getStringAttribute("RECURSIVE") == "true";
			child = e->getNextElement();
			n++;
		}
		e->deleteAllChildElements();
		
		return n > 0;
	}
	return false;
}

bool dirExists(String dirName_in)
{
	return true;
}

void PluginManager::KnownPluginScanner::addPluginFolder(String folder, bool searchSubFolders)
{
		pluginFolders[folder] = searchSubFolders;
}

void PluginManager::KnownPluginScanner::removePluginFolder(String folder)
{
	if(pluginFolders.find(folder) != pluginFolders.end())
		pluginFolders.erase(folder);
}

void PluginManager::KnownPluginScanner::rescan()
{
	for (auto it = pluginFolders.begin(); it != pluginFolders.end(); ++it)
		for (int nFormat = 0; nFormat < formatManager->getNumFormats(); nFormat++)
		{
			ScanFolder(it->first, *(formatManager->getFormat(nFormat)), it->second, true);
		}
}

void PluginManager::KnownPluginScanner::ScanFolder(String folder, AudioPluginFormat &format, bool searchSubFolders, bool dontRescanIfAlreadyInList)
{
	FileSearchPath &path = FileSearchPath(folder);

	ScopedPointer<PluginDirectoryScanner> scanner = new PluginDirectoryScanner(knownPluginList, format, path, searchSubFolders, Application::getDeadMansPedalFile());
	String name;

	while (scanner->scanNextFile(dontRescanIfAlreadyInList, name)) {};
}
