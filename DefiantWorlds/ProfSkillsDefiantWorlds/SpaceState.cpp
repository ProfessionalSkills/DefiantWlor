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

	//fleets attack each other according to tactics
	mpPlayerOneFleet->Fight();
	mpPlayerTwoFleet->Fight();

	//finds and removes dead ships
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
	mpPlayerOneFleet = nullptr;
	mpPlayerTwoFleet = nullptr;

	mpPlayerOneFleet->SetEnemy(nullptr);
	mpPlayerTwoFleet->SetEnemy(nullptr);
}
