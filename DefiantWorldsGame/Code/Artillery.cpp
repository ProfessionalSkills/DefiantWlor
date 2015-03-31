//-----------------------------------------------------
// FILE: Artillery.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Artillery.h"

IMesh* CArtillery::mspMshArtillery = nullptr;
IMesh* CArtillery::mspMshArtilleryShell = nullptr;


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
	mDamage = 20.0f;
	mFireRate = 0.333333f;
	mAttackTimer = 1.0f / mFireRate;
	//mAttackParticleFX;
	//mDestroyParticleFX;
	mState = OBJ_CONSTRUCTING;
	//mDestGridSq;
	mIsMoving = false;
	mHasPathTarget = false;
	mAttackTarget = nullptr;
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
bool CArtillery::Attack(CGameObject* target, float hitMod, float damageMod)
{
	if (mAttackTimer >= (1.0f / mFireRate))
	{
		mAttackTimer = 0.0f;
		SProjectile* newProjectile = new SProjectile();
		newProjectile->mModel = mspMshArtilleryShell->CreateModel(mWorldPos.x, mWorldPos.y, mWorldPos.z);
		newProjectile->mModel->LookAt(mAttackTarget->GetModel());
		newProjectile->mSpeed = 50.0f;

		mpProjectiles.push_back(newProjectile);
	}
	else
	{
		mAttackTimer += gFrameTime;
	}
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
