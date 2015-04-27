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
CWall::CWall()
{
	// Define default variables for this type of structure
	CRandomiser* pRandom = new CRandomiser();
	mScale = pRandom->GetRandomFloat(9.0f, 9.1f);

	mIsHorizontal = false;
	mHealth = 2000.0f;
	mMaxHealth = 2000.0f;
	mBuildTime = 5.0f;
	mRepairSpeed = 1.0f;
	mCurBuildTimeLeft = mBuildTime;
	mBuildCost = 0;

	mState = OBJ_BUILT;
	mStructureType = STR_WALL;

	mHeight = 50.0f;
	mOrientation = 0.0f;

	SafeDelete(pRandom);
}

CWall::CWall(bool horizontal)
{
	// Define default variables for this type of structure
	CRandomiser* pRandom = new CRandomiser();
	mScale = pRandom->GetRandomFloat(9.0f, 9.1f);

	mIsHorizontal = horizontal;
	mHealth = 2000.0f;
	mMaxHealth = 2000.0f;
	mBuildTime = 5.0f;
	mRepairSpeed = 1.0f;
	mCurBuildTimeLeft = mBuildTime;
	mBuildCost = 0;

	mState = OBJ_BUILT;
	mStructureType = STR_WALL;

	mHeight = 50.0f;

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
void CWall::CalculateBoundingBox()
{
	// Determine if it is a horizontal wall or not
	if (mIsHorizontal)
	{
		mStructureBL = SPointData(-15, -1);
		mStructureTR = SPointData(15, 1);
		
		float top = mWorldPos.z + (0.4f * GRID_TILE_SIZE) + (GRID_TILE_SIZE / 2.0f);
		float bottom = mWorldPos.z + (-0.4f * GRID_TILE_SIZE) - (GRID_TILE_SIZE / 2.0f);
		float right = mWorldPos.x + ((float)mStructureTR.mPosX * GRID_TILE_SIZE) + (GRID_TILE_SIZE / 2.0f);
		float left = mWorldPos.x + ((float)mStructureBL.mPosX * GRID_TILE_SIZE) - (GRID_TILE_SIZE / 2.0f);
		mBoundingBox = SBoundingCube(DX::XMFLOAT3(left, 0.0f, bottom), DX::XMFLOAT3(right, mHeight, top));
	}
	else
	{
		mStructureBL = SPointData(-1, -15);
		mStructureTR = SPointData(1, 15);

		float top = mWorldPos.z + ((float)mStructureTR.mPosY * GRID_TILE_SIZE) + (GRID_TILE_SIZE / 2.0f);
		float bottom = mWorldPos.z + ((float)mStructureBL.mPosY * GRID_TILE_SIZE) - (GRID_TILE_SIZE / 2.0f);
		float right = mWorldPos.x + (0.4f * GRID_TILE_SIZE) + (GRID_TILE_SIZE / 2.0f);
		float left = mWorldPos.x + (-0.4f * GRID_TILE_SIZE) - (GRID_TILE_SIZE / 2.0f);
		mBoundingBox = SBoundingCube(DX::XMFLOAT3(left, 0.0f, bottom), DX::XMFLOAT3(right, mHeight, top));
	}
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

		CalculateBoundingBox();
	}
}

void CWall::SaveStructure(std::ofstream& outFile)
{
	// Save to the output file all the required data
	outFile << mStructureType << " " << mGridPos.mPosX << " " << mGridPos.mPosY << " " << mFaction << " " << mState
		<< " " << mWorldPos.x << " " << mWorldPos.y << " " << mWorldPos.z << " " << mHealth << " " << 0 << " ";

	outFile << mOrientation << std::endl;
}

void CWall::LoadStructure(std::ifstream& inFile, CGrid* pGrid, CRTSPlayer* pPlayer)
{
	// Ensure no model is already loaded for it
	UnloadIModel();

	// Store the required data for the structure
	int faction;
	int state;
	int pad;
	inFile >> mGridPos.mPosX >> mGridPos.mPosY >> faction >> state >> mWorldPos.x >> mWorldPos.y
		>> mWorldPos.z >> mHealth >> pad >> mOrientation;

	if (mOrientation == 90.0f)
	{
		mIsHorizontal = true;
	}
	else
	{
		mIsHorizontal = false;
	}

	// Convert required values to enums
	mFaction = static_cast<EFactions>(faction);
	mState = static_cast<EObjectStates>(state);

	// Load the 3D model
	LoadIModel();
	CalculateBoundingBox();
}
