//-----------------------------------------------------
// FILE: SettingsManager.cpp
//-----------------------------------------------------

#include "SettingsManager.h"


//-----------------------------------------------------
// SETTINGS MANAGER CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CSettingsManager::CSettingsManager()
{
	mMineralDepositsList[0] = 2;
	mMineralDepositsList[1] = 4;
	mMineralDepositsList[2] = 6;
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
		return 500;
		break;
	case 1:
		return 1000;
		break;
	case 2:
		return 2500;
		break;
	}
}

void CSettingsManager::SaveSettings(std::ofstream& outFile)
{
	// Save the settings
	outFile << mMusicVolume << " " << mEffectsVolume << " " << mAIDifficulty << " " << mStartingResources << std::endl;
}

void CSettingsManager::LoadSettings(std::ifstream& inFile)
{
	// Get the required settings from the file
	inFile >> mMusicVolume >> mEffectsVolume >> mAIDifficulty >> mStartingResources;
}
