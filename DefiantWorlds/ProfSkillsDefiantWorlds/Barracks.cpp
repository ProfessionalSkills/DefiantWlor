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
	mBuildTime = 0.0f;
	mRepairSpeed = 1.0f;
	mCurBuildTimeLeft = 0.0f;
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
bool CBarracks::Destroy()
{
	return false;
}

void CBarracks::UnloadIModel()
{
	mspMshStructurePlacing->RemoveModel(mpObjModel);
}
