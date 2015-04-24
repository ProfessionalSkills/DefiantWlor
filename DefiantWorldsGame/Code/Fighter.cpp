//-----------------------------------------------------
// FILE: Fighter.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Fighter.h"
#include "WorldState.h"

IMesh* CFighter::mspMshFighter = nullptr;
IMesh* CFighter::mspMshFighterBullet = nullptr;

//-----------------------------------------------------
// FIGHTER CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CFighter::CFighter()
{
	mAgentInfo = SAgentData(GAV_FIGHTER, "Fighter");
	mMaxHealth = 200.0f;
	mHealth = 200.0f;
	mSpeed = 35.0f;
	mProductionTime = 15.0f;
	mCurProductionTimeLeft = mProductionTime;
	mDamage = 1.0f;
	mFireRate = 12.0f;
	mAttackTimer = 1.0f / mFireRate;
	mRange = 60.0f;
	mState = OBJ_CONSTRUCTING;
	mIsMoving = false;
	mHasPathTarget = false;
	mAttackTarget = nullptr;
	mScale = 2.0f;
	mBuildCost = 400;
	mRotarSpeed = 2000.0f;
	mPopCost = 3;
	mObjectType = Q_FIGHTER;
}

CFighter::~CFighter()
{

}

//-----------------------------------------------------
// FIGHTER CLASS METHODS
//-----------------------------------------------------
void CFighter::UnloadIModel()
{
	if (mpObjModel != 0)
	{
		mspMshFighter->RemoveModel(mpObjModel);
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

void CFighter::LoadIModel()
{
	if (mpObjModel == nullptr)
	{
		mpObjModel = mspMshFighter->CreateModel(mWorldPos.x, mWorldPos.y, mWorldPos.z);
		mpObjModel->Scale(mScale);
		if (mFaction == FAC_EARTH_DEFENSE_FORCE)
		{
			mpObjModel->SetSkin("machine.jpg");
		}
		else if (mFaction == FAC_THE_CRIMSON_LEGION)
		{
			mpObjModel->SetSkin("machineMars.jpg");
		}
		else
		{
			mpObjModel->SetSkin("machineRebel.jpg");
		}
	}

	// Create shadow
	mpObjShadow = CWorldState::mspMshUnitShadow->CreateModel(mWorldPos.x, 0.4f, mWorldPos.z);
}


//-----------------------------------------------------
// FIGHTER CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CFighter::Attack(CGameObject* target, float hitMod, float damageMod)
{
	// The RayCollision function calculates this value for us - so it needs no starting value. Only to be defined.
	float grndDistance;
	float airDistance;

	// Get the local Z axis of the turret
	DX::XMFLOAT4X4 objMatrix;
	mpObjModel->GetMatrix(&objMatrix.m[0][0]);

	DX::XMFLOAT3 localZ{ objMatrix.m[2][0], objMatrix.m[2][1], objMatrix.m[2][2] };

	// Normalise this local axis
	DX::XMVECTOR vecNormal = DX::XMVector4Normalize(DX::XMLoadFloat3(&localZ));
	DX::XMStoreFloat3(&localZ, vecNormal);

	// If the target is being looked at and is within range
	bool groundRayCollision = mAttackTarget->RayCollision({ mWorldPos.x, 0.0f, mWorldPos.z }, localZ, grndDistance);
	bool airRayCollision = mAttackTarget->RayCollision(mWorldPos, localZ, airDistance);
	if ((groundRayCollision && grndDistance <= mRange) || (airRayCollision && airDistance <= mRange))
	{
		// Calculate direction vector from the aircraft to the target
		DX::XMFLOAT3 target = mAttackTarget->GetWorldPos();
		DX::XMFLOAT3 dir{ target.x - mWorldPos.x, target.y - mWorldPos.y, target.z - mWorldPos.z };

		// Normalise direction vector
		DX::XMVECTOR vecNormal = DX::XMVector4Normalize(DX::XMLoadFloat3(&dir));
		DX::XMStoreFloat3(&dir, vecNormal);
		
		if (mAttackTimer >= (1.0f / mFireRate)) //Control rate of fire of the unit
		{
			SProjectile* newProjectile = new SProjectile();
			newProjectile->mModel = mspMshFighterBullet->CreateModel(mWorldPos.x, mWorldPos.y, mWorldPos.z);
			newProjectile->mDirection = dir;
			newProjectile->mSpeed = 150.0f;

			mpProjectiles.push_back(newProjectile);
			mAttackTimer = 0.0f;
		}
	}
	// Check to see if the attack was unsuccessful & that the user has not given the unit a target
	else if (!mHasPathTarget)
	{
		if (LookingAt(mAttackTarget->GetWorldPos()))
		{
			Move();
		}
	}

	// Increment attack timer
	mAttackTimer += gFrameTime;
	return false;
}

bool CFighter::Destroy()
{
	UnloadIModel();
	return false;
}

IModel* CFighter::CreateModel(DX::XMFLOAT3 pos)
{
	// Load model and set required skin
	IModel* pModel = mspMshFighter->CreateModel(pos.x, pos.y, pos.z);
	if (mFaction == FAC_EARTH_DEFENSE_FORCE)
	{
		pModel->SetSkin("machine.jpg");
	}
	else if (mFaction == FAC_THE_CRIMSON_LEGION)
	{
		pModel->SetSkin("machineMars.jpg");
	}
	else
	{
		pModel->SetSkin("machineRebel.jpg");
	}

	return pModel;
}

bool CFighter::Update()
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
				mpAttackExplosions.push_back(new CExplosion(projectile->mModel, 1, false));
				SafeDelete(projectile);
				mpProjectiles.erase(iter);
				break; //Breaks out of the loop as the vector size has been changed, comprimising the iterator loop
			}
		}
	}

	if (mHealth <= 0.0f) return CAirUnit::Update();

	// Check if object exists before rotating blades 
	if (mpObjModel) mpObjModel->GetNode(4)->RotateY(mRotarSpeed * gFrameTime);

	// Check if there is a target
	if (HasTarget()) //If there is a path target
	{
		//Move the unit towards the path target
		if (LookingAt(mPathTarget))
		{
			Move();
		}
	}

	return CAirUnit::Update();
}
