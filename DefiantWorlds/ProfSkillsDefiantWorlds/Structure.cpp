//-----------------------------------------------------
// FILE: Structure.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Structure.h"


//-----------------------------------------------------
// STRUCTURE CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CStructure::CStructure()
{

}

CStructure::~CStructure()
{

}


//-----------------------------------------------------
// STRUCTURE CLASS MUTATORS
//-----------------------------------------------------
bool CStructure::TakeDamage(float amount)
{
	return false;
}

bool CStructure::Repair(float amount)
{
	return false;
}


//-----------------------------------------------------
// STRUCTURE CLASS METHODS
//-----------------------------------------------------
void CStructure::CreateStructure(CGrid* pGrid)
{
	// (*TO DOOOOOOO - call BUILD() to update construction of structure*)

	// Mark the building's grid area as in use
	CTile* pNextTile;

	// Loop through structure size
	for (int x = mGridPos.mPosX + mStructureBL.mPosX; x <= mGridPos.mPosX + mStructureTR.mPosX; x++)
	{
		for (int y = mGridPos.mPosY + mStructureBL.mPosY; y <= mGridPos.mPosY + mStructureTR.mPosY; y++)
		{
			// Get current tile data
			pNextTile = pGrid->GetTileData(SPointData(x, y));

			// Set state of tile
			pNextTile->SetTileUsage(true);
		}
	}
}

bool CStructure::TestStructureArea(CGrid* pGrid, CTile* pTile)
{
	// pTile refers to the bottom left-most tile
	// Check area based on structure size
	mGridPos = pTile->GetGridPos();
	mWorldPos = pTile->GetWorldPos();
	CTile* pNextTile;

	// Loop through structure size
	for (int x = mGridPos.mPosX + mStructureBL.mPosX; x <= mGridPos.mPosX + mStructureTR.mPosX; x++)
	{
		for (int y = mGridPos.mPosY + mStructureBL.mPosY; y <= mGridPos.mPosY + mStructureTR.mPosY; y++)
		{
			// Get current tile data
			pNextTile = pGrid->GetTileData(SPointData(x, y));

			// Check if pointer is null
			if (!pNextTile)
			{
				// This tile is not within the grid - return false
				return false;
			}

			// Check if tile is in use
			if (pNextTile->IsTileUsed())
			{
				// Tile is used - return false
				return false;
			}
		}
	}
	
	// Area is free of other structures/resources
	return true;
}

bool CStructure::Build()
{
	// Update build timer
	// Simply scale y (proportionally to building height)
	// Reset scale then set new scale proportional to how far through building time it is
	return false;
}
