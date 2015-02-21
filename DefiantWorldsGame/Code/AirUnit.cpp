//-----------------------------------------------------
// FILE: AirUnit.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "AirUnit.h"


//-----------------------------------------------------
// AIR UNIT CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CAirUnit::CAirUnit()
{

}

CAirUnit::~CAirUnit()
{

}


//-----------------------------------------------------
// AIR UNIT CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CAirUnit::Attack(CGameAgent* target, float hitMod, float damageMod)
{
	return false;
}

void CAirUnit::Spawn(CGrid* pGrid, SPointData pCentre)
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
					mpObjModel = mpObjMesh->CreateModel(spawnTiles[i]->GetWorldPos().x, 30.0f, spawnTiles[i]->GetWorldPos().z);
					mpObjModel->Scale(2.0f);
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
}
//CAirUnit::void MoveTo(CTile* dest)


bool CAirUnit::Move()
{
	return false;
}

bool CAirUnit::Destroy()
{
	return false;
}

void CAirUnit::UnloadIModel()
{

}
