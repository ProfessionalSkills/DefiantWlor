//-----------------------------------------------------
// FILE: Artillery.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Artillery.h"

IMesh* CArtillery::mspMshArtillery = nullptr;


//-----------------------------------------------------
// ARTILLERY CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CArtillery::CArtillery()
{
	mAgentInfo = SAgentData(GAV_ARTILLERY, "Artillery");
	mMaxHealth = 100.0f;
	mHealth = 100.0f;
	mSpeed = 1.0f;
	mProductionTime = 15.0f;
	mProductionCost = 0.0f;
	mCurProductionTimeLeft = mProductionTime;
	mDamage = 1.0f;
	//mAttackParticleFX;
	//mDestroyParticleFX;
	mState = OBJ_CONSTRUCTING;
	//mDestGridSq;
	mIsMoving = false;
	hasTarget = false;
	mScale = 1.5f;
	mBuildCost = 150;
	mPopCost = 4;
}

CArtillery::~CArtillery()
{
	
}

//-----------------------------------------------------
// ARTILLERY CLASS METHODS
//-----------------------------------------------------

void CArtillery::UnloadIModel()
{
	if (mpObjModel != 0)
	{
		mspMshArtillery->RemoveModel(mpObjModel);
		mpObjModel = nullptr;
	}
}

void CArtillery::LoadIModel()
{
	if (mpObjModel == nullptr)
	{
		mpObjModel = mspMshArtillery->CreateModel(mWorldPos.x, mWorldPos.y, mWorldPos.z);
		mpObjModel->Scale(mScale);
		mpObjModel->RotateY(180.0f);
		if (mFaction == FAC_EARTH_DEFENSE_FORCE)
		{
			mpObjModel->SetSkin("marsAA.jpg");
		}
		else
		{
			mpObjModel->SetSkin("marsAAMars.jpg");
		}
	}
}


//-----------------------------------------------------
// ARTILLERY CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CArtillery::Attack(CGameAgent* target, float hitMod, float damageMod)
{
	return false;
}


//CArtillery::void MoveTo(CTile* dest)


bool CArtillery::Move()
{
	//mpObjModel->SetPosition(mPathTarget->GetWorldPos().x, 0, mPathTarget->GetWorldPos().z);
	return false;
}

bool CArtillery::Destroy()
{
	UnloadIModel();
	return false;
}

IModel* CArtillery::CreateModel(DX::XMFLOAT3 pos)
{
	// Load model and set required skin
	IModel* pModel = mspMshArtillery->CreateModel(pos.x, pos.y, pos.z);
	if (mFaction == FAC_EARTH_DEFENSE_FORCE)
	{
		pModel->SetSkin("marsAA.jpg");
	}
	else
	{
		pModel->SetSkin("marsAAMars.jpg");
	}

	return pModel;
}
