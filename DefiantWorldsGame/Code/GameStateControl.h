//-----------------------------------------------------
// FILE: GameStateControl.h
//-----------------------------------------------------

#ifndef _GAME_STATE_CONTROL_H_
#define _GAME_STATE_CONTROL_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "MenuState.h"
#include "SpaceState.h"
#include "WorldState.h"
#include "PlayerManager.h"
#include "SettingsManager.h"


//-----------------------------------------------------
// INTERFACE CLASS FOR GAME STATES
//-----------------------------------------------------
class CStateControl
{
private:
	// PRIVATE CONSTRUCTOR
	//---------------------------
	CStateControl(EGameStates inStartState);


	// SINGLETON
	//---------------------------
	static CStateControl* mspSingleton;


	// STATE CONTROL
	//---------------------------
	CGameState*  mpCurGameState;		// Polymorphic pointer to the current game state
	CMenuState*  mpMenuState;			// Data for the main menu state
	CSpaceState* mpSpaceState;			// Data for the space state
	CWorldState* mpWorldState;			// Data for the building on world state (includes both Earth and Mars loaded)

	EGameStates mCurState;				// A match to the current global state in Common Header. Used to identify changes in states, and what state to change to.

	float mTimeSinceGameStart = 0.0f;	// Time that has passed since the game started


	// PLAYER MANAGER
	//---------------------------
	CPlayerManager* mpPlayerManager;


	// SETTINGS MANAGER
	//---------------------------
	CSettingsManager* mpSettingsManager;


	CSound* mMusic;


public:
	// DESTRUCTOR
	//---------------------------
	~CStateControl();


	// MUTATORS
	//---------------------------
	void SetCurrentState(EGameStates inNewState);


	// ACCESSORS
	//---------------------------
	static CStateControl* GetInstance()
	{
		if (!mspSingleton)
		{
			mspSingleton = new CStateControl(GS_MAIN_MENU);
		}

		return mspSingleton;
	}

	inline CGameState* GetCurrentState() 
	{
		return mpCurGameState;
	}

	inline CPlayerManager* GetPlayerManager()
	{
		return mpPlayerManager;
	}

	inline CSettingsManager* GetSettingsManager()
	{
		return mpSettingsManager;
	}

	inline float GetTimeSinceStart()
	{
		return mTimeSinceGameStart;
	}


	// METHODS
	//---------------------------
	void Update();

private:
	void OnStateChange();
};


#endif /* _GAME_STATE_CONTROL_H_ */