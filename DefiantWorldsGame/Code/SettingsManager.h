//-----------------------------------------------------
// FILE: SettingsManager.h
//-----------------------------------------------------

#ifndef _SETTINGS_MANAGER_H_
#define _SETTINGS_MANAGER_H_


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


private:
	// SETTINGS DATA
	//---------------------------
	float mMusicVolume = 0.8f;
	float mEffectsVolume = 0.6f;

	int mAIDifficulty = 0;
};


#endif /* _SETTINGS_MANAGER_H_ */
