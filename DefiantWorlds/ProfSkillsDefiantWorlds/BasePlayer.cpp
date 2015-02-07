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
bool CPlayer::PurchaseStructure(EGameStructureTypes strucType, CGrid* pGrid, CTile* pTile)
{
	// Create new structure pointer
	CStructure* pNewStructure;
	
	// Determine which structure is to be built
	switch (strucType)
	{
		case STR_HOUSE:
			pNewStructure = new CHouse();
			break;
		case STR_BARRACKS:
			pNewStructure = new CBarracks();
			break;
		case STR_COMMS_CENTRE:
			pNewStructure = new CComCentre();
			break;
		case STR_HELLIPAD:
			pNewStructure = new CHellipad();
			break;
		case STR_SPACE_CENTRE:
			pNewStructure = new CSpaceCentre();
			break;
	}

	// Check whether new building can be afforded
	if (mNumMinerals - pNewStructure->GetBuildCost() < 0)
	{
		// Not enough minerals - return relevant error (* TO DOOOOOOO *)
		// First remove previously created pointer
		SafeDelete(pNewStructure);
		return false;
	}

	// Check if building area is free
	if (!pNewStructure->TestStructureArea(pGrid, pTile))
	{
		// Something blocking construction - return relevant error (* TO DOOOOOOO *)
		// First remove previously created pointer
		SafeDelete(pNewStructure);
		return false;
	}

	// Check to ensure no vehicles in the nearby area


	// Everything fine - add to vector
	mpStructureList.push_back(pNewStructure);
	return true;
}

bool CPlayer::QueueUnit(CStructure* structure, CGameAgent* unit)
{
	return false;
}
