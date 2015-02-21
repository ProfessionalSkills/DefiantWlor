//-----------------------------------------------------
// FILE: BasePlayer.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "BasePlayer.h"


//-----------------------------------------------------
// BASE PLAYER CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CPlayer::CPlayer()
{
	mNumMinerals = 10000;
	mpFleet = new CFleet();
	CSpaceFighter* Temp;
	for (int i = 0; i < 10; i++)
	{
		Temp = new CSpaceFighter();
		mpSpaceUnitsList.push_back(Temp);
	}
}

CPlayer::~CPlayer()
{
	for (mpiterStructures = mpStructureList.begin(); mpiterStructures != mpStructureList.end(); mpiterStructures++)
	{
		// Remove all models
		(*mpiterStructures)->UnloadIModel();
	}
}


//-----------------------------------------------------
// BASE PLAYER CLASS MUTATORS
//-----------------------------------------------------
bool CPlayer::MineralTransaction(int amount)
{
	if (mNumMinerals + amount < 0)
	{
		// Not enough funds for transaction
		return false;
	}
	else
	{
		// Enough funds
		mNumMinerals += amount;
		return true;
	}
}


//-----------------------------------------------------
// BASE PLAYER CLASS METHODS
//-----------------------------------------------------
bool CPlayer::PurchaseStructure(CStructure* pStructure, CGrid* pGrid, CTile* pTile)
{
	// Check whether new building can be afforded
	if (mNumMinerals - pStructure->GetBuildCost() < 0)
	{
		// Not enough minerals - return relevant error (* TO DOOOOOOO *)
		return false;
	}

	// Check if building area is free
	if (!pStructure->TestStructureArea(pGrid, pTile))
	{
		// Something blocking construction - return relevant error (* TO DOOOOOOO *)
		return false;
	}

	// Check to ensure no vehicles in the nearby area


	// Everything fine - build & add to vector
	pStructure->CreateStructure(pGrid);
	pStructure->SetBuildLocation(pTile->GetGridPos());
	mpStructureList.push_back(pStructure);
	return true;
}

bool CPlayer::QueueUnit(CStructure* structure, CGameAgent* unit)
{
	return false;
}

void CPlayer::LaunchAttack()
{
	mpSpaceUnitsList=mpFleet->LaunchFleet(mpSpaceUnitsList);
	//transition into space state from hear
}


