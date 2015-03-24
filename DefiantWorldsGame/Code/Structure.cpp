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
	mSelectSound = nullptr;
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

void CStructure::SetSelectedTexture()
{
	mpObjModel->SetSkin("bld-mt-selected.jpg");
}

void CStructure::SetDeselectedTexture()
{
	// Check if the structure is still building
	if (mState == OBJ_CONSTRUCTING)
	{
		mpObjModel->SetSkin("bld-mt-placed_tlxadd.jpg");
		return;
	}

	// Skin the model based on what faction it belongs to
	if (mFaction == FAC_EARTH_DEFENSE_FORCE)
	{
		mpObjModel->SetSkin("bld-mt.jpg");
	}
	else
	{
		mpObjModel->SetSkin("bld-mt-mars.jpg");
	}
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
	mGridSpawnLoc.mPosX += mGridPos.mPosX;
	mGridSpawnLoc.mPosY += mGridPos.mPosY;
	pNextTile = pGrid->GetTileData(mGridSpawnLoc);
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
	mBoundingBox = SBoundingCube(DX::XMFLOAT3(left, 0.0f, bottom), DX::XMFLOAT3(right, mHeight, top));
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
	// Alert news ticker if structure belongs to the player
	if (mFaction == FAC_EARTH_DEFENSE_FORCE)
	{
		switch (GetStructureType())
		{
		case STR_BARRACKS:
			mStrDisplay << "A Barracks";
			break;

		case STR_COM_CENTRE:
			mStrDisplay << "The Command Centre";
			break;

		case STR_HELLIPAD:
			mStrDisplay << "A Hellipad";
			break;

		case STR_SPACE_CENTRE:
			mStrDisplay << "A Space Centre";
			break;
		}
		mStrDisplay << " has been destroyed!";
		gpNewsTicker->AddNewElement(mStrDisplay.str(), true);
		mStrDisplay.str("");
	}
	
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
	gridPoint.mPosX = mGridSpawnLoc.mPosX;
	gridPoint.mPosY = mGridSpawnLoc.mPosY;
	pNextTile = mpGrid->GetTileData(gridPoint);
	pNextTile->SetTileUsage(false);

	// Remove the model
	UnloadIModel();
}
