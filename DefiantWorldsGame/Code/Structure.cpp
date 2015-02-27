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
	mpGrid = nullptr;
}

CStructure::~CStructure()
{

}


//-----------------------------------------------------
// STRUCTURE CLASS MUTATORS
//-----------------------------------------------------
void CStructure::SetState(EObjectStates newState)
{
	// If the state is set to constructing, the building cannot be deleted
	if (mState != OBJ_CONSTRUCTING)
	{
		mState = newState;
	}
}

bool CStructure::TakeDamage(float amount)
{
	return false;
}

bool CStructure::Repair(float amount)
{
	return false;
}

void CStructure::SetBadTexture()
{
	mpObjModel->SetSkin("bld-mt-bad_tlxadd.jpg");
}

void CStructure::SetGoodTexture()
{
	mpObjModel->SetSkin("bld-mt-good_tlxadd.jpg");
}

void CStructure::SetPlacedTexture()
{
	mpObjModel->SetSkin("bld-mt-placed_tlxadd.jpg");
}


//-----------------------------------------------------
// STRUCTURE CLASS METHODS
//-----------------------------------------------------
void CStructure::CreateStructure(CGrid* pGrid)
{
	mpGrid = pGrid;
	SPointData gridPoint;

	// Mark the building's grid area as in use
	CTile* pNextTile;

	// Loop through structure size
	for (int x = mGridPos.mPosX + mStructureBL.mPosX; x <= mGridPos.mPosX + mStructureTR.mPosX; x++)
	{
		for (int y = mGridPos.mPosY + mStructureBL.mPosY; y <= mGridPos.mPosY + mStructureTR.mPosY; y++)
		{
			gridPoint.mPosX = x;
			gridPoint.mPosY = y;

			// Get current tile data
			pNextTile = pGrid->GetTileData(gridPoint);

			// Set state of tile
			pNextTile->SetTileUsage(true);
		}
	}

	// Also set spawning grid tile to used
	gridPoint.mPosX = mGridPos.mPosX + mGridSpawnLoc.mPosX;
	gridPoint.mPosY = mGridPos.mPosY + mGridSpawnLoc.mPosY;
	pNextTile = pGrid->GetTileData(gridPoint);
	pNextTile->SetTileUsage(true);

	// Store spawn world location
	mWorldSpawnLoc = pNextTile->GetWorldPos();

	// Set to placed texture
	SetPlacedTexture();

	// Calculate bounding box
	float top = mWorldPos.z + ((float)mStructureTR.mPosY * GRID_TILE_SIZE) + (GRID_TILE_SIZE / 2.0f);
	float bottom = mWorldPos.z + ((float)mStructureBL.mPosY * GRID_TILE_SIZE) - (GRID_TILE_SIZE / 2.0f);
	float right = mWorldPos.x + ((float)mStructureTR.mPosX * GRID_TILE_SIZE) + (GRID_TILE_SIZE / 2.0f);
	float left = mWorldPos.x + ((float)mStructureBL.mPosX * GRID_TILE_SIZE) - (GRID_TILE_SIZE / 2.0f);
	mBoundingBox = SAABBNew(DX::XMFLOAT3(bottom, 0.0f, left), DX::XMFLOAT3(top, mHeight, right));
}

bool CStructure::TestStructureArea(CGrid* pGrid, CTile* pTile)
{
	// pTile refers to the bottom left-most tile
	// Check area based on structure size
	mGridPos = pTile->GetGridPos();
	mWorldPos = pTile->GetWorldPos();
	CTile* pNextTile;
	SPointData gridPoint;

	// Loop through structure size
	for (int x = mGridPos.mPosX + mStructureBL.mPosX; x <= mGridPos.mPosX + mStructureTR.mPosX; x++)
	{
		for (int y = mGridPos.mPosY + mStructureBL.mPosY; y <= mGridPos.mPosY + mStructureTR.mPosY; y++)
		{
			gridPoint.mPosX = x;
			gridPoint.mPosY = y;

			// Get current tile data
			pNextTile = pGrid->GetTileData(gridPoint);

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

	// Also set spawning grid tile to unused
	gridPoint.mPosX = mGridPos.mPosX + mGridSpawnLoc.mPosX;
	gridPoint.mPosY = mGridPos.mPosY + mGridSpawnLoc.mPosY;
	pNextTile = pGrid->GetTileData(gridPoint);

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


	// Area is free of other structures/resources
	return true;
}

bool CStructure::RayCollision(DX::XMFLOAT3 origin, DX::XMFLOAT3 direction, float& distance)
{
	return mBoundingBox.GetCollisionDistance(DX::XMLoadFloat3(&origin),
		DX::XMLoadFloat3(&direction), distance);
}

void CStructure::Destroy()
{
	// Mark the building's grid area as in use
	CTile* pNextTile;
	SPointData gridPoint;

	// Loop through structure size
	for (int x = mGridPos.mPosX + mStructureBL.mPosX; x <= mGridPos.mPosX + mStructureTR.mPosX; x++)
	{
		for (int y = mGridPos.mPosY + mStructureBL.mPosY; y <= mGridPos.mPosY + mStructureTR.mPosY; y++)
		{
			gridPoint.mPosX = x;
			gridPoint.mPosY = y;

			// Get current tile data
			pNextTile = mpGrid->GetTileData(gridPoint);

			// Set state of tile
			pNextTile->SetTileUsage(false);
		}
	}

	// Also set spawning grid tile to unused
	gridPoint.mPosX = mGridPos.mPosX + mGridSpawnLoc.mPosX;
	gridPoint.mPosY = mGridPos.mPosY + mGridSpawnLoc.mPosY;
	pNextTile = mpGrid->GetTileData(gridPoint);
	pNextTile->SetTileUsage(false);

	// Remove the model
	UnloadIModel();
}
