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
CGrid::CGrid(DX::XMFLOAT3 gridStartPos, IMesh* msh)
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
			tilePos.x = gridStartPos.x + (GRID_TILE_SIZE * x);
			tilePos.y = gridStartPos.y;
			tilePos.z = gridStartPos.z + (GRID_TILE_SIZE * y);
			tmp->SetWorldPos(tilePos);
			tmp->CreateTestModel(msh);
		}
	}
}

CGrid::~CGrid()
{

}


//-----------------------------------------------------
// GRID CLASS METHODS
//-----------------------------------------------------
CTile* CGrid::GetTileData(SPointData gridPos)
{
	return mpGridArea[gridPos.mPosX][gridPos.mPosY];
}
