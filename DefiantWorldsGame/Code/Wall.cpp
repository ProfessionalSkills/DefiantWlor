//-----------------------------------------------------
// FILE: Wall.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Wall.h"
#include "Player.h"


IMesh* CWall::mspMshStructure = nullptr;


//-----------------------------------------------------
// WALL CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CWall::CWall(bool horizontal)
{
	// Define default variables for this type of structure
	CRandomiser* pRandom = new CRandomiser();
	mScale = pRandom->GetRandomFloat(9.0f, 9.1f);

	mHealth = 1000.0f;
	mBuildTime = 5.0f;
	mRepairSpeed = 1.0f;
	mCurBuildTimeLeft = mBuildTime;
	mBuildCost = 500;

	mState = OBJ_BUILT;
	mStructureType = STR_WALL;

	mStructureBL = SPointData(-1, -1);
	mStructureTR = SPointData(1, 1);
	mGridSpawnLoc = SPointData(0, -2);

	mHeight = 30.0f;

	// Set the orientation
	if (horizontal)
	{
		mOrientation = 90.0f;
	}
	else
	{
		mOrientation = 0.0f;
	}

	SafeDelete(pRandom);
}

CWall::~CWall()
{

}


//-----------------------------------------------------
// WALL CLASS METHODS
//-----------------------------------------------------
CWall* CWall::CreateWall(DX::XMFLOAT3 position, bool horizontal, CRTSPlayer* pPlayer)
{
	// Create a new CWall model
	CWall* pWall = new CWall(horizontal);
	pWall->SetWorldPos(position);
	pWall->SetFaction(pPlayer->GetPlayerFaction());
	pWall->LoadIModel();
	return pWall; 
}

void CWall::UnloadIModel()
{
	if (mpObjModel != nullptr)
	{
		mspMshStructure->RemoveModel(mpObjModel);
		mpObjModel = nullptr;
	}
}

void CWall::LoadIModel()
{
	if (mpObjModel == nullptr)
	{
		// Create new model with original mesh
		mpObjModel = mspMshStructure->CreateModel(mWorldPos.x, mWorldPos.y, mWorldPos.z);
		mpObjModel->Scale(mScale);
		mpObjModel->RotateX(90.0f);
		mpObjModel->RotateY(mOrientation);

		// If the x is beyond a certain distance, the object is on mars - set relevant skin
		if (mFaction == FAC_THE_CRIMSON_LEGION)
		{
			mpObjModel->SetSkin("bld-mt-mars.jpg");
		}
	}
}
