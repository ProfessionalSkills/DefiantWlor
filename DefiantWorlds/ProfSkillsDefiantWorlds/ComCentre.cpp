//-----------------------------------------------------
// FILE: ComCentre.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "ComCentre.h"


IMesh* CComCentre::mspMshStructure = nullptr;


//-----------------------------------------------------
// COM CENTRE CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CComCentre::CComCentre()
{
	// Define default variables for this type of structure
	mScale = 0.85f;
	mOrientation = 0.0f;

	mHealth = 1000.0f;
	mBuildTime = 0.0f;
	mRepairSpeed = 1.0f;
	mCurBuildTimeLeft = 0.0f;
	mBuildCost = 0;

	mState = OBJ_BUILT;

	mStructureBL = SPointData(-3, -3);
	mStructureTR = SPointData(1, 3);

	// Create the model
	mpObjModel = mspMshStructure->CreateModel();
	mpObjModel->Scale(mScale);
}

CComCentre::~CComCentre()
{

}


//-----------------------------------------------------
// COM CENTRE CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CComCentre::Destroy()
{
	return false;
}

void CComCentre::UnloadIModel()
{
	mspMshStructure->RemoveModel(mpObjModel);
}
