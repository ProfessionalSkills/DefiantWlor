//-----------------------------------------------------
// FILE: Fighter.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Fighter.h"

IMesh* CFighter::mspMshFighter = nullptr;
IMesh* CFighter::mspMshFighterBullet = nullptr;

//-----------------------------------------------------
// FIGHTER CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CFighter::CFighter()
{
	mAgentInfo = SAgentData(GAV_FIGHTER, "Fighter");
	mMaxHealth = 100.0f;
	mHealth = 100.0f;
	mSpeed = 35.0f;
	mProductionTime = 15.0f;
	mProductionCost = 0.0f;
	mCurProductionTimeLeft = mProductionTime;
	mDamage = 5.0f;
	mFireRate = 12.0f;
	mAttackTimer = 1.0f / mFireRate;
	mRange = 15.0f;
	mState = OBJ_CONSTRUCTING;
	mIsMoving = false;
	mHasPathTarget = false;
	mAttackTarget = nullptr;
	mScale = 2.0f;
	mBuildCost = 600;
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
		else
		{
			mpObjModel->SetSkin("machineMars.jpg");
		}
	}
}


//-----------------------------------------------------
// FIGHTER CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CFighter::Attack(CGameObject* target, float hitMod, float damageMod)
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
	bool successfulAttack = mAttackTarget->RayCollision(mWorldPos, localZ, distance);
	if (successfulAttack)
	{
		if (mAttackTimer >= (1.0f / mFireRate)) //Control rate of fire of the unit
		{
			SProjectile* newProjectile = new SProjectile();
			newProjectile->mModel = mspMshFighterBullet->CreateModel(mWorldPos.x, mWorldPos.y, mWorldPos.z);
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
	// Check to see if the attack was unsuccessful & that the user has not given the unit a target
	else if (!successfulAttack && !mHasPathTarget)
	{
		// Move towards the target
		DX::XMFLOAT3 localZ{ objMatrix.m[2][0], objMatrix.m[2][1], objMatrix.m[2][2] };
		// Normalise this local axis
		vecNormal = DX::XMVector4Normalize(DX::XMLoadFloat3(&localZ));
		DX::XMStoreFloat3(&localZ, vecNormal);

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
	else
	{
		pModel->SetSkin("machineMars.jpg");
	}

	return pModel;
}

bool CFighter::Update()
{
	if (mState == OBJ_INSPACE) return CAirUnit::Update();

	// Check if object exists before rotating blades 
	if (mpObjModel) mpObjModel->GetNode(4)->RotateY(mRotarSpeed * gFrameTime);

	return CAirUnit::Update();
}
