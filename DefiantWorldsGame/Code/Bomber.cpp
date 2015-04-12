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
	mSpeed = 1.0f;
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

		DX::XMFLOAT3 localZ{ objMatrix.m[2][0], objMatrix.m[2][1], objMatrix.m[2][2] };

		// Normalise this local axis
		DX::XMVECTOR vecNormal = DX::XMVector4Normalize(DX::XMLoadFloat3(&localZ));
		DX::XMStoreFloat3(&localZ, vecNormal);
		DX::XMFLOAT3 worldPos = { mWorldPos.x, 0.0f, mWorldPos.z };
		// If the target is being looked at and is within range
		if (mAttackTarget->RayCollision(worldPos, localZ, distance) && distance <= (mRange* mRange))
		{
			if (mAttackTimer >= (1.0f / mFireRate)) //Control rate of fire of the unit
			{
				SProjectile* newProjectile = new SProjectile();
				newProjectile->mModel = mspMshBomb->CreateModel(mWorldPos.x, mWorldPos.y, mWorldPos.z);
				newProjectile->mModel->LookAt(mAttackTarget->GetModel());
				DX::XMFLOAT4X4 projectileMatrix;
				newProjectile->mModel->GetMatrix(&projectileMatrix.m[0][0]);
				DX::XMFLOAT3 projZ{ projectileMatrix.m[2][0], projectileMatrix.m[2][1], projectileMatrix.m[2][2] };
				newProjectile->mDirection = projZ;
				newProjectile->mSpeed = 50.0f;

				mpProjectiles.push_back(newProjectile);
				mAttackTimer = 0.0f;
			}
		}

		// Do this bit all the time so that if the target is moving, it follows it whilst still firing (for added effect)
		DX::XMFLOAT3 vectorZ = { (target->GetWorldPos().x - mWorldPos.x), 0.0f, (target->GetWorldPos().z - mWorldPos.z) };
		// Normalise vectorZ
		vecNormal = DX::XMVector4Normalize(DX::XMLoadFloat3(&vectorZ));
		DX::XMStoreFloat3(&vectorZ, vecNormal);

		DX::XMFLOAT3 localX = { objMatrix.m[0][0], objMatrix.m[0][1], objMatrix.m[0][2] };

		// Normalise this local axis
		vecNormal = DX::XMVector4Normalize(DX::XMLoadFloat3(&localX));
		DX::XMStoreFloat3(&localX, vecNormal);

		float dotProduct = Dot(localX, vectorZ);

		if (dotProduct > 0.001f)
		{
			mpObjModel->RotateY(100.0f * gFrameTime);
		}
		else if (dotProduct < -0.001f)
		{
			mpObjModel->RotateY(-100.0f * gFrameTime);
		}

		// Check for is the dot product is in the range of -0.001 and 0.001. The reason for this is to make sure
		// that the target is IN FRONT of the turret and not behind it
		if (dotProduct > -0.001f && dotProduct < 0.001f)
		{
			// Do another dot product, this time checking for it being in front
			dotProduct = Dot(localZ, vectorZ);

			// Check for behind
			if (dotProduct < 0.0f)
			{
				mpObjModel->RotateY(150.0f * gFrameTime);
			}
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
