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


//-----------------------------------------------------
// INTERFACE CLASS FOR GAME STATES
//-----------------------------------------------------
class CStateControl
{
private:
	IGameState*  mpCurGameState;		// Polymorphic pointer to the current game state
	CPlayState*  mpPlayState;			// Handles creation of players
	CMenuState*  mpMenuState;			// Data for the main menu state
	CSpaceState* mpSpaceState;			// Data for the space state
	CWorldState* mpWorldState;			// Data for the building on world state (includes both Earth and Mars loaded)

	EGameStates mCurState;				// A match to the current global state in Common Header. Used to identify changes in states, and what state to change to.


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CStateControl(EGameStates inStartState);
	virtual ~CStateControl();


	// MUTATORS
	//---------------------------
	void SetCurrentState(EGameStates inNewState);


	// ACCESSORS
	//---------------------------
	inline IGameState* GetCurrentState() 
	{
		return mpCurGameState;
	}


	// METHODS
	//---------------------------
	void Update();

private:
	void OnStateChange();
};


#endif /* _GAME_STATE_CONTROL_H_ */