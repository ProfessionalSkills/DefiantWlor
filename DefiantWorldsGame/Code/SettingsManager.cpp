//-----------------------------------------------------
// FILE: SettingsManager.cpp
//-----------------------------------------------------

#include "SettingsManager.h"


//-----------------------------------------------------
// SETTINGS MANAGER CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CSettingsManager::CSettingsManager()
{

}

CSettingsManager::~CSettingsManager()
{

}


//-----------------------------------------------------
// SETTINGS MANAGER CLASS METHODS
//-----------------------------------------------------
int CSettingsManager::GetStartingResourcesAmount()
{
	// Convert the starting resources index into a starting valu
	switch (mStartingResources)
	{
	case 0:
		return 2000;
		break;
	case 1:
		return 4000;
		break;
	case 2:
		return 8000;
		break;
	}
}

void CSettingsManager::SaveSettings(std::ofstream& outFile)
{
	// Save the settings
	outFile << mMusicVolume << " " << mEffectsVolume << " " << mAIDifficulty << " " << mStartingResources << std::endl;
}
