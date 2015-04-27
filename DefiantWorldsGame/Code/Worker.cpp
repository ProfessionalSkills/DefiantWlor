//-----------------------------------------------------
// FILE: Worker.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Worker.h"
#include "WorldState.h"

IMesh* CWorker::mspMshWorker = nullptr;
IMesh* CWorker::mspMshWorkerLaser = nullptr;


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
	mHealCountdown = mHealingTime;
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

	// Check if a harvesting model exists
	if (mpMdlHarvest)
	{
		mspMshWorkerLaser->RemoveModel(mpMdlHarvest);
		mpMdlHarvest = nullptr;
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
		return mHarvesting;
	}

	// No active mineral attached
	return false;
}

bool CWorker::RepairUnit()
{
	if (mpHealTarget->GetHealth() == mpHealTarget->GetMaxHealth())
	{
		mHealing = false;
		return false;
	}
	//Count down to next heal on unit
	mHealCountdown -= gFrameTime;
	//When timer reaches 0, heal unit and reset timer
	if (mHealCountdown <= 0.0f)
	{
		mHealCountdown = mHealingTime;
		mpHealTarget->Heal(1.0f);
	}
	if (!mpHealingSpark)
	{
		mpHealingSpark = new CSpark(mpHealTarget->GetWorldPos());
	}
	else
	{
		mpHealingSpark->UpdateSystem();
	}
	return false;
}

void CWorker::UnloadIModel()
{
	if (mpObjModel != 0)
	{
		mspMshWorker->RemoveModel(mpObjModel);
		mpObjModel = nullptr;
		mHasPathTarget = false;
		if (mAttackTarget && mUnitSelected) mAttackTarget->SetNormalTexture();
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

	// Remove laser
	if (mpMdlHarvest)
	{
		mspMshWorkerLaser->RemoveModel(mpMdlHarvest);
		mpMdlHarvest = nullptr;
		mHarvesting = false;
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
bool CWorker::Attack(CGameObject* pTarget, float hitMod, float damageMod)
{
	if (mUnitSelected) mAttackTarget->SetNormalTexture();
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

bool CWorker::Update()
{
	if (mState == OBJ_INSPACE) return CGroundUnit::Update();

	// If it has a path target
	if (mHasPathTarget)
	{
		// Cannot have a path target and a mineral target
		if (mpActiveMineral)
		{
			mpActiveMineral->SetUsage(false);
			mpActiveMineral = nullptr;
			mHarvesting = false;
		}
		//Cannot have a heal target and path target
		if (mpHealTarget)
		{
			mpHealTarget = nullptr;
			mHealing = false;
		}
	}
	//If it has a heal target
	if (mpHealTarget)
	{
		//Cannot have a heal target and mineral target
		if (mpActiveMineral)
		{
			mpActiveMineral->SetUsage(false);
			mpActiveMineral = nullptr;
			mHarvesting = false;
		}

		if (mpObjModel)
		{
			float threshold = 5.0f;
			float distance;

			DX::XMFLOAT4X4 objMatrix;
			mpObjModel->GetMatrix(&objMatrix.m[0][0]);
			DX::XMFLOAT3 localZ{ objMatrix.m[2][0], objMatrix.m[2][1], objMatrix.m[2][2] };

			bool rayCollision = mpHealTarget->RayCollision(mWorldPos, localZ, distance);

			// Check the distance is less than the threshold
			if (distance <= threshold && rayCollision)
			{
				mHealing = true;
			}
			else
			{
				mHealing = false;
				LookingAt(mpHealTarget->GetWorldPos());
				Move();
			}
		}
	}

	// Check if the worker has an active material (and is still alive)
	if (mpActiveMineral && mpObjModel)
	{
		// Check to see if the worker is close enough to the active mineral
		float threshold = 5.0f;
		float distance;

		// Get the local Z axis of the worker unit
		DX::XMFLOAT4X4 objMatrix;
		mpObjModel->GetMatrix(&objMatrix.m[0][0]);
		DX::XMFLOAT3 localZ{ objMatrix.m[2][0], objMatrix.m[2][1], objMatrix.m[2][2] };

		bool rayCollision = mpActiveMineral->RayCollision(mWorldPos, localZ, distance);

		// Check the distance is less than the threshold
		if (distance <= threshold && rayCollision)
		{
			mHarvesting = true;
		}
		else
		{
			mHarvesting = false;
			LookingAt(mpActiveMineral->GetWorldPos());
			Move();
		}
	}
	//If the worker is currently healing a unit
	if (mHealing)
	{
		RepairUnit();
	}
	else
	{
		if (mpHealingSpark)
		{
			mpHealingSpark->~CSpark();
			mpHealingSpark = nullptr;
		}
	}

	// Check if the unit is harvesting in order to create beams of energy for mining animations
	if (mHarvesting)
	{
		// Check if a harvesting model already exists
		if (!mpMdlHarvest)
		{
			// Create a harvest model from the position of the player to the mineral pile
			mpMdlHarvest = mspMshWorkerLaser->CreateModel(mWorldPos.x, mWorldPos.y + 2.0f, mWorldPos.z);
			mpMdlHarvest->LookAt(mpActiveMineral->GetModel());

			// Scale the harvesting model & rotate a little bit
			mpMdlHarvest->ScaleZ(10.0f);
			mpMdlHarvest->ScaleX(0.25f);
			mpMdlHarvest->ScaleY(0.25f);
			mpMdlHarvest->SetSkin("tlxadd_lazer - red.tga");
		}
		else
		{
			// Simple animation of making the model rotate
			mpMdlHarvest->RotateLocalZ(50.0f * gFrameTime);
		}
		if (!mpCollectionSpark)
		{
			mpCollectionSpark = new CSpark(mpActiveMineral->GetWorldPos());
		}
		else
		{
			mpCollectionSpark->UpdateSystem();
		}
	}
	else
	{
		// Check if a harvesting model exists
		if (mpMdlHarvest)
		{
			mspMshWorkerLaser->RemoveModel(mpMdlHarvest);
			mpMdlHarvest = nullptr;
		}
		if (mpCollectionSpark)
		{
			mpCollectionSpark->~CSpark();
			mpCollectionSpark = nullptr;
		}
	}

	return CGroundUnit::Update();
}
