//-----------------------------------------------------
// FILE: GameStateControl.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "GameStateControl.h"

CStateControl* CStateControl::mspSingleton = nullptr;


//-----------------------------------------------------
// GAME STATE CONTROL CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CStateControl::CStateControl(EGameStates inStartState)
{
	// Initialise states
	//mpPlayState = new CPlayState();
	mpMenuState = new CMenuState();
	mpSpaceState = new CSpaceState();
	mpWorldState = new CWorldState();
	mpNewGameState = new CNewGameState();
	mpSettingsState = new CSettingsScreenState();

	// Initialise player manager
	mpPlayerManager = new CPlayerManager();

	// Initialise settings manager
	mpSettingsManager = new CSettingsManager();

	// Set global state
	gCurState = inStartState;
	mCurState = inStartState;

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
		case GS_NEW_GAME:
			mpCurGameState = mpNewGameState;
			break;
		case GS_SETTINGS:
			mpCurGameState = mpSettingsState;
			break;
	}

	// Setup scene
	mpCurGameState->StateSetup();
}

CStateControl::~CStateControl()
{
	SafeDelete(mpPlayerManager);

	SafeDelete(mpMenuState);
	SafeDelete(mpSpaceState);
	SafeDelete(mpWorldState);
	SafeDelete(mpSettingsState);
}


//-----------------------------------------------------
// GAME STATE CONTROL CLASS MUTATORS
//-----------------------------------------------------
void CStateControl::SetCurrentState(EGameStates inNewState)
{
	// Before changing pointer, unload current level
	mpCurGameState->StateCleanup();
	
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
	case GS_NEW_GAME:
		mpCurGameState = mpNewGameState;

		break;
	case GS_SETTINGS:
		mpCurGameState = mpSettingsState;

		break;
	}

	OnStateChange();
}


//-----------------------------------------------------
// GAME STATE CONTROL CLASS METHODS
//-----------------------------------------------------
void CStateControl::Update()
{
	// Compare current state to global state to check for state changes
	if (gCurState != mCurState)
	{
		// Update class' current stored state
		mCurState = gCurState;

		// Update polymorphic pointer
		SetCurrentState(gCurState);
	}
}

void CStateControl::OnStateChange()
{
	// Call the setup function of the new state
	mpCurGameState->StateSetup();
}
