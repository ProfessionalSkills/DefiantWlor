//-----------------------------------------------------
// FILE: Bomber.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Bomber.h"

IMesh* CBomber::mspMshBomber = nullptr;
IMesh* CBomber::mspMshBomb = nullptr;

//-----------------------------------------------------
// BOMBER CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CBomber::CBomber()
{
	//Set Initial Values to member variables 
	mAgentInfo = SAgentData(GAV_BOMBER, "Bomber");
	mMaxHealth = 100.0f;
	mHealth = 100.0f;
	mSpeed = 25.0f;
	mProductionTime = 15.0f;
	mProductionCost = 0.0f;
	mCurProductionTimeLeft = mProductionTime;				
	mDamage = 100.0f;
	mFireRate = 1.5f;
	mAttackTimer = (1.0f / mFireRate);
	mState = OBJ_CONSTRUCTING;
	mIsMoving = false;
	mHasPathTarget = false;
	mAttackTarget = nullptr;
	mScale = 1.5f;
	mBuildCost = 900;
	mPopCost = 3;
	mObjectType = Q_BOMBER;
	mRange = 10.0f;
}

CBomber::~CBomber()
{

}

//-----------------------------------------------------
// BOMBER CLASS METHODS
//-----------------------------------------------------

void CBomber::UnloadIModel()
{
	if (mpObjModel != nullptr)
	{
		mspMshBomber->RemoveModel(mpObjModel);
		mpObjModel = nullptr;
		mHasPathTarget = false;
		mAttackTarget = nullptr;
	}
}

void CBomber::LoadIModel()
{
	if (mpObjModel == nullptr)
	{
		mpObjModel = mspMshBomber->CreateModel(mWorldPos.x, mWorldPos.y, mWorldPos.z);
		mpObjModel->Scale(mScale);
		mpObjModel->RotateY(180.0f);
		if (mFaction == FAC_EARTH_DEFENSE_FORCE)
		{
			mpObjModel->SetSkin("heli.jpg");
		}
		else
		{
			mpObjModel->SetSkin("heliMars.jpg");
		}
	}
}


//-----------------------------------------------------
// BOMBER CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CBomber::Attack(CGameObject* target, float hitMod, float damageMod)
{
	if (mAttackTarget->GetObjectType() != Q_FIGHTER && mAttackTarget->GetObjectType() != Q_BOMBER)
	{
		// The RayCollision function calculates this value for us - so it needs no starting value. Only to be defined.
		float distance;

		// Get the local Z axis of the turret
		DX::XMFLOAT4X4 objMatrix;
		mpObjModel->GetMatrix(&objMatrix.m[0][0]);

		DX::XMFLOAT3 localY{ objMatrix.m[1][0], objMatrix.m[1][1], objMatrix.m[1][2] };
		DX::XMFLOAT3 localZ{ objMatrix.m[2][0], objMatrix.m[2][1], objMatrix.m[2][2] };

		localY.x = -localY.x;
		localY.y = -localY.y;
		localY.z = -localY.z;

		// Normalise this local axis
		DX::XMVECTOR vecNormal = DX::XMVector4Normalize(DX::XMLoadFloat3(&localY));
		DX::XMStoreFloat3(&localY, vecNormal);

		// Normalise this local axis
		vecNormal = DX::XMVector4Normalize(DX::XMLoadFloat3(&localZ));
		DX::XMStoreFloat3(&localZ, vecNormal);

		// If the target is being looked at and is within range
		bool successfulAttack = mAttackTarget->RayCollision(mWorldPos, localY, distance);
		if (successfulAttack)
		{
			if (mAttackTimer >= (1.0f / mFireRate)) //Control rate of fire of the unit
			{
				SProjectile* newProjectile = new SProjectile();
				newProjectile->mModel = mspMshBomb->CreateModel(mWorldPos.x, mWorldPos.y, mWorldPos.z);
				newProjectile->mModel->LookAt(mAttackTarget->GetModel());
				newProjectile->mDirection = localY;
				newProjectile->mSpeed = 50.0f;

				mpProjectiles.push_back(newProjectile);
				mAttackTimer = 0.0f;
			}

			// Set a point in front of the unit to give the effect of fly-bys
			DX::XMFLOAT3 pathTarget = { localZ.x * 50.0f, localZ.y * 50.0f, localZ.z * 50.0f };
			mPathTarget = { mWorldPos.x + pathTarget.x, 0.0f, mWorldPos.z + pathTarget.z };
			mHasPathTarget = true;
		}
		// Check to see if the attack was unsuccessful & that the user has not given the unit a target
		else if (!successfulAttack && !mHasPathTarget)
		{
			// Move towards the target
			// Get the vector between the bomber and the target position (using the height of the bomber as the Y position)
			DX::XMFLOAT3 vectorZ = { (target->GetWorldPos().x - mWorldPos.x), 0.0f, (target->GetWorldPos().z - mWorldPos.z) };
			// Normalise vectorZ
			vecNormal = DX::XMVector4Normalize(DX::XMLoadFloat3(&vectorZ));
			DX::XMStoreFloat3(&vectorZ, vecNormal);

			// Do a dot product between the facing direction of the bomber and the vectorZ
			LookingAt(mAttackTarget->GetWorldPos());
			Move();
		}

		// Increment attack timer
		mAttackTimer += gFrameTime;
	}
	return false;
}

IModel* CBomber::CreateModel(DX::XMFLOAT3 pos)
{
	// Load model and set required skin
	IModel* pModel = mspMshBomber->CreateModel(pos.x, pos.y, pos.z);
	if (mFaction == FAC_EARTH_DEFENSE_FORCE)
	{
		pModel->SetSkin("heli.jpg");
	}
	else
	{
		pModel->SetSkin("heliMars.jpg");
	}

	return pModel;
}
