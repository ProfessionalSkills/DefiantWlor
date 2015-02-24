//-----------------------------------------------------
// FILE: SpaceCentre.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "SpaceCentre.h"

IMesh* CSpaceCentre::mspMshStructureBuilt = nullptr;
IMesh* CSpaceCentre::mspMshStructurePlacing = nullptr;


//-----------------------------------------------------
// SPACE CENTRE CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CSpaceCentre::CSpaceCentre()
{
	// Define default variables for this type of structure
	mScale = 0.6f;
	mOrientation = 0.0f;

	mHealth = 1000.0f;
	mBuildTime = 15.0f;
	mRepairSpeed = 1.0f;
	mCurBuildTimeLeft = mBuildTime;
	mBuildCost = 0;

	mState = OBJ_CONSTRUCTING;
	mStructureType = STR_SPACE_CENTRE;

	mStructureBL = SPointData(-1, -1);
	mStructureTR = SPointData(1, 1);
	mGridSpawnLoc = SPointData(0, 0);

	// Create the model
	mpObjModel = mspMshStructurePlacing->CreateModel();
	mpObjModel->Scale(mScale);

	// Define list of responsible agents
	mRespectiveAgentsList.push_back(new CSpaceFighter());
	mRespectiveAgentsList.push_back(new CTransport());
	mRespectiveAgentsList.push_back(new CMothership());
}

CSpaceCentre::~CSpaceCentre()
{

}


//-----------------------------------------------------
// SPACE CENTRE CLASS OVERRIDE METHODS
//-----------------------------------------------------
void CSpaceCentre::SetBuiltModel()
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

void CSpaceCentre::UnloadIModel()
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

void CSpaceCentre::LoadIModel()
{
	if (mState == OBJ_BUILT || mState == OBJ_DAMAGED || mState == OBJ_DEAD || mState == OBJ_WARNING)
	{
		// Create new model with original mesh
		mpObjModel = mspMshStructureBuilt->CreateModel(mWorldPos.x, mWorldPos.y, mWorldPos.z);
		mpObjModel->Scale(mScale);

		// If the x is beyond a certain distance, the object is on mars - set relevant skin
		if (mpObjModel->GetX() > (GRID_SIZE_X * GRID_TILE_SIZE))
		{
			mpObjModel->SetSkin("bld-mt-mars.jpg");
		}
	}
	else
	{
		// Create the model
		mpObjModel = mspMshStructurePlacing->CreateModel();
		mpObjModel->Scale(mScale);
	}
}