#pragma once

#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED


#include "../../JuceLibraryCode/JuceHeader.h"
#include "BASEPluginHostApplication.h"

static class Application
{
public:
	static void init();

	static BASEPluginHostApplication& getApp() { return *dynamic_cast<BASEPluginHostApplication*>(JUCEApplication::getInstance()); }
	//static ApplicationCommandManager& getCommandManager() { return getApp().commandManager; }
	static ApplicationProperties& getAppProperties() { \
		ApplicationProperties* ap = getApp().appProperties;
		File appSettingsDir = ap->getUserSettings()->getFile().getParentDirectory();
		
		if (!appSettingsDir.exists())
			appSettingsDir.createDirectory();

		if (!ap->getUserSettings()->getFile().exists())
			ap->getUserSettings()->getFile().create();

		if (!ap->getUserSettings()->getFile().getSiblingFile("RecentlyCrashedPluginsList").exists())
			ap->getUserSettings()->getFile().getSiblingFile("RecentlyCrashedPluginsList").create();

		return *ap;
	}

	static File getDeadMansPedalFile() { 
		return getAppProperties().getUserSettings()->getFile().getSiblingFile("RecentlyCrashedPluginsList"); 
	}
};

#endif  // APPLICATION_H_INCLUDED
