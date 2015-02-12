//-----------------------------------------------------
// FILE: Barracks.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Barracks.h"

IMesh* CBarracks::mspMshStructureBuilt = nullptr;
IMesh* CBarracks::mspMshStructurePlacing = nullptr;


//-----------------------------------------------------
// BARRACKS CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CBarracks::CBarracks()
{
	// Define default variables for this type of structure
	mScale = 0.6f;
	mOrientation = 0.0f;

	mHealth = 1000.0f;
	mBuildTime = 5.0f;
	mRepairSpeed = 1.0f;
	mCurBuildTimeLeft = mBuildTime;
	mBuildCost = 0;

	mState = OBJ_CONSTRUCTING;

	mStructureBL = SPointData(-1, -1);
	mStructureTR = SPointData(1, 1);

	// Create the model
	mpObjModel = mspMshStructurePlacing->CreateModel();
	mpObjModel->Scale(mScale);
}

CBarracks::~CBarracks()
{

}


//-----------------------------------------------------
// BARRACKS CLASS OVERRIDE METHODS
//-----------------------------------------------------
void CBarracks::SetBuiltModel()
{
	// Remove the current model
	mspMshStructurePlacing->RemoveModel(mpObjModel);

	// Create new model with original mesh
	mpObjModel = mspMshStructureBuilt->CreateModel(mWorldPos.x, mWorldPos.y, mWorldPos.z);
	mpObjModel->Scale(mScale);
}

bool CBarracks::Destroy()
{
	return false;
}

void CBarracks::UnloadIModel()
{
	mspMshStructurePlacing->RemoveModel(mpObjModel);
}
