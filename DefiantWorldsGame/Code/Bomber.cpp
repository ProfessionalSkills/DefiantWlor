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
	mMaxHealth = 400.0f;
	mHealth = 400.0f;
	mSpeed = 30.0f;
	mProductionTime = 18.0f;
	mCurProductionTimeLeft = mProductionTime;				
	mDamage = 80.0f;
	mFireRate = 0.6f;
	mAttackTimer = (1.0f / mFireRate);
	mState = OBJ_CONSTRUCTING;
	mIsMoving = false;
	mHasPathTarget = false;
	mAttackTarget = nullptr;
	mScale = 1.5f;
	mBuildCost = 600;
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
				newProjectile->mDirection = localY;
				newProjectile->mSpeed = 30.0f;

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
	else
	{
		// WRong type of target selected
		mAttackTarget = nullptr;
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

bool CBomber::Update()
{
	if (mState == OBJ_INSPACE) return CAirUnit::Update();

	// Moved projectile updating to here so that explosions can be altered based on the unit type i.e. the bomber's bomb
	// should make a bigger explosion than that of bullets from a fighter.
	if (mpProjectiles.size() > 0)
	{
		for (auto iter = mpProjectiles.begin(); iter != mpProjectiles.end(); iter++) //For each projectile that unit has fired
		{
			SProjectile* projectile = (*iter);
			projectile->Update();
			DX::XMFLOAT3 position = { projectile->mModel->GetX(), projectile->mModel->GetY(), projectile->mModel->GetZ() }; //projectile's new position stored for collision detection

			// Check to see if the attack target has been lost or it has been destroyed
			if (mAttackTarget == nullptr)
			{
				SafeDelete(projectile);
				mpProjectiles.erase(iter);
				break;
			}
			else if (mAttackTarget->SphereCollision(projectile->mCollisionSphere)) //Point to Box collision between the projectile and the attack target
			{
				mAttackTarget->TakeDamage(mDamage);
				mpAttackExplosions.push_back(new CExplosion(projectile->mModel, 25, false));
				SafeDelete(projectile);
				mpProjectiles.erase(iter);
				break; //Breaks out of the loop as the vector size has been changed, comprimising the iterator loop
			}
		}
	}

	if (mHealth <= 0.0f) return CAirUnit::Update();
	
	// Check if it has a target
	if (mHasPathTarget) //If there is a path target
	{
		//Move the unit towards the path target
		LookingAt(mPathTarget);
		Move();
	}

	// Call parent's update function
	return CAirUnit::Update();
}
