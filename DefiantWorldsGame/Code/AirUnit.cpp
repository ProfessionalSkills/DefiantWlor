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
	if (HasTarget()) //If there is a path target
	{
		if (LookingAt(mPathTarget)) //Rotates the unit to face the path target
		{
			Move(); //Move the unit towards the path target
		}
	}
	else if (mAttackTarget != nullptr) //Else if there is an attack target
	{
		if (LookingAt(mAttackTarget->GetWorldPos())) //Rotate the unit to face the target
		{
			Move(); //Move in range of the target
		}
	}

	if (mpProjectiles.size() > 0)
	{
		for (auto iter = mpProjectiles.begin(); iter != mpProjectiles.end(); iter++) //For each projectile that unit has fired
		{
			SProjectile* projectile = (*iter);
			projectile->mModel->MoveLocalZ(projectile->mSpeed * gFrameTime); //Move the projectile 
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
				else if (BoxCollision(position, mAttackTarget->GetWorldPos(), 3.0f)) //Point to Box collision between the projectile and the attack target
				{
					mAttackTarget->TakeDamage(mDamage);
					mpAttackExplosions.push_back(new CExplosion(projectile->mModel, 50));
					SProjectile* tmp = projectile;
					SafeDelete(tmp);
					mpProjectiles.erase(iter);
					break; //Breaks out of the loop as the vector size has been changed, comprimising the iterator loop
				}
			}
		}
	}

	if (mpAttackExplosions.size() > 0)
	{
		for (auto explosions : mpAttackExplosions) //For each explosion resulting from a projectile colliding
		{
			explosions->UpdateSystem(); //Update systems 
		}
	}

	// Check if the model is still alive
	if (mState == OBJ_DEAD)
	{
		Destroy();
		return false;
	}
	else
	{
		return true;
	}
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

		}
		else
		{
			float matrix[16];
			mpObjModel->GetMatrix(matrix);
			float movement = 20.0f * gFrameTime;
			mpObjModel->MoveLocalZ(movement);
			mWorldPos = DX::XMFLOAT3(mpObjModel->GetX(), mpObjModel->GetY(), mpObjModel->GetZ());
			DX::XMFLOAT3 moveAmount = { matrix[8] * movement, matrix[9] * movement, matrix[10] * movement };
			mBoundingSphere.Move(mWorldPos);
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
			mBoundingSphere.Move(mWorldPos);
		}
		else
		{
			Attack(mAttackTarget, 100, mDamage);
		}
	}
}

