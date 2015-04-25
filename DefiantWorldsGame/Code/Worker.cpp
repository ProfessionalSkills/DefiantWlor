//-----------------------------------------------------
// FILE: Worker.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Worker.h"
#include "WorldState.h"

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
	mSpeed = 12.0f;
	mProductionTime = 7.5f;
	mCurProductionTimeLeft = mProductionTime;
	mDamage = 0.0f;
	mFireRate = 0.1f;
	mAttackTimer = 1.0f / mFireRate;
	mHasPathTarget = false;
	mAttackTarget = nullptr;
	mState = OBJ_CONSTRUCTING;
	mIsMoving = false;
	mScale = 1.0f;
	mBuildCost = 150;
	mPopCost = 5;
	mTurretNode = 0;
	mObjectType = Q_WORKER;
	mRange = 10.0f;
}

CWorker::~CWorker()
{
	// Set the mineral the worker is working with to not being used
	if (mpActiveMineral)
	{
		mpActiveMineral->SetUsage(false);
	}
}


//-----------------------------------------------------
// WORKER CLASS METHODS
//-----------------------------------------------------
bool CWorker::IsHarvestingMineral()
{
	// Check if there is a mineral object attached to this worker
	if (mpActiveMineral && mpObjModel)
	{
		// Check to see if the worker is close enough to the active mineral
		float threshold = 10.0f;
		float distance = 100.0f;

		// Get the local Z axis of the worker unit
		DX::XMFLOAT4X4 objMatrix;
		mpObjModel->GetMatrix(&objMatrix.m[0][0]);
		DX::XMFLOAT3 localZ{ objMatrix.m[2][0], objMatrix.m[2][1], objMatrix.m[2][2] };

		mpActiveMineral->RayCollision(mWorldPos, localZ, distance);

		// Check the distance is less than the threshold
		if (distance <= threshold)
		{
			return true;
		}
		return false;
	}

	// No active mineral attached
	return false;
}

bool CWorker::RepairUnit(CGroundUnit* unit)
{
	return false;
}

void CWorker::UnloadIModel()
{
	if (mpObjModel != 0)
	{
		mspMshWorker->RemoveModel(mpObjModel);
		mpObjModel = nullptr;
		mHasPathTarget = false;
		mAttackTarget = nullptr;
	}

	// Delete any particle effects attached to this model
	if (mWarningSmoke) SafeDelete(mWarningSmoke);

	while (mpProjectiles.size() > 0)
	{
		SProjectile* pProjectile = mpProjectiles.back();
		SafeDelete(pProjectile);
		mpProjectiles.pop_back();
	}

	while (mpAttackExplosions.size() > 0)
	{
		CExplosion* pExplosion = mpAttackExplosions.back();
		SafeDelete(pExplosion);
		mpAttackExplosions.pop_back();
	}

	// Remove shadow
	if (mpObjShadow)
	{
		CWorldState::mspMshUnitShadow->RemoveModel(mpObjShadow);
		mpObjShadow = nullptr;
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
		else if (mFaction == FAC_THE_CRIMSON_LEGION)
		{
			mpObjModel->SetSkin("ttruckGermanMars.jpg");
		}
		else
		{
			mpObjModel->SetSkin("ttruckGermanRebel.jpg");
		}
	}

	// Create shadow
	mpObjShadow = CWorldState::mspMshUnitShadow->CreateModel(mWorldPos.x, 0.4f, mWorldPos.z);
}


//-----------------------------------------------------
// WORKER CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CWorker::Attack(CGameObject* target, float hitMod, float damageMod)
{
	mAttackTarget = nullptr;
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
	else if (mFaction == FAC_THE_CRIMSON_LEGION)
	{
		pModel->SetSkin("ttruckGermanMars.jpg");
	}
	else
	{
		pModel->SetSkin("ttruckGermanRebel.jpg");
	}

	return pModel;
}

void CWorker::SetTargetTexture()
{
	if (mpObjModel) mpObjModel->SetSkin("ttruckGermanTarget.jpg");
}

void CWorker::SetNormalTexture()
{
	if (mpObjModel)
	{
		if (mFaction == FAC_EARTH_DEFENSE_FORCE)
		{
			mpObjModel->SetSkin("ttruckGerman.jpg");
		}
		else if (mFaction == FAC_THE_CRIMSON_LEGION)
		{
			mpObjModel->SetSkin("ttruckGermanMars.jpg");
		}
		else
		{
			mpObjModel->SetSkin("ttruckGermanRebel.jpg");
		}
	}
}
