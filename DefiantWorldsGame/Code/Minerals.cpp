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
	mpObjModel->RotateX(90.0f);
	mpObjModel->Scale(mScale);
}
