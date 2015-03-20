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
	mpObjModel->RotateX(mOrientation);
	mpObjModel->Scale(mScale);
}
