//-----------------------------------------------------
// FILE: Fighter.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Fighter.h"

IMesh* CFighter::mspMshFighter = nullptr;
IMesh* CFighter::mspMshFighterBullet = nullptr;

//-----------------------------------------------------
// FIGHTER CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CFighter::CFighter()
{
	mAgentInfo = SAgentData(GAV_FIGHTER, "Fighter");
	mMaxHealth = 100.0f;
	mHealth = 100.0f;
	mSpeed = 1.0f;
	mProductionTime = 15.0f;
	mProductionCost = 0.0f;
	mCurProductionTimeLeft = mProductionTime;
	mDamage = 100.0f;
	mFireRate = 4.0f;
	mAttackTimer = 1.0f / mFireRate;
	//mAttackParticleFX;
	//mDestroyParticleFX;
	mState = OBJ_CONSTRUCTING;
	//mDestGridSq;
	mIsMoving = false;
	mHasPathTarget = false;
	mAttackTarget = nullptr;
	mScale = 2.0f;
	mBuildCost = 600;
	mRotarSpeed = 2000.0f;
	mPopCost = 3;
}

CFighter::~CFighter()
{

}

//-----------------------------------------------------
// FIGHTER CLASS METHODS
//-----------------------------------------------------

void CFighter::UnloadIModel()
{
	if (mpObjModel != 0)
	{
		mspMshFighter->RemoveModel(mpObjModel);
		mpObjModel = nullptr;
	}
}

void CFighter::LoadIModel()
{
	if (mpObjModel == nullptr)
	{
		mpObjModel = mspMshFighter->CreateModel(mWorldPos.x, mWorldPos.y, mWorldPos.z);
		mpObjModel->Scale(mScale);
		if (mFaction == FAC_EARTH_DEFENSE_FORCE)
		{
			mpObjModel->SetSkin("machine.jpg");
		}
		else
		{
			mpObjModel->SetSkin("machineMars.jpg");
		}
	}
}


//-----------------------------------------------------
// FIGHTER CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CFighter::Attack(CGameObject* target, float hitMod, float damageMod)
{
	if (mAttackTimer >= (1.0f / mFireRate))
	{
		mAttackTimer = 0.0f;
		SProjectile* newProjectile = new SProjectile();
		newProjectile->mModel = mspMshFighterBullet->CreateModel(mWorldPos.x, mWorldPos.y, mWorldPos.z);
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

bool CFighter::Destroy()
{
	UnloadIModel();
	return false;
}

IModel* CFighter::CreateModel(DX::XMFLOAT3 pos)
{
	// Load model and set required skin
	IModel* pModel = mspMshFighter->CreateModel(pos.x, pos.y, pos.z);
	if (mFaction == FAC_EARTH_DEFENSE_FORCE)
	{
		pModel->SetSkin("machine.jpg");
	}
	else
	{
		pModel->SetSkin("machineMars.jpg");
	}

	return pModel;
}

bool CFighter::Update()
{
	mpObjModel->GetNode(4)->RotateY(mRotarSpeed * gFrameTime);

	return CAirUnit::Update();
}
