//-----------------------------------------------------
// FILE: Grid.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Grid.h"


//-----------------------------------------------------
// GRID CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CGrid::CGrid(DX::XMFLOAT3 gridStartPos)
{
	mGridStartPos = gridStartPos;
	CTile* tmp;
	

	// Initialise array
	for (int x = 0; x < GRID_SIZE_X; x++)
	{
		for (int y = 0; y < GRID_SIZE_Y; y++)
		{
			mpGridArea[x][y] = new CTile();
			tmp = mpGridArea[x][y];
			tmp->SetGridPos(SPointData(x, y));
			DX::XMFLOAT3 tilePos;
			tilePos.x = gridStartPos.x + (GRID_TILE_SIZE * x) + (GRID_TILE_SIZE / 2.0f);
			tilePos.y = gridStartPos.y;
			tilePos.z = gridStartPos.z + (GRID_TILE_SIZE * y) + (GRID_TILE_SIZE / 2.0f);
			tmp->SetWorldPos(tilePos);
		}
	}

	// Calculate the end position of the grid
	mGridEndPos.x = (GRID_TILE_SIZE * GRID_SIZE_X) + mGridStartPos.x;
	mGridEndPos.y = mGridStartPos.y;
	mGridEndPos.z = (GRID_TILE_SIZE * GRID_SIZE_Y) + mGridStartPos.z;

	// Calculate middle position of the grid
	mGridCentrePos.x = (mGridEndPos.x - mGridStartPos.x) / 2.0f;
	mGridCentrePos.y = (mGridEndPos.y - mGridStartPos.y) / 2.0f;
	mGridCentrePos.z = (mGridEndPos.z - mGridStartPos.z) / 2.0f;
}

CGrid::~CGrid()
{
	for (int x = 0; x < GRID_SIZE_X; x++)
	{
		for (int y = 0; y < GRID_SIZE_Y; y++)
		{
			CTile* tmp = mpGridArea[x][y];
			SafeDelete(tmp);
		}
	}
}


//-----------------------------------------------------
// GRID CLASS ACCESSORS
//-----------------------------------------------------
CTile* CGrid::GetTileData(SPointData gridPos)
{
	// Ensure position is within grid boundaries
	if (gridPos.mPosX < 0 || gridPos.mPosY < 0 || gridPos.mPosX >= GRID_SIZE_X || gridPos.mPosY >= GRID_SIZE_Y)
	{
		// Out of bounds - return null
		return nullptr;
	}
	
	// Get the grid position, and return the data
	return mpGridArea[gridPos.mPosX][gridPos.mPosY];
}

CTile* CGrid::GetTileToLeft(CTile* pTile)
{
	SPointData gridPos = pTile->GetGridPos();

	// Get new grid position & ensure is within bounds
	gridPos.mPosX--;
	if (gridPos.mPosX < 0)
	{
		// Out of bounds - return null
		return nullptr;
	}

	// Within bounds - return tile at gridPos
	return mpGridArea[gridPos.mPosX][gridPos.mPosY];
}

CTile* CGrid::GetTileToRight(CTile* pTile)
{
	SPointData gridPos = pTile->GetGridPos();

	// Get new grid position & ensure is within bounds
	gridPos.mPosX++;
	if (gridPos.mPosX >= GRID_SIZE_X)
	{
		// Out of bounds - return null
		return nullptr;
	}

	// Within bounds - return tile at gridPos
	return mpGridArea[gridPos.mPosX][gridPos.mPosY];
}

CTile* CGrid::GetTileAbove(CTile* pTile)
{
	SPointData gridPos = pTile->GetGridPos();

	// Get new grid position & ensure is within bounds
	gridPos.mPosY++;
	if (gridPos.mPosY >= GRID_SIZE_Y)
	{
		// Out of bounds - return null
		return nullptr;
	}

	// Within bounds - return tile at gridPos
	return mpGridArea[gridPos.mPosX][gridPos.mPosY];
}

CTile* CGrid::GetTileBelow(CTile* pTile)
{
	SPointData gridPos = pTile->GetGridPos();

	// Get new grid position & ensure is within bounds
	gridPos.mPosY--;
	if (gridPos.mPosY < 0)
	{
		// Out of bounds - return null
		return nullptr;
	}

	// Within bounds - return tile at gridPos
	return mpGridArea[gridPos.mPosX][gridPos.mPosY];
}


//-----------------------------------------------------
// GRID CLASS METHODS
//-----------------------------------------------------
void CGrid::ResetTilesModels()
{
	// Loop through all tiles
	for (int x = 0; x < GRID_SIZE_X; x++)
	{
		for (int y = 0; y < GRID_SIZE_Y; y++)
		{
			mpGridArea[x][y]->RemoveTileModel();
		}
	}
}

bool CGrid::TurnOnTiles(CTile* gridPos, SPointData pointBL, SPointData pointTR)
{
	bool badTiles = false;
	SPointData origin = gridPos->GetGridPos();
	
	// Mark the building's grid area as in use
	CTile* pNextTile;

	ResetTilesModels();

	// Loop through structure size
	for (int x = origin.mPosX + pointBL.mPosX; x <= origin.mPosX + pointTR.mPosX; x++)
	{
		for (int y = origin.mPosY + pointBL.mPosY; y <= origin.mPosY + pointTR.mPosY; y++)
		{
			// Get current tile data
			pNextTile = GetTileData(SPointData(x, y));

			// Set state of tile
			if (pNextTile)
			{
				// Only show up tiles if they are being used
				if (pNextTile->IsTileUsed())
				{
					pNextTile->CreateTileModel();
				}

				// If a used tile has not been found yet, check if this is a red tile
				if (!badTiles)
				{
					badTiles = pNextTile->IsTileUsed();
				}
			}
			else
			{
				// Edge of map detected
				badTiles = true;
			}
		}
	}

	// Return if there are any red tiles
	return badTiles;
}
