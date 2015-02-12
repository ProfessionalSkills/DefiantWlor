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
	mBuildTime = 10.0f;
	mRepairSpeed = 1.0f;
	mCurBuildTimeLeft = mBuildTime;
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
void CHellipad::SetBuiltModel()
{
	// Remove the current model
	mspMshStructurePlacing->RemoveModel(mpObjModel);

	// Create new model with original mesh
	mpObjModel = mspMshStructureBuilt->CreateModel(mWorldPos.x, mWorldPos.y, mWorldPos.z);
	mpObjModel->Scale(mScale);
}

bool CHellipad::Destroy()
{
	return false;
}

void CHellipad::UnloadIModel()
{
	mspMshStructurePlacing->RemoveModel(mpObjModel);
}
