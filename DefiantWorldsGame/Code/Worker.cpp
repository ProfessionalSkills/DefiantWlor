//-----------------------------------------------------
// FILE: Worker.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Worker.h"

IMesh* CWorker::mspMshWorker = nullptr;
IMesh* CWorker::mspMshWorkerBullet = nullptr;


//-----------------------------------------------------
// WORKER CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CWorker::CWorker()
{
	mAgentInfo = SAgentData(GAV_WORKER, "Worker");
	mMaxHealth = 100.0f;
	mHealth = 100.0f;
	mSpeed = 1.0f;
	mProductionTime = 5.0f;
	mProductionCost = 0.0f;
	mCurProductionTimeLeft = mProductionTime;
	mDamage = 1.0f;
	mFireRate = 1.0f;
	mAttackTimer = 1.0f / mFireRate;
	mHasPathTarget = false;
	mAttackTarget = nullptr;
	//mAttackParticleFX;
	//mDestroyParticleFX;
	mState = OBJ_CONSTRUCTING;
	//mDestGridSq;
	mIsMoving = false;
	mScale = 1.5f;
	mBuildCost = 50;
	mPopCost = 5;
}

CWorker::~CWorker()
{

}


//-----------------------------------------------------
// WORKER CLASS METHODS
//-----------------------------------------------------
//bool RepairBuilding(CStructure* structure)

//bool CollectResources(CResources* resource)

bool RepairUnit(CGroundUnit* unit)
{
	return false;
}

void CWorker::UnloadIModel()
{
	if (mpObjModel != 0)
	{
		mspMshWorker->RemoveModel(mpObjModel);
		mpObjModel = nullptr;
	}
}

void CWorker::LoadIModel()
{
	if (mpObjModel == nullptr)
	{
		mpObjModel = mspMshWorker->CreateModel(mWorldPos.x, mWorldPos.y, mWorldPos.z);
		mpObjModel->Scale(mScale);
		if (mFaction == FAC_EARTH_DEFENSE_FORCE)
		{
			mpObjModel->SetSkin("ttruckGerman.jpg");
		}
		else
		{
			mpObjModel->SetSkin("ttruckGermanMars.jpg");
		}
	}
}


//-----------------------------------------------------
// WORKER CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CWorker::Attack(CGameObject* target, float hitMod, float damageMod)
{
	if (mAttackTimer >= (1.0f / mFireRate))
	{
		mAttackTimer = 0.0f;
		SProjectile* newProjectile = new SProjectile();
		newProjectile->mModel = mspMshWorkerBullet->CreateModel(mWorldPos.x, mWorldPos.y, mWorldPos.z);
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
bool CWorker::Destroy()
{
	UnloadIModel();
	return false;
}

IModel* CWorker::CreateModel(DX::XMFLOAT3 pos)
{
	// Load model and set required skin
	IModel* pModel = mspMshWorker->CreateModel(pos.x, pos.y, pos.z);
	if (mFaction == FAC_EARTH_DEFENSE_FORCE)
	{
		pModel->SetSkin("ttruckGerman.jpg");
	}
	else
	{
		pModel->SetSkin("ttruckGermanMars.jpg");
	}

	return pModel;
}
