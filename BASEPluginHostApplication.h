/*
  ==============================================================================

    BASEPluginHostApplication.h
    Created: 12 Oct 2015 1:46:43pm
    Author:  weste

  ==============================================================================
*/

#ifndef BASEPLUGINHOSTAPPLICATION_H_INCLUDED
#define BASEPLUGINHOSTAPPLICATION_H_INCLUDED

#include "../../JuceLibraryCode/JuceHeader.h"

class BASEPluginHostApplication : public JUCEApplication
{
public:
	//==============================================================================
	BASEPluginHostApplication(String _appName);

	const String getApplicationName() override { return ProjectInfo::projectName; }
	const String getApplicationVersion() override { return ProjectInfo::versionString; }
	bool moreThanOneInstanceAllowed() override { return true; }

	//==============================================================================
	void initialise(const String& commandLine) override;
	
	virtual void init();
	virtual void close();

	void shutdown() override;
	//==============================================================================
	void systemRequestedQuit() override;

	void anotherInstanceStarted(const String& commandLine) override;

	ApplicationProperties* appProperties;

	String getCommandArgs();

	//==============================================================================
private:
	String appName;
	String commandArgs;
};



#endif  // BASEPLUGINHOSTAPPLICATION_H_INCLUDED
