//-----------------------------------------------------
// FILE: SettingsManager.h
//-----------------------------------------------------

#ifndef _SETTINGS_MANAGER_H_
#define _SETTINGS_MANAGER_H_

#include <fstream>


class CSettingsManager
{
public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CSettingsManager();
	~CSettingsManager();


	// ACCESSORS
	//---------------------------
	inline float GetMusicVolume()
	{
		return mMusicVolume;
	}

	inline float GetEffectsVolume()
	{
		return mEffectsVolume;
	}

	inline int GetAIDifficulty()
	{
		return mAIDifficulty;
	}

	inline int GetStartingResourcesIndex()
	{
		return mStartingResources;
	}

	inline bool IsGameBeingLoaded()
	{
		return mLoadingGame;
	}

	inline void GetLoadFileName(std::string& loadFileName)
	{
		loadFileName = mLoadFileName;
	}

	int GetStartingResourcesAmount();


	// MUTATORS
	//---------------------------
	inline void SetMusicVolume(float newVolume)
	{
		mMusicVolume = newVolume;
	}

	inline void SetEffectsVolume(float newVolume)
	{
		mEffectsVolume = newVolume;
	}

	inline void SetAIDifficulty(int difficulty)
	{
		mAIDifficulty = difficulty;
	}

	void SetStartingResources(int amount)
	{
		mStartingResources = amount;
	}

	void SetIfLoadingGame(bool loading)
	{
		mLoadingGame = loading;
	}

	void SetLoadFile(std::string& loadFileName)
	{
		mLoadFileName = loadFileName;
	}


	// METHODS
	//---------------------------
	void SaveSettings(std::ofstream& outFile);
	void LoadSettings(std::ifstream& inFile);


private:
	// SETTINGS DATA
	//---------------------------
	bool mLoadingGame = false;				// Used to identify whether or not the player is loading a game from file
	std::string mLoadFileName = "";

	float mMusicVolume = 0.8f;
	float mEffectsVolume = 0.6f;

	int mAIDifficulty = 1;
	int mStartingResources = 1;
};


#endif /* _SETTINGS_MANAGER_H_ */
