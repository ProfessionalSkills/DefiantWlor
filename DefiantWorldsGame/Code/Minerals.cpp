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
