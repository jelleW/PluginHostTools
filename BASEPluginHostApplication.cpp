/*
  ==============================================================================

    BASEPluginHostApplication.cpp
    Created: 12 Oct 2015 1:46:43pm
    Author:  weste

  ==============================================================================
*/

#include "BASEPluginHostApplication.h"
#include "PluginManager.h"
#include "AudioGraphManager.h"
#include "MidiManager.h"

//==============================================================================
BASEPluginHostApplication::BASEPluginHostApplication(String _appName) 
{
	appName = _appName;
}

//==============================================================================
void BASEPluginHostApplication::initialise(const String& commandLine)
{
	// This method is where you should put your application's initialisation code..

	// initialise our settings file..
	PropertiesFile::Options options;
	options.applicationName = appName;
	options.filenameSuffix = "settings";
	options.osxLibrarySubFolder = "Preferences";

	appProperties = new ApplicationProperties();
	appProperties->setStorageParameters(options);
	appProperties->saveIfNeeded();

	commandArgs = commandLine;

	this->init();
}

String BASEPluginHostApplication::getCommandArgs()
{
	return commandArgs;
}

void BASEPluginHostApplication::shutdown()
{
	this->close();

	PluginManager::clear();
	AudioGraphManager::clear();
	MidiManager::clear();

	appProperties->saveIfNeeded();
	delete appProperties;
}

void BASEPluginHostApplication::init()
{}

void BASEPluginHostApplication::close()
{}

DocumentWindow * BASEPluginHostApplication::getMainWindow()
{
	return mainWindow;
}

//==============================================================================
void BASEPluginHostApplication::systemRequestedQuit()
{
	// This is called when the app is being asked to quit: you can ignore this
	// request and let the app carry on running, or call quit() to allow the app to close.
	quit();
}

void BASEPluginHostApplication::anotherInstanceStarted(const String& commandLine)
{
	// When another instance of the app is launched while this one is running,
	// this method is invoked, and the commandLine parameter tells you what
	// the other instance's command-line arguments were.
}

