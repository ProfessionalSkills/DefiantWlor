//-----------------------------------------------------
// FILE: GameStateControl.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "GameStateControl.h"


//-----------------------------------------------------
// GAME STATE CONTROL CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CStateControl::CStateControl(EGameStates inStartState)
{
	// Initialise states
	mpMenuState = new CMenuState();
	mpSpaceState = new CSpaceState();
	mpWorldState = new CWorldState();

	// Set current state
	switch (inStartState)
	{
		case GS_MAIN_MENU:
			mpCurGameState = mpMenuState;
			break;
		case GS_SPACE:
			mpCurGameState = mpSpaceState;
			break;
		case GS_WORLD:
			mpCurGameState = mpWorldState;
			break;
	}
}

CStateControl::~CStateControl()
{

}


//-----------------------------------------------------
// GAME STATE CONTROL CLASS MUTATORS
//-----------------------------------------------------
void CStateControl::SetCurrentState(EGameStates inNewState)
{
	// Set new current state
	switch (inNewState)
	{
	case GS_MAIN_MENU:
		mpCurGameState = mpMenuState;
		break;
	case GS_SPACE:
		mpCurGameState = mpSpaceState;
		break;
	case GS_WORLD:
		mpCurGameState = mpWorldState;
		break;
	}
}


//-----------------------------------------------------
// GAME STATE CONTROL CLASS METHODS
//-----------------------------------------------------

