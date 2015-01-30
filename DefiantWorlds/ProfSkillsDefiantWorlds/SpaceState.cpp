//-----------------------------------------------------
// FILE: SpaceState.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "SpaceState.h"


//-----------------------------------------------------
// SPACE STATE CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CSpaceState::CSpaceState() : IGameState()
{

}

CSpaceState::~CSpaceState()
{

}


//-----------------------------------------------------
// MENU STATE CLASS OVERRIDE METHODS
//-----------------------------------------------------
void CSpaceState::StateSetup()
{

}

void CSpaceState::StateUpdate(const float inDelta)
{
	// Draw the scene
	gpEngine->DrawScene();
}

void CSpaceState::StateLoad()
{

}

void CSpaceState::StateSave()
{

}

void CSpaceState::StateCleanup()
{

}
