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
	if (mAttackTimer >= (1.0f / mFireRate))
	{
		// Check to see if the worker is close enough to the target to be able to attack it
		float distance;

		// Get the local Y axis of the bomber as it drops bombs downwards
		DX::XMFLOAT4X4 objMatrix;
		mpObjModel->GetMatrix(&objMatrix.m[0][0]);
		DX::XMFLOAT3 localY{ objMatrix.m[1][0], objMatrix.m[1][1], objMatrix.m[1][2] };

		// Invert the local Y axis so it points down instead of up
		localY.x = -localY.x;
		localY.y = -localY.y;
		localY.z = -localY.z;

		// Normalise this local axis
		DX::XMVECTOR vecNormal = DX::XMVector4Normalize(DX::XMLoadFloat3(&localY));
		DX::XMStoreFloat3(&localY, vecNormal);

		// If the target is being looked at and is within range
		if (mAttackTarget->RayCollision(mWorldPos, localY, distance) && distance <= mRange)
		{
			SProjectile* newProjectile = new SProjectile();
			newProjectile->mModel = mspMshBomb->CreateModel(mWorldPos.x, mWorldPos.y, mWorldPos.z);
			newProjectile->mDirection = localY;
			newProjectile->mSpeed = 20.0f;

			mpProjectiles.push_back(newProjectile);
		}

		mAttackTimer = 0.0f;
	}
	else
	{
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
