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
	mScale = 0.8f;
	mOrientation = 90.0f;
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
void CMinerals::CreateResource(DX::XMFLOAT3 pos)
{
	mpObjModel = mspMshMineral->CreateModel(pos.x, pos.y, pos.z);
	mWorldPos = pos;
	mpObjModel->RotateX(mOrientation);
	mpObjModel->Scale(mScale);
}

void CMinerals::LoadIModel()
{
	if (mpObjModel == nullptr)
	{
		// Create the model
		mpObjModel = mspMshMineral->CreateModel(mWorldPos.x, mWorldPos.y, mWorldPos.z);
		mpObjModel->Scale(mScale);
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
