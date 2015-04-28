//-----------------------------------------------------
// FILE: Infantry.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Infantry.h"
#include "WorldState.h"

IMesh* CInfantry::mspMshInfantry = nullptr;
IMesh* CInfantry::mspMshInfantryBullet = nullptr;


//-----------------------------------------------------
// INFANTRY CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CInfantry::CInfantry()
{
	mAgentInfo = SAgentData(GAV_INFANTRY, "Light Tank");
	mMaxHealth = 250.0f;
	mHealth = 250.0f;
	mSpeed = 18.0f;
	mProductionTime = 12.0f;
	mCurProductionTimeLeft = mProductionTime;
	mDamage = 15.0f;
	mFireRate = 2.0f;
	mAttackTimer = 1.0f / mFireRate;
	mHasPathTarget = false;
	mAttackTarget = nullptr;
	mState = OBJ_CONSTRUCTING;
	mIsMoving = false;
	mScale = 1.5f;
	mBuildCost = 300;
	mPopCost = 2;
	mTurretNode = 3;
	mRange = 50.0f;
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
		mHasPathTarget = false;
		if (mAttackTarget && mUnitSelected) mAttackTarget->SetNormalTexture();
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
		else if (mFaction == FAC_THE_CRIMSON_LEGION)
		{
			mpObjModel->SetSkin("tigerMars.jpg");
		}
		else
		{
			mpObjModel->SetSkin("tigerRebel.jpg");
		}
	}

	// Create shadow
	mpObjShadow = CWorldState::mspMshUnitShadow->CreateModel(mWorldPos.x, 0.4f, mWorldPos.z);
}


//-----------------------------------------------------
// INFANTRY CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CInfantry::Attack(CGameObject* pTarget, float hitMod, float damageMod)
{
	// The RayCollision function calculates this value for us - so it needs no starting value. Only to be defined.
	float distance;

	// Get the local Z axis of the turret
	DX::XMFLOAT4X4 objMatrix;
	DX::XMFLOAT4X4 turMatrix;
	mpObjModel->GetMatrix(&objMatrix.m[0][0]);
	mpObjModel->GetNode(mTurretNode)->GetMatrix(&turMatrix.m[0][0]);

	// As the matrix of the turret is RELATIVE to the base model matrix, you have to multiply them together to get the ACTUAL matrix for the turret
	DX::XMFLOAT4X4 finalMatrix;
	DX::XMMATRIX matMul = DX::XMMatrixMultiply(DX::XMLoadFloat4x4(&objMatrix), DX::XMLoadFloat4x4(&turMatrix));
	DX::XMStoreFloat4x4(&finalMatrix, matMul);

	DX::XMFLOAT3 localZ{ finalMatrix.m[2][0], finalMatrix.m[2][1], finalMatrix.m[2][2] };

	DX::XMFLOAT3 target = mAttackTarget->GetWorldPos();

	// Normalise this local axis
	DX::XMVECTOR vecNormal = DX::XMVector4Normalize(DX::XMLoadFloat3(&localZ));
	DX::XMStoreFloat3(&localZ, vecNormal);

	// If the target is being looked at and is within range
	if (mAttackTarget->RayCollision(mWorldPos, localZ, distance) && distance <= mRange)
	{
		if (mAttackTimer >= (1.0f / mFireRate)) //Control rate of fire of the unit
		{
			SProjectile* newProjectile = new SProjectile();
			newProjectile->mModel = mspMshInfantryBullet->CreateModel(mWorldPos.x, mWorldPos.y, mWorldPos.z);
			newProjectile->mDirection = localZ;
			newProjectile->mSpeed = 800.0f;
			newProjectile->mLifeTime = 3.0f;

			mpProjectiles.push_back(newProjectile);
			mAttackTimer = 0.0f;
		}
	}
	else
	{
		// The attack was unsuccessful - most likely reason is the target is too far away
		// Get the direction vector from the tank to the target
		DX::XMFLOAT3 length{ target.x - mWorldPos.x, target.y - mWorldPos.y, target.z - mWorldPos.z };

		// Find out the distance to the target
		DX::XMVECTOR vecLength = DX::XMVector3LengthSq(DX::XMLoadFloat3(&length));
		DX::XMStoreFloat3(&length, vecLength);

		// Varify the distance
		if (length.x > mRange)
		{
			// Move the unit
			LookingAt(target);
			Move();
		}
	}

	// TURRET ROTATING
	// Do this bit all the time so that if the target is moving, it follows it whilst still firing (for added effect)
	DX::XMFLOAT3 vectorZ = { (target.x - mWorldPos.x), 0.0f, (target.z - mWorldPos.z) };
	// Normalise vectorZ
	vecNormal = DX::XMVector4Normalize(DX::XMLoadFloat3(&vectorZ));
	DX::XMStoreFloat3(&vectorZ, vecNormal);

	DX::XMFLOAT3 localX = { finalMatrix.m[0][0], finalMatrix.m[0][1], finalMatrix.m[0][2] };

	// Normalise this local axis
	vecNormal = DX::XMVector4Normalize(DX::XMLoadFloat3(&localX));
	DX::XMStoreFloat3(&localX, vecNormal);

	float dotProduct = Dot(localX, vectorZ);

	if (dotProduct > 0.001f)
	{
		mpObjModel->GetNode(mTurretNode)->RotateY(250.0f * gFrameTime);
	}
	else if (dotProduct < -0.001f)
	{
		mpObjModel->GetNode(mTurretNode)->RotateY(-250.0f * gFrameTime);
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
			mpObjModel->GetNode(mTurretNode)->RotateY(250.0f * gFrameTime);
		}
	}

	// Increment attack timer
	mAttackTimer += gFrameTime;
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
	else if (mFaction == FAC_THE_CRIMSON_LEGION)
	{
		pModel->SetSkin("tigerMars.jpg");
	}
	else
	{
		pModel->SetSkin("tigerRebel.jpg");
	}

	return pModel;
}

void CInfantry::SetTargetTexture()
{
	if (mpObjModel) mpObjModel->SetSkin("tigerTarget.jpg");
}

void CInfantry::SetNormalTexture()
{
	if (mpObjModel)
	{
		if (mFaction == FAC_EARTH_DEFENSE_FORCE)
		{
			mpObjModel->SetSkin("tiger.jpg");
		}
		else if (mFaction == FAC_THE_CRIMSON_LEGION)
		{
			mpObjModel->SetSkin("tigerMars.jpg");
		}
		else
		{
			mpObjModel->SetSkin("tigerRebel.jpg");
		}
	}
}

bool CInfantry::Update()
{
	if (mState == OBJ_INSPACE) return CGroundUnit::Update();

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
				mpAttackExplosions.push_back(new CExplosion(position, 10, false));
				SafeDelete(projectile);
				mpProjectiles.erase(iter);
				break; //Breaks out of the loop as the vector size has been changed, comprimising the iterator loop
			}
		}
	}

	return CGroundUnit::Update();
}
