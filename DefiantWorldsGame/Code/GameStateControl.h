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
	//CPlayState*  mpPlayState;			
	CMenuState*  mpMenuState;			// Data for the main menu state
	CSpaceState* mpSpaceState;			// Data for the space state
	CWorldState* mpWorldState;			// Data for the building on world state (includes both Earth and Mars loaded)

	EGameStates mCurState;				// A match to the current global state in Common Header. Used to identify changes in states, and what state to change to.


	// PLAYER MANAGER
	//---------------------------
	CPlayerManager* mpPlayerManager;


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

	CPlayerManager* GetPlayerManager()
	{
		return mpPlayerManager;
	}


	// METHODS
	//---------------------------
	void Update();

private:
	void OnStateChange();
};


#endif /* _GAME_STATE_CONTROL_H_ */