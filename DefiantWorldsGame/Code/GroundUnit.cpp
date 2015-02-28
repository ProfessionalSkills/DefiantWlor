//-----------------------------------------------------
// FILE: GroundUnit.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "GroundUnit.h"

//-----------------------------------------------------
// GROUND UNIT CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CGroundUnit::CGroundUnit()
{
	mScale = 2.0f;

}

CGroundUnit::~CGroundUnit()
{	

}


//-----------------------------------------------------
// GROUND UNIT CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CGroundUnit::Attack(CGameAgent* target, float hitMod, float damageMod)
{
	return false;
}

//CGroundUnit::void MoveTo(CTile* dest)

void CGroundUnit::Spawn(CGrid* pGrid, SPointData pCentre)
{
	CTile* spawnTiles[4];
	//Tiles around the central tile for positioning
	spawnTiles[0] = pGrid->GetTileBelow(pGrid->GetTileData(pCentre));
	spawnTiles[1] = pGrid->GetTileAbove(pGrid->GetTileData(pCentre));
	spawnTiles[2] = pGrid->GetTileToLeft(pGrid->GetTileData(pCentre));
	spawnTiles[3] = pGrid->GetTileToRight(pGrid->GetTileData(pCentre));

	bool freeTileFound = false;
	while (!freeTileFound)
	{
		for (int i = 0; i < 4; i++) //For each possible spawn tile
		{
			if (!freeTileFound)
			{
				if (spawnTiles[i]->IsTileUsed() == false) //Check for useage 
				{
					//Sets the tile usage so vehicles cannot overlap and spawns the vehicle in
					spawnTiles[i]->SetTileUsage(true);
					freeTileFound = true;
					mpObjModel = CreateModel(DX::XMFLOAT3(spawnTiles[i]->GetWorldPos().x, 1.0f, spawnTiles[i]->GetWorldPos().z));
					mWorldPos.x = spawnTiles[i]->GetWorldPos().x;
					mWorldPos.y = 1.0f;
					mWorldPos.z = spawnTiles[i]->GetWorldPos().z;
					mpObjModel->Scale(mScale);

					DX::XMFLOAT3 centre = { mpObjModel->GetX(), mpObjModel->GetY(), mpObjModel->GetZ() };
					float radius = 5.0f;

					mBoundingSphere = SBoundingSphere(centre, radius);
				}
			}
		}
		if (!freeTileFound) //If none of the tiles are suitable, expand tile search
		{
			spawnTiles[0] = pGrid->GetTileBelow(pGrid->GetTileData(spawnTiles[0]->GetGridPos()));
			spawnTiles[1] = pGrid->GetTileAbove(pGrid->GetTileData(spawnTiles[1]->GetGridPos()));
			spawnTiles[2] = pGrid->GetTileToLeft(pGrid->GetTileData(spawnTiles[2]->GetGridPos()));
			spawnTiles[3] = pGrid->GetTileToRight(pGrid->GetTileData(spawnTiles[3]->GetGridPos()));
		}
	}

	// HIGGY
	// Don't remove the tiles! There is only one copy of them. The function GetTile<x>() does not allocate new memory - it just returns a pointer to the tile you want data for :)

	//for (int i = 0; i < 4; i++)
	//{
	//	delete spawnTiles[i];
	//}
}


bool CGroundUnit::Move()
{
	return false;
}

bool CGroundUnit::Destroy()
{
	return false;
}
