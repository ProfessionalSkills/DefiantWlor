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
}

CTile::~CTile()
{
	mspMshTile->RemoveModel(mpMdlTile);
}


//-----------------------------------------------------
// TILE CLASS METHODS
//-----------------------------------------------------
void CTile::CreateTestModel()
{
	mpMdlTile = CTile::mspMshTile->CreateModel(mTileWorldPos.x, mTileWorldPos.y, mTileWorldPos.z);
}
