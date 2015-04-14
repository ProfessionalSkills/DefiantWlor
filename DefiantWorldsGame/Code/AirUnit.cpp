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
		if (((mHealth / mMaxHealth) * 100.0f) <= 66.6f)
		{
			mState = OBJ_DAMAGED;
		}
		break;
	case OBJ_DAMAGED:
		if (((mHealth / mMaxHealth) * 100.0f) <= 33.3f)
		{
			mState = OBJ_WARNING;
		}
		break;
	case OBJ_WARNING:
		if (mHealth <= 0.0f)
		{
			if (mDestructionExplosion == nullptr)
			{
				mDestructionExplosion = new CExplosion(mpObjModel, 100);
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

	// ALL THESE UPDATES OCCUR IF THE UNIT IS NOT DEAD OR IN SPACE
	if (HasTarget()) //If there is a path target
	{
		//Move the unit towards the path target
		Move();
		LookingAt(mPathTarget);
	}
	if (mAttackTarget != nullptr) //if there is an attack target
	{
		Attack(mAttackTarget, 100, mDamage);

		// Check if target is dead
		if (mAttackTarget->GetHealth() <= 0.0f)
		{
			mAttackTarget = nullptr;
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

			if (projectile == mpProjectiles.front()) //As all projectiles move at the same speed, the only projectile that will collide is the one fired first  
			{
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
					mpAttackExplosions.push_back(new CExplosion(projectile->mModel, 20));
					SProjectile* tmp = projectile;
					SafeDelete(tmp);
					mpProjectiles.erase(iter);
					break; //Breaks out of the loop as the vector size has been changed, comprimising the iterator loop
				}
			}
		}
	}

	// Object is still alive, return true
	return true;
}

bool CAirUnit::LookingAt(DX::XMFLOAT3 targetLocation)
{
	DX::XMFLOAT3 targetPosition = { targetLocation.x, 30.0f, targetLocation.z };
	DX::XMFLOAT3 vectorZ = { (targetPosition.x - mpObjModel->GetX()), (targetPosition.y - mpObjModel->GetY()), (targetPosition.z - mpObjModel->GetZ()) };
	float matrix[16];
	mpObjModel->GetMatrix(matrix);

	DX::XMFLOAT3 facingVector = { matrix[8], matrix[9], matrix[10] };
	const DX::XMFLOAT3 kYAxis(0.0f, 1.0f, 0.0f);

	float dotProduct = Dot(vectorZ, Cross(kYAxis, facingVector));

	if (dotProduct > 0.1f)
	{
		mpObjModel->RotateY(100.0f * gFrameTime);
		if (mYaw >= -30.0f)
		{
			float mZRotate = -50.0f * gFrameTime;
			mpObjModel->RotateLocalZ(mZRotate);
			mYaw += mZRotate;
		}
	}
	else if (dotProduct < -0.1f)
	{
		mpObjModel->RotateY(-100.0f * gFrameTime);
		if (mYaw <= 30.0f)
		{
			float mZRotate = 50.0f * gFrameTime;
			mpObjModel->RotateLocalZ(mZRotate);
			mYaw += mZRotate;
		}
	}
	else
	{
		if (mYaw >= 5.0f)
		{
			float mZRotate = -50.0f * gFrameTime;
			mpObjModel->RotateLocalZ(mZRotate);
			mYaw += mZRotate;
		}
		else if (mYaw <= -5.0f)
		{
			float mZRotate = 50.0f * gFrameTime;
			mpObjModel->RotateLocalZ(mZRotate);
			mYaw += mZRotate;
		}
	}
	return true;
}

void CAirUnit::Move()
{
	if (mHasPathTarget)
	{
		float MaxX = mPathTarget.x + 1.0f;
		float MinX = mPathTarget.x - 1.0f;

		float MaxZ = mPathTarget.z + 1.0f;
		float MinZ = mPathTarget.z - 1.0f;

		if (mWorldPos.x > MinX && mWorldPos.x < MaxX && mWorldPos.z > MinZ && mWorldPos.z < MaxZ)
		{
			// Set path target to null
			mHasPathTarget = false;
		}
		else
		{
			float matrix[16];
			mpObjModel->GetMatrix(matrix);
			float movement = 20.0f * gFrameTime;
			mpObjModel->MoveLocalZ(movement);
			mWorldPos = DX::XMFLOAT3(mpObjModel->GetX(), mpObjModel->GetY(), mpObjModel->GetZ());
			DX::XMFLOAT3 moveAmount = { matrix[8] * movement, matrix[9] * movement, matrix[10] * movement };
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
			float matrix[16];
			mpObjModel->GetMatrix(matrix);
			float movement = 20.0f * gFrameTime;
			mpObjModel->MoveLocalZ(movement);
			mWorldPos = DX::XMFLOAT3(mpObjModel->GetX(), mpObjModel->GetY(), mpObjModel->GetZ());
			DX::XMFLOAT3 moveAmount = { matrix[8] * movement, matrix[9] * movement, matrix[10] * movement };
			mBoundingSphere.MoveTo(mWorldPos);
		}
		/*else
		{
			Attack(mAttackTarget, 100, mDamage);
		}*/
	}
}

