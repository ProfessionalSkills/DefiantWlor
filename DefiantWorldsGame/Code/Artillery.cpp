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
	mAgentInfo = SAgentData(GAV_ARTILLERY, "Anti-Air");
	mMaxHealth = 300.0f;
	mHealth = 300.0f;
	mSpeed = 20.0f;
	mProductionTime = 15.0f;
	mCurProductionTimeLeft = mProductionTime;
	mDamage = 50.0f;
	mFireRate = 0.8f;
	mAttackTimer = 1.0f / mFireRate;
	mState = OBJ_CONSTRUCTING;
	mIsMoving = false;
	mHasPathTarget = false;
	mAttackTarget = nullptr;
	mScale = 1.5f;
	mBuildCost = 500;
	mPopCost = 4;
	mTurretNode = 0;
	mObjectType = Q_ARTILLERY;
	mRange = 60.0f;
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
		else if (mFaction == FAC_THE_CRIMSON_LEGION)
		{
			mpObjModel->SetSkin("marsAAMars.jpg");
		}
		else
		{
			mpObjModel->SetSkin("marsAARebel.jpg");
		}
	}
}


//-----------------------------------------------------
// ARTILLERY CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CArtillery::Attack(CGameObject* target, float hitMod, float damageMod)
{
	if (mAttackTarget->GetObjectType() == Q_BOMBER || mAttackTarget->GetObjectType() == Q_FIGHTER)
	{
		// The RayCollision function calculates this value for us - so it needs no starting value. Only to be defined.
		float distance;

		// Get the local Z axis of the turret
		DX::XMFLOAT3 target = mAttackTarget->GetWorldPos();
		DX::XMFLOAT4X4 objMatrix;
		mpObjModel->GetMatrix(&objMatrix.m[0][0]);

		DX::XMFLOAT3 localZ{ objMatrix.m[2][0], objMatrix.m[2][1], objMatrix.m[2][2] };

		// Normalise this local axis
		DX::XMVECTOR vecNormal = DX::XMVector4Normalize(DX::XMLoadFloat3(&localZ));
		DX::XMStoreFloat3(&localZ, vecNormal);
		DX::XMFLOAT3 worldPos = { mWorldPos.x, 75.0f, mWorldPos.z };
		// If the target is being looked at and is within range
		if (mAttackTarget->RayCollision(worldPos, localZ, distance) && distance <= mRange)
		{
			// Calculate direction vector from the aircraft to the target
			DX::XMFLOAT3 dir{ target.x - mWorldPos.x, target.y - mWorldPos.y, target.z - mWorldPos.z };

			// Normalise direction vector
			DX::XMVECTOR vecNormal = DX::XMVector4Normalize(DX::XMLoadFloat3(&dir));
			DX::XMStoreFloat3(&dir, vecNormal);

			if (mAttackTimer >= (1.0f / mFireRate)) //Control rate of fire of the unit
			{
				SProjectile* newProjectile = new SProjectile();
				newProjectile->mModel = mspMshArtilleryShell->CreateModel(mWorldPos.x, mWorldPos.y, mWorldPos.z);
				newProjectile->mDirection = dir;
				newProjectile->mSpeed = 500.0f;
				newProjectile->mLifeTime = 10.0f;

				mpProjectiles.push_back(newProjectile);
				mAttackTimer = 0.0f;
			}
		}
		else
		{
			// Move the unit toward the target
			LookingAt(target);
			Move();
		}
		// Increment attack timer
		mAttackTimer += gFrameTime;
	}
	else
	{
		// Wrong target has been selected
		mAttackTarget = nullptr;
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
	else if (mFaction == FAC_THE_CRIMSON_LEGION)
	{
		pModel->SetSkin("marsAAMars.jpg");
	}
	else
	{
		pModel->SetSkin("marsAARebel.jpg");
	}

	return pModel;
}
