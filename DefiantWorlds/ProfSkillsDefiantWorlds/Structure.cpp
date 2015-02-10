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
void CStructure::CreateStructure(DX::XMFLOAT3 pos)
{

}

bool CStructure::TestStructureArea(CGrid* pGrid, CTile* pTile)
{
	// pTile refers to the bottom left-most tile
	// Check area based on structure size
	SPointData gridPos = pTile->GetGridPos();
	CTile* pNextTile;

	// Loop through structure size
	for (int x = gridPos.mPosX + mStructureBL.mPosX; x < gridPos.mPosX + mStructureTR.mPosX; x++)
	{
		for (int y = gridPos.mPosY + mStructureBL.mPosY; y < gridPos.mPosY + mStructureTR.mPosY; y++)
		{
			// Get current tile data
			pNextTile = pGrid->GetTileData(SPointData(gridPos.mPosX + x, gridPos.mPosY + y));

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

bool CStructure::Build(float delta)
{
	// Update build timer
	// Simply scale y (proportionally to building height)
	// Reset scale then set new scale proportional to how far through building time it is
	return false;
}
