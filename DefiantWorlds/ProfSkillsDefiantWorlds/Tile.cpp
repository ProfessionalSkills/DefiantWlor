//-----------------------------------------------------
// FILE: Tile.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Tile.h"

IMesh* CTile::mspMshTile = nullptr;


//-----------------------------------------------------
// TILE CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CTile::CTile()
{
	mTileGridPos = SPointData(0, 0);
	mTileWorldPos = DX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	mTileUsed = false;
	mpParent = nullptr;
	mpMdlTile = nullptr;
}

CTile::~CTile()
{
	mspMshTile->RemoveModel(mpMdlTile);
}


//-----------------------------------------------------
// TILE CLASS MUTATORS
//-----------------------------------------------------
void CTile::SetTileUsage(bool isUsed)
{
	mTileUsed = isUsed;

	// Apply texture based on the state of the tile
	if (mTileUsed)
	{
		// Set red texture
	}
	else
	{
		// Set base texture
	}
}


//-----------------------------------------------------
// TILE CLASS METHODS
//-----------------------------------------------------
void CTile::CreateTileModel()
{
	if (!mpMdlTile)
	{
		mpMdlTile = CTile::mspMshTile->CreateModel(mTileWorldPos.x, mTileWorldPos.y, mTileWorldPos.z);
	}
}

void CTile::RemoveTileModel()
{
	if (mpMdlTile)
	{
		mspMshTile->RemoveModel(mpMdlTile);
		mpMdlTile = nullptr;
	}
}
