//-----------------------------------------------------
// FILE: AirUnit.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "AirUnit.h"


//-----------------------------------------------------
// AIR UNIT CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CAirUnit::CAirUnit()
{

}

CAirUnit::~CAirUnit()
{

}


//-----------------------------------------------------
// AIR UNIT CLASS OVERRIDE METHODS
//-----------------------------------------------------

void CAirUnit::Spawn(CGrid* pGrid, SPointData pCentre)
{
	mpObjModel = CreateModel(DX::XMFLOAT3(pGrid->GetTileData(pCentre)->GetWorldPos().x, 30.0f, (pGrid->GetTileData(pCentre)->GetWorldPos().z)));
	mWorldPos.x = pGrid->GetTileData(pCentre)->GetWorldPos().x;
	mWorldPos.y = 1.0f;
	mWorldPos.z = pGrid->GetTileData(pCentre)->GetWorldPos().z;
	mpObjModel->Scale(mScale);

	CalculateBoundingSphere();
}

bool CAirUnit::Update()
{
	// Check which state the object is currently in
	switch (mState)
	{
	case OBJ_CONSTRUCTING:
		// Objects should not be in this state when they get updated. If they are, change them to the BUILT state
		mState = OBJ_BUILT;
		return true;
		break;
	case OBJ_INSPACE:
		// Just return true as there needs to be no updating
		return true;
		break;
	case OBJ_BUILT:
		// Check how much health is left for smoke creation
		if (mHealth > 0.0f && ((mHealth / mMaxHealth) * 100.0f) <= 50.0f)
		{
			if (mWarningSmoke == nullptr)
			{
				mWarningSmoke = new CSmoke(mpObjModel, 20, 0.0f, 0.5f);
			}
		}

		// If there is no health left
		if (mHealth <= 0.0f)
		{
			if (mDestructionExplosion == nullptr)
			{
				SafeDelete(mWarningSmoke);
				mDestructionExplosion = new CExplosion(mpObjModel, 20);
				Destroy();
			}
			else
			{
				// Check if the explosion system has finished
				if (!mDestructionExplosion->UpdateSystem())
				{
					// particle system is finished
					SafeDelete(mDestructionExplosion);
					mState = OBJ_DEAD;
				}
			}

			// Object is no longer alive, however its death animation is still playing. So return true
			return true;
		}
		break;
	case OBJ_DEAD:
		// Object no longer alive
		return false;
		break;
	}

	// Update smoke system
	if (mWarningSmoke != nullptr)
	{
		mWarningSmoke->UpdateSystem();
	}

	// Always check to see if the attack target is still alive
	if (mAttackTarget != nullptr)
	{
		if (mAttackTarget->GetHealth() <= 0.0f)
		{
			mAttackTarget = nullptr;
		}
	}

	// ALL THESE UPDATES OCCUR IF THE UNIT IS NOT DEAD OR IN SPACE
	if (HasTarget()) //If there is a path target
	{
		//Move the unit towards the path target
		LookingAt(mPathTarget);
		Move();
	}
	else if (mAttackTarget != nullptr) //if there is an attack target
	{
		Attack(mAttackTarget, 100, mDamage);
	}
	else
	{
		// Correct any yaw issues
		if (mYaw >= 0.3f)
		{
			float rotateAmount = -50.0f * gFrameTime;
			mYaw += rotateAmount;
			mpObjModel->RotateLocalZ(rotateAmount);
		}
		else if (mYaw <= -0.3f)
		{
			float rotateAmount = 50.0f * gFrameTime;
			mYaw += rotateAmount;
			mpObjModel->RotateLocalZ(rotateAmount);
		}
		else
		{
			// Straighten the aircraft by the remaining amount
			mpObjModel->RotateLocalZ(mYaw);
			mYaw = 0.0f;
		}
	}

	if (mpAttackExplosions.size() > 0)
	{
		for (auto explosions : mpAttackExplosions) //For each explosion resulting from a projectile colliding
		{
			explosions->UpdateSystem(); //Update systems 
		}
	}

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
				SProjectile* tmp = projectile;
				SafeDelete(tmp);
				mpProjectiles.erase(iter);
				break;
			}
			else if (mAttackTarget->SphereCollision(projectile->mCollisionSphere)) //Point to Box collision between the projectile and the attack target
			{
				mAttackTarget->TakeDamage(mDamage);
				mpAttackExplosions.push_back(new CExplosion(projectile->mModel, 5));
				SProjectile* tmp = projectile;
				SafeDelete(tmp);
				mpProjectiles.erase(iter);
				break; //Breaks out of the loop as the vector size has been changed, comprimising the iterator loop
			}
		}
	}

	// Object is still alive, return true
	return true;
}

