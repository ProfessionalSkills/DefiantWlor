//-----------------------------------------------------
// FILE: SpaceState.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "PlayState.h"


//-----------------------------------------------------
// SPACE STATE CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CPlayState::CPlayState() : IGameState()
{
	mpHumanPlayer = nullptr;
	mpAIPlayer = nullptr;
}

CPlayState::~CPlayState()
{
	RemovePlayers();
}


//-----------------------------------------------------
// PLAY STATE CLASS VIRTUAL METHODS
//-----------------------------------------------------
void CPlayState::StateSetup()
{

}

void CPlayState::StateUpdate()
{

}
void CPlayState::StateCleanup()
{

}

void CPlayState::StateSave()
{

}

void CPlayState::StateLoad()
{

}


//-----------------------------------------------------
// PLAY STATE CLASS METHODS
//-----------------------------------------------------
void CPlayState::CreatePlayers()
{
	// Check if a pointer to the players already exist
	if (!mpHumanPlayer)
	{
		mpHumanPlayer = new CHumanPlayer();
	}
	if (!mpAIPlayer)
	{
		mpAIPlayer = new CAIPlayer();
	}
}

void CPlayState::RemovePlayers()
{
	SafeDelete(mpHumanPlayer);
	SafeDelete(mpAIPlayer);
}
