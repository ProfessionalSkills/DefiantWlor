//-----------------------------------------------------
// FILE: Minerals.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Minerals.h"

IMesh* CMinerals::mspMshMineral = nullptr;

//-----------------------------------------------------
// MINERALS CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CMinerals::CMinerals()
{
	mScale = 1.5f;
	mOrientation = 0.0f;
	mBuildCost = 0.0f;
	mPopCost = 0.0f;
	mpObjModel = nullptr;

	mStructureBL = SPointData(0, 0);
	mStructureTR = SPointData(0, 0);
}

CMinerals::~CMinerals()
{

}


//-----------------------------------------------------
// MINERALS CLASS METHODS
//-----------------------------------------------------
void CMinerals::CreateResource(CGrid* pGrid, SPointData gridPos)
{
	CTile* pTile = pGrid->GetTileData(gridPos);
	mGridPos = gridPos;
	mWorldPos = pTile->GetWorldPos();
	mpObjModel = mspMshMineral->CreateModel(mWorldPos.x, mWorldPos.y, mWorldPos.z);
	mpObjModel->RotateX(mOrientation);
	mpObjModel->Scale(mScale);
	mpObjModel->ScaleY(3.0f);

	CalculateBoundingBox();
}

bool CMinerals::RayCollision(DX::XMFLOAT3 origin, DX::XMFLOAT3 direction, float& distance)
{
	return mBoundingBox.GetCollisionDistance(DX::XMLoadFloat3(&origin),
		DX::XMLoadFloat3(&direction), distance);
}

void CMinerals::CalculateBoundingBox()
{
	// Calculate bounding box
	float top = mWorldPos.z + ((float)mStructureTR.mPosY * GRID_TILE_SIZE) + (GRID_TILE_SIZE / 2.0f);
	float bottom = mWorldPos.z + ((float)mStructureBL.mPosY * GRID_TILE_SIZE) - (GRID_TILE_SIZE / 2.0f);
	float right = mWorldPos.x + ((float)mStructureTR.mPosX * GRID_TILE_SIZE) + (GRID_TILE_SIZE / 2.0f);
	float left = mWorldPos.x + ((float)mStructureBL.mPosX * GRID_TILE_SIZE) - (GRID_TILE_SIZE / 2.0f);
	mBoundingBox = SBoundingCube(DX::XMFLOAT3(left, 0.0f, bottom), DX::XMFLOAT3(right, mHeight, top));
}

void CMinerals::LoadIModel()
{
	if (mpObjModel == nullptr)
	{
		// Create the model
		mpObjModel = mspMshMineral->CreateModel(mWorldPos.x, mWorldPos.y, mWorldPos.z);
		mpObjModel->Scale(mScale);
		mpObjModel->ScaleY(3.0f);
		mpObjModel->RotateX(mOrientation);
	}
}

void CMinerals::UnloadIModel()
{
	if (mpObjModel != nullptr)
	{
		mspMshMineral->RemoveModel(mpObjModel);
		mpObjModel = nullptr;
	}
}
