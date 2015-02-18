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
CSpaceState::CSpaceState() : CPlayState()
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
	mpPlayerOneFleet = mpHumanPlayer->GetFleet();
	mpPlayerTwoFleet = mpAIPlayer->GetFleet();
	mpPlayerOneFleet->SetEnemy(mpPlayerTwoFleet);
	mpPlayerTwoFleet->SetEnemy(mpPlayerOneFleet);
}

void CSpaceState::StateUpdate()
{
	// Draw the scene
	gpEngine->DrawScene();
	mpPlayerOneFleet->Fight();
	mpPlayerTwoFleet->Fight();
	mpPlayerOneFleet->UpdateCondition();
	mpPlayerTwoFleet->UpdateCondition();
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
