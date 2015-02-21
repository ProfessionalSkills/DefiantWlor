//-----------------------------------------------------
// FILE: Tank.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Tank.h"

IMesh* CTank::mpObjMesh = nullptr;

//-----------------------------------------------------
// TANK CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CTank::CTank()
{
	mAgentInfo = SAgentData(GAV_TANK, "Tank");

	mHealth = 100.0f;
	mSpeed = 1.0f;
	mProductionTime = 2.0f;
	mProductionCost = 0.0f;
	mCurProductionTimeLeft = mProductionTime;
	mDamage = 1.0f;
	//mAttackParticleFX;
	//mDestroyParticleFX;
	mState = OBJ_CONSTRUCTING;
	//mDestGridSq;
	mIsMoving = false;
	mPopCost = 1;
}

CTank::~CTank()
{

}


//-----------------------------------------------------
// TANK CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CTank::Attack(CGameAgent* target, float hitMod, float damageMod)
{
	return false;
}

void CTank::Spawn(CGrid* pGrid, SPointData pCentre)
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
					mpObjModel = mpObjMesh->CreateModel(spawnTiles[i]->GetWorldPos().x, 1.0f, spawnTiles[i]->GetWorldPos().z);
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

	//for (int i = 0; i < 4; i++)
	//{
	//	delete spawnTiles[i];
	//}

}
//CTank::void MoveTo(CTile* dest)


bool CTank::Move()
{
	return false;
}

bool CTank::Destroy()
{
	return false;
}
