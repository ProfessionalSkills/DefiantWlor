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
CRTSPlayer::CRTSPlayer(EFactions playerFaction)
{
	mNumMinerals = 10000;
	mpFleet = new CFleet();
	mPlayerFaction = playerFaction;
	mNumMothership = 0;
	mNumSpaceFighter = 0;
	mNumTransport = 0;
	
	mpPlayerGrid = nullptr;

	CSpaceFighter* Temp;
	for (int i = 0; i < 40; i++)
	{
		Temp = new CSpaceFighter();
		mpSpaceUnitsList.push_back(Temp);
	}
}

CRTSPlayer::~CRTSPlayer()
{
	for (miterStructuresMap = mpStructuresMap.begin(); miterStructuresMap != mpStructuresMap.end(); miterStructuresMap++)
	{
		// Remove all models
		miterStructuresMap->second->UnloadIModel();
	}

	// Handle removal of the grids
	SafeDelete(mpPlayerGrid);
}


//-----------------------------------------------------
// PLAYER CLASS MUTATORS
//-----------------------------------------------------
bool CRTSPlayer::MineralTransaction(int amount)
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

void CRTSPlayer::SetNumSpaceFighter(int numShips)
{
	mNumSpaceFighter = numShips;
}

void CRTSPlayer::SetNumTransport(int numShips)
{
	mNumTransport = numShips;
}

void CRTSPlayer::SetNumMothership(int numShips)
{
	mNumMothership = numShips;
}


//-----------------------------------------------------
// PLAYER CLASS METHODS
//-----------------------------------------------------
bool CRTSPlayer::PurchaseStructure(CStructure* pStructure, CGrid* pGrid, CTile* pTile)
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
	pStructure->SetFaction(mPlayerFaction);
	mpStructuresMap.insert(GS_MultiMap::value_type(pStructure->GetStructureType(), pStructure));
	return true;
}

bool CRTSPlayer::QueueUnit(CStructure* structure, CGameAgent* unit)
{
	return false;
}

void CRTSPlayer::LaunchAttack()
{
	mpSpaceUnitsList = (*mpFleet->LaunchFleet(&mpSpaceUnitsList));
}

void CRTSPlayer::CheckGameObjectSelection(CStructure*& pStructure, CGameAgent*& pGameAgent,
	DX::XMFLOAT3 origin, DX::XMFLOAT3 direction)
{
	float curDist = 99999.0f;
	float newDist = 0;
	
	// FIRST CHECK UNITS
	pGameAgent = nullptr;

	// Loop through all Units
	for (miterUnitsMap = mpUnitsMap.begin(); miterUnitsMap != mpUnitsMap.end(); miterUnitsMap++)
	{
		// If there is a collision, return the pointer to that object
		if (miterUnitsMap->second->RayCollision(origin, direction, newDist) && newDist < curDist)
		{
			pGameAgent = miterUnitsMap->second;
			curDist = newDist;
		}
	}


	// THEN CHECK BUILDINGS
	pStructure = nullptr;

	// Loop through all structures
	for (miterStructuresMap = mpStructuresMap.begin(); miterStructuresMap != mpStructuresMap.end(); miterStructuresMap++)
	{
		// If there is a collision, return the pointer to that object
		if (miterStructuresMap->second->RayCollision(origin, direction, newDist) && newDist < curDist)
		{
			pStructure = miterStructuresMap->second;
			curDist = newDist;
		}
	}

	// If structure is pointing at something, game agent cannot
	if (pStructure)
	{
		pGameAgent = nullptr;
	}
}

void CRTSPlayer::Update()
{
	// Loop through all structures & update them
	for (miterStructuresMap = mpStructuresMap.begin(); miterStructuresMap != mpStructuresMap.end(); miterStructuresMap++)
	{
		// Call update function for this structure
		if (!miterStructuresMap->second->Update(this))
		{
			// The current structure has been destroyed
			CStructure* tmp = miterStructuresMap->second;
			SafeDelete(tmp);
			mpStructuresMap.erase(miterStructuresMap);
			break;
		}
	}

	// Loop through all units (NOT SPACE) & update them
	for (miterUnitsMap = mpUnitsMap.begin(); miterUnitsMap != mpUnitsMap.end(); miterUnitsMap++)
	{
		miterUnitsMap->second->Update();
		//// Call update function for this structure
		//if (!miterUnitsMap->second->Update(this))
		//{
		//	// The current structure has been destroyed
		//	CGameAgent* tmp = miterUnitsMap->second;
		//	SafeDelete(tmp);
		//	mpUnitsMap.erase(miterUnitsMap);
		//	break;
		//}
	}
}

void CRTSPlayer::LoadStructureModels()
{
	for (miterStructuresMap = mpStructuresMap.begin(); miterStructuresMap != mpStructuresMap.end(); miterStructuresMap++)
	{
		miterStructuresMap->second->LoadIModel();
	}
}

void CRTSPlayer::UnloadStructureModels()
{
	// Loop through all structures & unload their models
	for (miterStructuresMap = mpStructuresMap.begin(); miterStructuresMap != mpStructuresMap.end(); miterStructuresMap++)
	{
		miterStructuresMap->second->UnloadIModel();
	}
}

void CRTSPlayer::LoadUnitModels()
{
	//loads units back into their original postion
	for (miterUnitsMap = mpUnitsMap.begin(); miterUnitsMap != mpUnitsMap.end(); miterUnitsMap++)
	{
		miterUnitsMap->second->LoadIModel();
	}
}

void CRTSPlayer::UnloadUnitModels()
{
	for (miterUnitsMap = mpUnitsMap.begin(); miterUnitsMap != mpUnitsMap.end(); miterUnitsMap++)
	{
		miterUnitsMap->second->UnloadIModel();
	}
}