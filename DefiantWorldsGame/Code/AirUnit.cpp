//-----------------------------------------------------
// FILE: AirUnit.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "AirUnit.h"
#include "PlayerManager.h"


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
	mpObjModel = CreateModel(DX::XMFLOAT3(pGrid->GetTileData(pCentre)->GetWorldPos().x, 20.0f, (pGrid->GetTileData(pCentre)->GetWorldPos().z)));
	mWorldPos.x = pGrid->GetTileData(pCentre)->GetWorldPos().x;
	mWorldPos.y = 20.0f;
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
		// If the height of the unit is below 50, increase it
		if (mWorldPos.y < 50.0f && mpObjModel)
		{
			mpObjModel->MoveY(30.0f * gFrameTime);
			mWorldPos.y = mpObjModel->GetY();
			mBoundingSphere.MoveTo(mWorldPos);
		}

		// Check how much health is left for smoke creation
		if (((mHealth / mMaxHealth) * 100.0f) <= 50.0f)
		{
			if (mWarningSmoke == nullptr)
			{
				if (mpObjModel)
					mWarningSmoke = new CSmoke(mpObjModel, 20, 0.0f, 0.5f);
			}
		}

		// Update smoke system
		if (mWarningSmoke != nullptr)
		{
			mWarningSmoke->UpdateSystem();
		}

		// Update explosions
		if (mpAttackExplosions.size() > 0)
		{
			for (auto explosions : mpAttackExplosions) //For each explosion resulting from a projectile colliding
			{
				explosions->UpdateSystem(); //Update systems 
			}
		}

		// If there is no health left
		if (mHealth <= 0.0f)
		{
			// Check the height of the model has hit the floor
			if (!mpObjModel)
			{
				if (!mDestructionExplosion->UpdateSystem())
				{
					// particle system is finished
					mState = OBJ_DEAD;
				}
			}
			else if (mpObjModel->GetY() > 1.0f)
			{
				// Make the model lose control
				mpObjModel->RotateLocalZ(gpRandomiser->GetRandomFloat(0.0f, 60.0f) * 10.0f * gFrameTime);
				mpObjModel->RotateLocalY(gpRandomiser->GetRandomFloat(0.0f, 100.0f) * 10.0f * gFrameTime);

				// Move down
				mpObjModel->MoveY(-20.0f * gFrameTime);
			}
			else
			{
				if (mDestructionExplosion == nullptr)
				{
					SafeDelete(mWarningSmoke);
					mDestructionExplosion = new CExplosion(mpObjModel, 20, false);
					Destroy();
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

	// Always check to see if the attack target is still alive
	if (mAttackTarget != nullptr)
	{
		if (mAttackTarget->GetHealth() <= 0.0f)
		{
			mAttackTarget = nullptr;
		}
		// Check which faction the unit is from
		else if (mFaction == FAC_EARTH_DEFENSE_FORCE)
		{
			// Check if the rebels have fled
			if (mAttackTarget->GetWorldXPos() < -2500.0f)
			{
				mAttackTarget = nullptr;
			}
		}
		else
		{
			// Check if the rebels have fled
			if (mAttackTarget->GetWorldZPos() > 4500.0f)
			{
				mAttackTarget = nullptr;
			}
		}
	}
	else
	{
		// Check if the unit is out of the grid boundaries to make it return to where it should be *TO DO*
		if (mpOwner && !mHasPathTarget)
		{
			DX::XMFLOAT3 bottomLeft = mpOwner->GetPlayerGrid()->GetGridStartPos();
			DX::XMFLOAT3 topRight = mpOwner->GetPlayerGrid()->GetGridEndPos();

			// Check if the unit is out of the area bounds
			if (mWorldPos.x < bottomLeft.x || mWorldPos.x > topRight.x || mWorldPos.z < bottomLeft.z || mWorldPos.z > topRight.z)
			{
				// Out of the area - send unit back inside the walls
				// Choose random point inside the walls
				float targetX = gpRandomiser->GetRandomFloat(bottomLeft.x, topRight.x);
				float targetZ = gpRandomiser->GetRandomFloat(bottomLeft.z, topRight.z);

				// Move unit to the new target area
				mPathTarget = {targetX, 0.0f, targetZ};
				mHasPathTarget = true;
			}
		}
	}

	// ALL THESE UPDATES OCCUR IF THE UNIT IS NOT DEAD OR IN SPACE
	if (!mHasPathTarget && mAttackTarget != nullptr) //if there is an attack target
	{
		Attack(mAttackTarget, 100, mDamage);
	}
	else if (!mHasPathTarget)
	{
		// Correct any yaw issues
		if (mYaw >= 0.01f)
		{
			float rotateAmount = -50.0f * gFrameTime;
			mYaw += rotateAmount;
			mpObjModel->RotateLocalZ(rotateAmount);
		}
		else if (mYaw <= -0.01f)
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
	bool inFront = false;

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
		// Currently, it can be assumed target is ahead
		inFront = true;
		
		// Do another dot product, this time checking for it being in front
		dotProduct = Dot(targetDirection, localZ);

		// Check for behind
		if (dotProduct < 0.0f)
		{
			// Rotate a litte so target is no longer directly behind
			mpObjModel->RotateY(-20.0f * gFrameTime);

			// Target is actually behind
			inFront = false;
		}
	}

	// Turn the aircraft - check which direction to turn
	if (left)
	{
		// Check if the yaw is already at max
		if (mYaw >= 30.0f) return inFront;

		float rotateAmount = 50.0f * gFrameTime;
		mYaw += rotateAmount;
		mpObjModel->RotateLocalZ(rotateAmount);
	}
	else if (right)
	{
		// Check if the yaw is already at max
		if (mYaw <= -30.0f) return inFront;
		
		float rotateAmount = -50.0f * gFrameTime;
		mYaw += rotateAmount;
		mpObjModel->RotateLocalZ(rotateAmount);
	}
	else
	{
		// Target is ahead - straighten up
		// Determine if yaw is positive or negative
		if (mYaw >= 0.01f)
		{
			float rotateAmount = -50.0f * gFrameTime;
			mYaw += rotateAmount;
			mpObjModel->RotateLocalZ(rotateAmount);
		}
		else if (mYaw <= -0.01f)
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

	return inFront;
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
		float movement = mSpeed * gFrameTime;
		mpObjModel->MoveLocalZ(movement);
		mWorldPos = DX::XMFLOAT3(mpObjModel->GetX(), mpObjModel->GetY(), mpObjModel->GetZ());
		mBoundingSphere.MoveTo(mWorldPos);
	}
}

