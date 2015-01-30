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
	CMenuState*  mpMenuState;			// Data for the main menu state
	CSpaceState* mpSpaceState;			// Data for the space state
	CWorldState* mpWorldState;			// Data for the building on world state (includes both Earth and Mars loaded)

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
	inline IGameState* GetCurrentState() const 
	{
		return mpCurGameState;
	}


	// METHODS
	//---------------------------
private:
	void OnStateChange();
};


#endif /* _GAME_STATE_CONTROL_H_ */