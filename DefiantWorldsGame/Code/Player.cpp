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
	
	mpPlayerGrid = nullptr;

	//CSpaceFighter* Temp;
	//for (int i = 0; i < 10; i++)
	//{
	//	Temp = new CSpaceFighter();
	//	mpSpaceUnitsList.push_back(Temp);
	//}
}

CRTSPlayer::~CRTSPlayer()
{
	for (mpiterStructures = mpStructureList.begin(); mpiterStructures != mpStructureList.end(); mpiterStructures++)
	{
		// Remove all models
		(*mpiterStructures)->UnloadIModel();
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
	mpStructureList.push_back(pStructure);
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

CStructure* CRTSPlayer::CheckStructureSelection(DX::XMFLOAT3 pos)
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

void CRTSPlayer::Update()
{
	// Loop through all structures & update them
	for (mpiterStructures = mpStructureList.begin(); mpiterStructures != mpStructureList.end(); mpiterStructures++)
	{
		// Call update function for this structure
		if (!(*mpiterStructures)->Update(this))
		{
			// The current structure has been destroyed
			CStructure* tmp = (*mpiterStructures);
			SafeDelete(tmp);
			mpStructureList.erase(mpiterStructures);
			break;
		}
	}

	// Loop through all units (NOT SPACE) & update them
}

void CRTSPlayer::LoadStructureModels()
{
	for (int i = 0; i < mpStructureList.size(); i++)
	{
		mpStructureList[i]->LoadIModel();
	}
}

void CRTSPlayer::UnloadStructureModels()
{
	// Loop through all structures & unload their models
	for (int i = 0; i < mpStructureList.size();i++)
	{
		mpStructureList[i]->UnloadIModel();
	}
}

void CRTSPlayer::LoadUnitModels()
{
	//loads units back into their original postion
	for (int i = 0; i < mpWorldUnitsList.size(); i++)
	{
		//mpWorldUnitsList[i]->LoadIModel();
	}
}

void CRTSPlayer::UnloadUnitModels()
{
	for (int i = 0; i < mpWorldUnitsList.size(); i++)
	{
		mpWorldUnitsList[i]->UnloadIModel();
	}
}