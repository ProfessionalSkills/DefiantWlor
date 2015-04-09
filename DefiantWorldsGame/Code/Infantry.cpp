//-----------------------------------------------------
// FILE: Infantry.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Infantry.h"

IMesh* CInfantry::mspMshInfantry = nullptr;
IMesh* CInfantry::mspMshInfantryBullet = nullptr;

//-----------------------------------------------------
// INFANTRY CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CInfantry::CInfantry()
{
	mAgentInfo = SAgentData(GAV_INFANTRY, "Infantry");
	mMaxHealth = 100.0f;
	mHealth = 100.0f;
	mSpeed = 20.0f;
	mProductionTime = 10.0f;
	mProductionCost = 0.0f;
	mCurProductionTimeLeft = mProductionTime;
	mDamage = 100.0f;
	mFireRate = 2.0f;
	mAttackTimer = 1.0f / mFireRate;
	mHasPathTarget = false;
	mAttackTarget = nullptr;
	mState = OBJ_CONSTRUCTING;
	mIsMoving = false;
	mScale = 1.5f;
	mBuildCost = 100;
	mPopCost = 2;
	mTurretNode = 3;
}

CInfantry::~CInfantry()
{

}

//-----------------------------------------------------
// INFANTRY CLASS METHODS
//-----------------------------------------------------

void CInfantry::UnloadIModel()
{
	if (mpObjModel != 0)
	{
		mspMshInfantry->RemoveModel(mpObjModel);
		mpObjModel = nullptr;
	}
}

void CInfantry::LoadIModel()
{
	if (mpObjModel == nullptr)
	{
		mpObjModel = mspMshInfantry->CreateModel(mWorldPos.x, mWorldPos.y, mWorldPos.z);
		mpObjModel->Scale(mScale);
		mpObjModel->RotateY(180.0f);
		if (mFaction == FAC_EARTH_DEFENSE_FORCE)
		{
			mpObjModel->SetSkin("tiger.jpg");
		}
		else
		{
			mpObjModel->SetSkin("tigerMars.jpg");
		}
	}
}


//-----------------------------------------------------
// INFANTRY CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CInfantry::Attack(CGameObject* target, float hitMod, float damageMod)
{
	if (mAttackTimer >= (1.0f / mFireRate))
	{
		// Check to see if the unit is close enough to the target to be able to attack it
		float distance = 100.0f;

		// Get the local Z axis of the unit
		float objMatrix[16];
		float projMatrix[16];
		mpObjModel->GetNode(mTurretNode)->GetMatrix(objMatrix);
		DX::XMFLOAT3 localZ{ objMatrix[8], objMatrix[9], objMatrix[10] };

		// Normalise this local axis
		DX::XMVECTOR vecNormal = DX::XMVector4Normalize(DX::XMLoadFloat3(&localZ));
		DX::XMStoreFloat3(&localZ, vecNormal);

		// Reverse the local z for the turret
		localZ.x = -localZ.x;
		localZ.y = -localZ.y;
		localZ.z = -localZ.z;

		// If the target is being looked at and is within range
		if (mAttackTarget->RayCollision(mWorldPos, localZ, distance) && distance <= mRange)
		{
			SProjectile* newProjectile = new SProjectile();
			newProjectile->mModel = mspMshInfantryBullet->CreateModel(mWorldPos.x, mWorldPos.y, mWorldPos.z);
			newProjectile->mDirection = localZ;
			newProjectile->mSpeed = 50.0f;

			mpProjectiles.push_back(newProjectile);
		}
		else
		{
			// DO TURRET ROTATING HERE WITH DOT PRODUCT AND SHIZ - also remove the endless rotating loop below
		}

		mAttackTimer = 0.0f;
	}
	else
	{
		mAttackTimer += gFrameTime;
		//ENDLESS ROTATING LOOPZ
		mpObjModel->GetNode(3)->RotateY(-50.0f * gFrameTime);
	}
	return false;
}

bool CInfantry::Destroy()
{
	UnloadIModel();
	return false;
}

IModel* CInfantry::CreateModel(DX::XMFLOAT3 pos)
{
	// Load model and set required skin
	IModel* pModel = mspMshInfantry->CreateModel(pos.x, pos.y, pos.z);
	if (mFaction == FAC_EARTH_DEFENSE_FORCE)
	{
		pModel->SetSkin("tiger.jpg");
	}
	else
	{
		pModel->SetSkin("tigerMars.jpg");
	}

	return pModel;
}
