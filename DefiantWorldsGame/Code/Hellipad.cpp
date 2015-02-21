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
	mStructureType = STR_HELLIPAD;

	mStructureBL = SPointData(-2, -2);
	mStructureTR = SPointData(2, 2);
	mGridSpawnLoc = SPointData(0, -1);

	// Create the model
	mpObjModel = mspMshStructurePlacing->CreateModel();
	mpObjModel->Scale(mScale);

	// Define list of responsible agents
	mRespectiveAgentsList.push_back(new CFighter());
	mRespectiveAgentsList.push_back(new CBomber());
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

	// If the x is beyond a certain distance, the object is on mars - set relevant skin
	if (mpObjModel->GetX() > (GRID_SIZE_X * GRID_TILE_SIZE))
	{
		mpObjModel->SetSkin("bld-mt-mars.jpg");
	}
}

void CHellipad::UnloadIModel()
{
	if (mState == OBJ_BUILT || mState == OBJ_DAMAGED || mState == OBJ_DEAD || mState == OBJ_WARNING)
	{
		mspMshStructureBuilt->RemoveModel(mpObjModel);
	}
	else
	{
		mspMshStructurePlacing->RemoveModel(mpObjModel);
	}
}
