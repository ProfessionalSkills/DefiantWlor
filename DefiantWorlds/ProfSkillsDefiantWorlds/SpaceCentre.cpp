//-----------------------------------------------------
// FILE: SpaceCentre.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "SpaceCentre.h"

IMesh* CSpaceCentre::mspMshStructure = nullptr;


//-----------------------------------------------------
// SPACE CENTRE CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CSpaceCentre::CSpaceCentre()
{
	// Define default variables for this type of structure
	mScale = 0.6f;
	mOrientation = 0.0f;

	mHealth = 1000.0f;
	mBuildTime = 0.0f;
	mRepairSpeed = 1.0f;
	mCurBuildTimeLeft = 0.0f;
	mBuildCost = 0;

	mState = OBJ_BUILT;

	mStructureBL = SPointData(-1, -1);
	mStructureTR = SPointData(1, 1);

	// Create the model
	mpObjModel = mspMshStructure->CreateModel();
	mpObjModel->Scale(mScale);
}

CSpaceCentre::~CSpaceCentre()
{

}


//-----------------------------------------------------
// SPACE CENTRE CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CSpaceCentre::Destroy()
{
	return false;
}

void CSpaceCentre::UnloadIModel()
{
	mspMshStructure->RemoveModel(mpObjModel);
}
