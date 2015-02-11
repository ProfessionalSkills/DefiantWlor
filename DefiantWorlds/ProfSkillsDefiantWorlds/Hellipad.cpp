//-----------------------------------------------------
// FILE: Hellipad.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Hellipad.h"

IMesh* CHellipad::mspMshStructureBuilt = nullptr;
IMesh* CHellipad::mspMshStructurePlacing = nullptr;


//-----------------------------------------------------
// HELLIPAD CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CHellipad::CHellipad()
{
	// Define default variables for this type of structure
	mScale = 0.6f;
	mOrientation = 0.0f;

	mHealth = 1000.0f;
	mBuildTime = 0.0f;
	mRepairSpeed = 1.0f;
	mCurBuildTimeLeft = 0.0f;
	mBuildCost = 0;

	mState = OBJ_CONSTRUCTING;

	mStructureBL = SPointData(-2, -2);
	mStructureTR = SPointData(2, 2);

	// Create the model
	mpObjModel = mspMshStructurePlacing->CreateModel();
	mpObjModel->Scale(mScale);
}

CHellipad::~CHellipad()
{

}


//-----------------------------------------------------
// HELLIPAD CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CHellipad::Destroy()
{
	return false;
}

void CHellipad::UnloadIModel()
{
	mspMshStructurePlacing->RemoveModel(mpObjModel);
}