bool CAirUnit::LookingAt(DX::XMFLOAT3 targetLocation)
{
	// Get position of the target
	DX::XMFLOAT3 targetDirection{ targetLocation.x - mpObjModel->GetX(), 0.0f, targetLocation.z - mpObjModel->GetZ() };
	
	// Get the local X and Z axis of the aircraft
	DX::XMFLOAT4X4 matrix;
	mpObjModel->GetMatrix(&matrix.m[0][0]);
	DX::XMFLOAT3 localX{ matrix.m[0][0], matrix.m[0][1], matrix.m[0][2] };
	DX::XMFLOAT3 localZ{ matrix.m[2][0], matrix.m[2][1], matrix.m[2][2] };

	// Normalise local x & z axis
	DX::XMVECTOR vecNormal = DX::XMVector4Normalize(DX::XMLoadFloat3(&localX));
	DX::XMStoreFloat3(&localX, vecNormal);
	vecNormal = DX::XMVector4Normalize(DX::XMLoadFloat3(&localZ));
	DX::XMStoreFloat3(&localZ, vecNormal);

	// Normalise direction
	vecNormal = DX::XMVector4Normalize(DX::XMLoadFloat3(&targetDirection));
	DX::XMStoreFloat3(&targetDirection, vecNormal);

	// Get dot product between target and localX axis
	float dotProduct = Dot(targetDirection, localX);
	bool left = false;
	bool right = false;

	// Check which direction the target is in
	if (dotProduct > 0.01f)
	{
		right = true;
		mpObjModel->RotateY(100.0f * gFrameTime);
	}
	else if (dotProduct < -0.01f)
	{
		left = true;
		mpObjModel->RotateY(-100.0f * gFrameTime);
	}
	else
	{
		// Do another dot product, this time checking for it being in front
		dotProduct = Dot(targetDirection, localZ);

		// Check for behind
		if (dotProduct < 0.0f)
		{
			// Rotate a litte so target is no longer directly behind
			mpObjModel->RotateY(-20.0f * gFrameTime);
		}
	}

	// Turn the aircraft - check which direction to turn
	if (left)
	{
		// Check if the yaw is already at max
		if (mYaw >= 30.0f) return true;

		float rotateAmount = 50.0f * gFrameTime;
		mYaw += rotateAmount;
		mpObjModel->RotateLocalZ(rotateAmount);
	}
	else if (right)
	{
		// Check if the yaw is already at max
		if (mYaw <= -30.0f) return true;
		
		float rotateAmount = -50.0f * gFrameTime;
		mYaw += rotateAmount;
		mpObjModel->RotateLocalZ(rotateAmount);
	}
	else
	{
		// Target is ahead - straighten up
		// Determine if yaw is positive or negative
		if (mYaw >= 0.3f)
		{
			float rotateAmount = -50.0f * gFrameTime;
			mYaw += rotateAmount;
			mpObjModel->RotateLocalZ(rotateAmount);
		}
		else if (mYaw <= -0.3f)
		{
			float rotateAmount = 50.0f * gFrameTime;
			mYaw += rotateAmount;
			mpObjModel->RotateLocalZ(rotateAmount);
		}
		else
		{
			// Straighten the aircraft by the remaining amount
			mpObjModel->RotateLocalZ(mYaw);
			mYaw = 0.0f;
		}
	}

	return true;
}

void CAirUnit::Move()
{
	if (mHasPathTarget)
	{
		float MaxX = mPathTarget.x + 3.0f;
		float MinX = mPathTarget.x - 3.0f;

		float MaxZ = mPathTarget.z + 3.0f;
		float MinZ = mPathTarget.z - 3.0f;

		if (mWorldPos.x > MinX && mWorldPos.x < MaxX && mWorldPos.z > MinZ && mWorldPos.z < MaxZ)
		{
			// Set path target to null
			mHasPathTarget = false;
		}
		else
		{
			float movement = mSpeed * gFrameTime;
			mpObjModel->MoveLocalZ(movement);
			mWorldPos = DX::XMFLOAT3(mpObjModel->GetX(), mpObjModel->GetY(), mpObjModel->GetZ());
			mBoundingSphere.MoveTo(mWorldPos);
		}
	}
	else if (mAttackTarget != nullptr)
	{
		float xDist = mAttackTarget->GetWorldPos().x - mWorldPos.x;
		float yDist = mAttackTarget->GetWorldPos().y - mWorldPos.y;
		float zDist = mAttackTarget->GetWorldPos().z - mWorldPos.z;

		float Distance = ((xDist * xDist) + (yDist * yDist) + (zDist * zDist));

		if (Distance > (mRange * mRange))
		{
			float movement = mSpeed * gFrameTime;
			mpObjModel->MoveLocalZ(movement);
			mWorldPos = DX::XMFLOAT3(mpObjModel->GetX(), mpObjModel->GetY(), mpObjModel->GetZ());
			mBoundingSphere.MoveTo(mWorldPos);
		}
	}
}

