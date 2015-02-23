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
	mStructureType = STR_COM_CENTRE;

	mStructureBL = SPointData(-3, -3);
	mStructureTR = SPointData(1, 3);
	mGridSpawnLoc = SPointData(-4, -2);

	// Create the model
	mpObjModel = mspMshStructurePlacing->CreateModel();
	mpObjModel->Scale(mScale);

	// Define list of responsible agents
	mRespectiveAgentsList.push_back(new CWorker());
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

	// If the x is beyond a certain distance, the object is on mars - set relevant skin
	if (mpObjModel->GetX() > (GRID_SIZE_X * GRID_TILE_SIZE))
	{
		mpObjModel->SetSkin("bld-mt-mars.jpg");
	}
}

void CComCentre::UnloadIModel()
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