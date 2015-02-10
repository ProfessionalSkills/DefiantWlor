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
	
}

CPlayer::~CPlayer()
{
	
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
	pStructure->Build();
	mpStructureList.push_back(pStructure);
	return true;
}

bool CPlayer::QueueUnit(CStructure* structure, CGameAgent* unit)
{
	return false;
}
