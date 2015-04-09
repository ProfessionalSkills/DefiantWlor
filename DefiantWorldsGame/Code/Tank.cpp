//-----------------------------------------------------
// FILE: Tank.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Tank.h"

IMesh* CTank::mspMshTank = nullptr;
IMesh* CTank::mspMshTankShell = nullptr;
//-----------------------------------------------------
// TANK CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CTank::CTank()
{
	mAgentInfo = SAgentData(GAV_TANK, "Tank");
	mMaxHealth = 100.0f;
	mHealth = 100.0f;
	mSpeed = 1.0f;
	mProductionTime = 15.0f;
	mProductionCost = 0.0f;
	mCurProductionTimeLeft = mProductionTime;
	mDamage = 1.0f;
	mFireRate = 0.5f;
	mAttackTimer = 1.0f / mFireRate;
	mHasPathTarget = false;
	mAttackTarget = nullptr;
	mState = OBJ_CONSTRUCTING;
	mIsMoving = false;
	mScale = 1.5f;
	mBuildCost = 300;
	mPopCost = 3;
}

CTank::~CTank()
{

}

//-----------------------------------------------------
// TANK CLASS METHODS
//-----------------------------------------------------

void CTank::UnloadIModel()
{
	if (mpObjModel != 0)
	{
		mspMshTank->RemoveModel(mpObjModel);
		mpObjModel = nullptr;
	}
}

void CTank::LoadIModel()
{
	if (mpObjModel == nullptr)
	{
		mpObjModel = mspMshTank->CreateModel(mWorldPos.x, mWorldPos.y, mWorldPos.z);
		mpObjModel->Scale(mScale);
		mpObjModel->RotateY(180.0f);
		if (mFaction == FAC_EARTH_DEFENSE_FORCE)
		{
			mpObjModel->SetSkin("Hovertank01.jpg");
		}
		else
		{
			mpObjModel->SetSkin("Hovertank01Mars.jpg");
		}
	}
}


//-----------------------------------------------------
// TANK CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CTank::Attack(CGameObject* target, float hitMod, float damageMod)
{
	if (mAttackTimer >= (1.0f / mFireRate))
	{
		// Check to see if the worker is close enough to the target to be able to attack it
		float distance = 100.0f;

		// Get the local Z axis of the worker unit
		DX::XMFLOAT4X4 objMatrix;
		mpObjModel->GetMatrix(&objMatrix.m[0][0]);
		DX::XMFLOAT3 localZ{ objMatrix.m[2][0], objMatrix.m[2][1], objMatrix.m[2][2] };

		// Normalise this local axis
		DX::XMVECTOR vecNormal = DX::XMVector4Normalize(DX::XMLoadFloat3(&localZ));
		DX::XMStoreFloat3(&localZ, vecNormal);

		// If the target is being looked at and is within range
		if (mAttackTarget->RayCollision(mWorldPos, localZ, distance) && distance <= mRange)
		{
			SProjectile* newProjectile = new SProjectile();
			newProjectile->mModel = mspMshTankShell->CreateModel(mWorldPos.x, mWorldPos.y, mWorldPos.z);
			//newProjectile->mModel->LookAt(mAttackTarget->GetModel());
			newProjectile->mDirection = localZ;
			newProjectile->mSpeed = 50.0f;

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

bool CTank::Destroy()
{
	UnloadIModel();
	return false;
}

IModel* CTank::CreateModel(DX::XMFLOAT3 pos)
{
	// Load model and set required skin
	IModel* pModel = mspMshTank->CreateModel(pos.x, pos.y, pos.z);
	if (mFaction == FAC_EARTH_DEFENSE_FORCE)
	{
		pModel->SetSkin("Hovertank01.jpg");
	}
	else
	{
		pModel->SetSkin("Hovertank01Mars.jpg");
	}

	return pModel;
}
