//-----------------------------------------------------
// FILE: GroundUnit.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "GroundUnit.h"

//-----------------------------------------------------
// GROUND UNIT CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CGroundUnit::CGroundUnit()
{
	mScale = 2.0f;
}

CGroundUnit::~CGroundUnit()
{	

}


//-----------------------------------------------------
// GROUND UNIT CLASS OVERRIDE METHODS
//-----------------------------------------------------
void CGroundUnit::Spawn(CGrid* pGrid, SPointData pCentre)
{					
	//Sets the tile usage so vehicles cannot overlap and spawns the vehicle in

	mpObjModel = CreateModel(DX::XMFLOAT3(pGrid->GetTileData(pCentre)->GetWorldPos().x, 1.0f, (pGrid->GetTileData(pCentre)->GetWorldPos().z)));
	mpObjModel->RotateY(180.0f);
	mWorldPos.x = pGrid->GetTileData(pCentre)->GetWorldPos().x;
	mWorldPos.y = 1.0f;
	mWorldPos.z = pGrid->GetTileData(pCentre)->GetWorldPos().z;
	mpObjModel->Scale(mScale);

	CalculateBoundingSphere();
}


bool CGroundUnit::Update()
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
		// Check if health is below half
		if (((mHealth / mMaxHealth) * 100.0f) <= 50.0f)
		{
			if (mWarningSmoke == nullptr)
			{
				if (mpObjModel)
					mWarningSmoke = new CSmoke(mpObjModel, 20, 0.0f, 0.5f);
			}
		}

		// Check if there is no health left
		if (mHealth <= 0.0f)
		{
			if (mDestructionExplosion == nullptr)
			{
				SafeDelete(mWarningSmoke);
				mDestructionExplosion = new CExplosion(mpObjModel, 20, false);
				Destroy();
			}
			else
			{
				// Check if the explosion system has finished
				if (!mDestructionExplosion->UpdateSystem())
				{
					// particle system is finished
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

	// ALL THESE UPDATES OCCUR IF THE UNIT IS NOT DEAD OR IN SPACE
	if (HasTarget() && !mAttackTarget) //If there is a path target
	{
		//Move the unit towards the path target
		LookingAt(mPathTarget); //Rotates the unit to face the path target
		Move();
	}
	else if (mAttackTarget != nullptr) //if there is an attack target
	{
		// Check if target is dead
		if (mAttackTarget->GetHealth() <= 0.0f)
		{
			mAttackTarget = nullptr;
		}

		if (mAttackTarget != nullptr)
		{
			Attack(mAttackTarget, 100, mDamage);
		}
	}
	else
	{
		if (mAutoTargetting >= mTargettingTimer)
		{
			mAutoTargetting = 0.0f;
			mTargettingTimer = gpRandomiser->GetRandomFloat(5.0f, 10.0f);
		}
		mAutoTargetting += gFrameTime;
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
				if (mAttackTarget == nullptr || projectile->mLifeTime <= 0.0f)
				{
					SafeDelete(projectile);
					mpProjectiles.erase(iter);
					break;
				}
				else if (mAttackTarget->SphereCollision(projectile->mCollisionSphere)) //Point to Box collision between the projectile and the attack target
				{
					mAttackTarget->TakeDamage(mDamage);
					mpAttackExplosions.push_back(new CExplosion(projectile->mModel, 5, false));
					SafeDelete(projectile);
					mpProjectiles.erase(iter);
					break; //Breaks out of the loop as the vector size has been changed, comprimising the iterator loop
				}
		}
	}

	// Object is still alive, return true
	return true;
}
bool CGroundUnit::LookingAt(DX::XMFLOAT3 target)
{	
	DX::XMFLOAT3 targetPosition = target;
	DX::XMFLOAT3 vectorZ = { (targetPosition.x - mpObjModel->GetX()), 0.0f, (targetPosition.z - mpObjModel->GetZ()) };

	float matrix[16];
	mpObjModel->GetMatrix(matrix);

	DX::XMFLOAT3 facingVector = { matrix[8], matrix[9], matrix[10] };
	const DX::XMFLOAT3 kYAxis(0.0f, 1.0f, 0.0f);

	float dotProduct = Dot(vectorZ, Cross(kYAxis, facingVector));

	if (dotProduct > 0.01f)
	{
		mpObjModel->RotateY(100.0f * gFrameTime);
		return false;
	}
	else if (dotProduct < -0.01f)
	{
		mpObjModel->RotateY(-100.0f * gFrameTime);
		return false;
	}
	else
	{
		return true;
	}
}

void CGroundUnit::Move()
{
	if (HasTarget())
	{
		int MaxX = mPathTarget.x + 1.0f;
		int MinX = mPathTarget.x - 1.0f;

		int MaxZ = mPathTarget.z + 1.0f;
		int MinZ = mPathTarget.z - 1.0f;

		if (mWorldPos.x > MinX && mWorldPos.x < MaxX && mWorldPos.z > MinZ && mWorldPos.z < MaxZ)
		{
			mHasPathTarget = false;
		}
		else
		{
			float matrix[16];
			mpObjModel->GetMatrix(matrix);
			float movement = mSpeed * gFrameTime;
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
			float movement = mSpeed * gFrameTime;
			mpObjModel->MoveLocalZ(movement);
			mWorldPos = DX::XMFLOAT3(mpObjModel->GetX(), mpObjModel->GetY(), mpObjModel->GetZ());
			DX::XMFLOAT3 moveAmount = { matrix[8] * movement, matrix[9] * movement, matrix[10] * movement };
			mBoundingSphere.MoveTo(mWorldPos);
		}
	}
}