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
	mAgentInfo = SAgentData(GAV_ARTILLERY, "Artillery");
	mMaxHealth = 100.0f;
	mHealth = 100.0f;
	mSpeed = 15.0f;
	mProductionTime = 15.0f;
	mProductionCost = 0.0f;
	mCurProductionTimeLeft = mProductionTime;
	mDamage = 20.0f;
	mFireRate = 0.333333f;
	mAttackTimer = 1.0f / mFireRate;
	mState = OBJ_CONSTRUCTING;
	mIsMoving = false;
	mHasPathTarget = false;
	mAttackTarget = nullptr;
	mScale = 1.5f;
	mBuildCost = 150;
	mPopCost = 4;
	mTurretNode = 0;
	mObjectType = Q_ARTILLERY;
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
		else
		{
			mpObjModel->SetSkin("marsAAMars.jpg");
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
		DX::XMFLOAT4X4 objMatrix;
		mpObjModel->GetMatrix(&objMatrix.m[0][0]);

		DX::XMFLOAT3 localZ{ objMatrix.m[2][0], objMatrix.m[2][1], objMatrix.m[2][2] };

		// Normalise this local axis
		DX::XMVECTOR vecNormal = DX::XMVector4Normalize(DX::XMLoadFloat3(&localZ));
		DX::XMStoreFloat3(&localZ, vecNormal);

		// If the target is being looked at and is within range
		if (mAttackTarget->RayCollision(mWorldPos, localZ, distance) && distance <= (mRange* mRange))
		{
			if (mAttackTimer >= (1.0f / mFireRate)) //Control rate of fire of the unit
			{
				SProjectile* newProjectile = new SProjectile();
				newProjectile->mModel = mspMshArtilleryShell->CreateModel(mWorldPos.x, mWorldPos.y, mWorldPos.z);
				newProjectile->mDirection = localZ;
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
			mpObjModel->RotateY(150.0f * gFrameTime);
		}
		else if (dotProduct < -0.001f)
		{
			mpObjModel->RotateY(-150.0f * gFrameTime);
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
	else
	{
		pModel->SetSkin("marsAAMars.jpg");
	}

	return pModel;
}
