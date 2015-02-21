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
	mpPlayState = new CPlayState();
	mpMenuState = new CMenuState();
	mpSpaceState = new CSpaceState();
	mpWorldState = new CWorldState();

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
	}

	// Ensure other play states have links to the same players
	mpSpaceState->SetHumanPlayer(mpPlayState->GetHumanPlayer());
	mpSpaceState->SetAIPlayer(mpPlayState->GetAIPlayer());
	mpWorldState->SetHumanPlayer(mpPlayState->GetHumanPlayer());
	mpWorldState->SetAIPlayer(mpPlayState->GetAIPlayer());

	// Setup scene
	mpCurGameState->StateSetup();
}

CStateControl::~CStateControl()
{
	SafeDelete(mpPlayState);
	SafeDelete(mpMenuState);
	SafeDelete(mpSpaceState);
	SafeDelete(mpWorldState);
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

		// Remove players
		mpPlayState->RemovePlayers();
		break;
	case GS_SPACE:
		mpCurGameState = mpSpaceState;

		// Pass on player pointers to state
		mpSpaceState->SetHumanPlayer(mpPlayState->GetHumanPlayer());
		mpSpaceState->SetAIPlayer(mpPlayState->GetAIPlayer());
		break;
	case GS_WORLD:
		mpCurGameState = mpWorldState;

		if (!mpPlayState->ArePlayersCreated())
		{
			mpPlayState->CreatePlayers();
		}

		// Pass on player pointers to state
		mpWorldState->SetHumanPlayer(mpPlayState->GetHumanPlayer());
		mpWorldState->SetAIPlayer(mpPlayState->GetAIPlayer());
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
