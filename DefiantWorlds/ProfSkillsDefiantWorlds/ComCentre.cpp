//-----------------------------------------------------
// FILE: ComCentre.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "ComCentre.h"

IMesh* CComCentre::mspMshStructureBuilt = nullptr;
IMesh* CComCentre::mspMshStructurePlacing = nullptr;


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
	mCurBuildTimeLeft = mBuildTime;
	mBuildCost = 0;

	mState = OBJ_CONSTRUCTING;

	mStructureBL = SPointData(-3, -3);
	mStructureTR = SPointData(1, 3);

	// Create the model
	mpObjModel = mspMshStructurePlacing->CreateModel();
	mpObjModel->Scale(mScale);
}

CComCentre::~CComCentre()
{

}


//-----------------------------------------------------
// COM CENTRE CLASS OVERRIDE METHODS
//-----------------------------------------------------
void CComCentre::SetBuiltModel()
{
	// Remove the current model
	mspMshStructurePlacing->RemoveModel(mpObjModel);

	// Create new model with original mesh
	mpObjModel = mspMshStructureBuilt->CreateModel(mWorldPos.x, mWorldPos.y, mWorldPos.z);
	mpObjModel->Scale(mScale);
}

bool CComCentre::Destroy()
{
	return false;
}

void CComCentre::UnloadIModel()
{
	mspMshStructurePlacing->RemoveModel(mpObjModel);
}
