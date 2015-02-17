//-----------------------------------------------------
// FILE: AIPlayer.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "AIPlayer.h"


//-----------------------------------------------------
// MENU STATE CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CAIPlayer::CAIPlayer() : CPlayer()
{

}

CAIPlayer::~CAIPlayer()
{

}


//-----------------------------------------------------
// AI PLAYER CLASS METHODS
//-----------------------------------------------------
CStructure* CAIPlayer::CheckStructureSelection(DX::XMFLOAT3 pos)
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


//-----------------------------------------------------
// HUMAN PLAYER CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CAIPlayer::PurchaseStructure(CStructure* pStructure, CGrid* pGrid, CTile* pTile)
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
	mpStructureList.push_back(pStructure);
	return true;
}

void CAIPlayer::Update()
{
	// Loop through all structures & update them
	for (mpiterStructures = mpStructureList.begin(); mpiterStructures != mpStructureList.end(); mpiterStructures++)
	{
		(*mpiterStructures)->Update();
	}
}
