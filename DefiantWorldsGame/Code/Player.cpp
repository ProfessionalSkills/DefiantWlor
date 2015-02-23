//-----------------------------------------------------
// FILE: Player.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Player.h"


//-----------------------------------------------------
// BASE PLAYER CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CPlayer::CPlayer(EFactions playerFaction)
{
	mNumMinerals = 10000;
	mpFleet = new CFleet();
	mPlayerFaction = playerFaction;
	
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
// PLAYER CLASS MUTATORS
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
// PLAYER CLASS METHODS
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
	mpSpaceUnitsList = mpFleet->LaunchFleet(mpSpaceUnitsList);
	//transition into space state from hear
}

CStructure* CPlayer::CheckStructureSelection(DX::XMFLOAT3 pos)
{
	// Loop through all structures
	for (mpiterStructures = mpStructureList.begin(); mpiterStructures != mpStructureList.end(); mpiterStructures++)
	{
		// If there is a collision, return the pointer to that object
		if ((*mpiterStructures)->PointCollision(pos))
		{
			return (*mpiterStructures);
		}
	}

	return nullptr;
}

void CPlayer::Update()
{
	// Loop through all structures & update them
	for (mpiterStructures = mpStructureList.begin(); mpiterStructures != mpStructureList.end(); mpiterStructures++)
	{
		if (!(*mpiterStructures)->Update())
		{
			// The current structure has been destroyed
			CStructure* tmp = (*mpiterStructures);
			SafeDelete(tmp);
			mpStructureList.erase(mpiterStructures);
			break;
		}
	}
}
