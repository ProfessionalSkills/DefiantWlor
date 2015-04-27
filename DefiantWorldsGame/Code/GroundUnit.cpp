//-----------------------------------------------------
// FILE: GroundUnit.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "GroundUnit.h"
#include "GameStateControl.h"
#include "WorldState.h"


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

	// Create shadow
	mpObjShadow = CWorldState::mspMshUnitShadow->CreateModel(mWorldPos.x, 0.4f, mWorldPos.z);

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
		// Update position of shadow
		if (mpObjShadow)
		{
			mpObjShadow->SetPosition(mWorldPos.x, 0.5f, mWorldPos.z);
		}

		// Check if health is below half
		if (((mHealth / mMaxHealth) * 100.0f) <= 50.0f)
		{
			if (mWarningSmoke == nullptr)
			{
				if (mpObjModel)
					mWarningSmoke = new CSmoke(mWorldPos, 0.0f, 0.5f, 2.5f, 2.5f);
			}
		}

		// Check if there is no health left
		if (mHealth <= 0.0f)
		{
			if (mDestructionExplosion == nullptr)
			{
				SafeDelete(mWarningSmoke);
				mDestructionExplosion = new CExplosion(mWorldPos, 20, false);
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

	// First check which airspace this unit is currently in
	DetermineAirspace();

	// Update smoke system
	if (mWarningSmoke != nullptr)
	{
		mWarningSmoke->UpdateSystem(mWorldPos);
	}

	// ALL THESE UPDATES OCCUR IF THE UNIT IS NOT DEAD OR IN SPACE
	if (HasTarget() && !mAttackTarget) //If there is a path target
	{
		UpdateWaypointArrow();
		//Move the unit towards the path target
		LookingAt(mPathTarget); //Rotates the unit to face the path target
		Move();
	}
	else if (mAttackTarget != nullptr) //if there is an attack target
	{
		// Check if target is dead
		if (mAttackTarget->GetHealth() <= 0.0f)
		{
			if (mUnitSelected) mAttackTarget->SetNormalTexture();
			mAttackTarget = nullptr;
		}
		// Check the target is not in space or constructing
		else if (!mAttackTarget->GetModel() || mAttackTarget->GetState() == OBJ_CONSTRUCTING)
		{
			if (mUnitSelected) mAttackTarget->SetNormalTexture();
			mAttackTarget = nullptr;
		}
		// Check which faction the unit is from
		else if (mFaction == FAC_EARTH_DEFENSE_FORCE)
		{
			// Check if the rebels have fled
			if (mAttackTarget->GetWorldXPos() < -2500.0f)
			{
				if (mUnitSelected) mAttackTarget->SetNormalTexture();
				mAttackTarget = nullptr;
			}
		}
		else
		{
			// Check if the rebels have fled
			if (mAttackTarget->GetWorldXPos() > 6000.0f)
			{
				if (mUnitSelected) mAttackTarget->SetNormalTexture();
				mAttackTarget = nullptr;
			}
		}

		// If there is still a target, attack it
		if (mAttackTarget != nullptr)
		{
			Attack(mAttackTarget, 100, mDamage);
		}
	}
	// Check if the unit is out of the grid boundaries to make it return to where it should be *TO DO*
	else if (!mHasPathTarget)
	{
		// Pointer to the player whose airspace is being invaded
		CRTSPlayer* pPlayerAirspace = nullptr;

		// Get player manager
		CPlayerManager* pPManager = CStateControl::GetInstance()->GetPlayerManager();

		// Determine airspace
		if (mAirspace == AS_EARTH) pPlayerAirspace = pPManager->GetHumanPlayer();
		else if (mAirspace == AS_MARS) pPlayerAirspace = pPManager->GetAIPlayer(0);

		if (pPlayerAirspace)
		{
			DX::XMFLOAT3 bottomLeft = pPlayerAirspace->GetPlayerGrid()->GetGridStartPos();
			DX::XMFLOAT3 topRight = pPlayerAirspace->GetPlayerGrid()->GetGridEndPos();

			// Check if the unit is out of the area bounds
			if (mWorldPos.x < bottomLeft.x || mWorldPos.x > topRight.x || mWorldPos.z < bottomLeft.z || mWorldPos.z > topRight.z)
			{
				// Out of the area - send unit back inside the walls
				// Choose random point inside the walls
				float targetX = gpRandomiser->GetRandomFloat(bottomLeft.x, topRight.x);
				float targetZ = gpRandomiser->GetRandomFloat(bottomLeft.z, topRight.z);

				// Move unit to the new target area
				mPathTarget = { targetX, 0.0f, targetZ };
				SetWaypointArrow();
				mHasPathTarget = true;
			}
		}
		// Case for if the airspace is none and this unit has an owner
		else if (mAirspace == AS_NONE && mpOwner)
		{
			// Move based on owner's walls
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
				mPathTarget = { targetX, 0.0f, targetZ };
				SetWaypointArrow();
				mHasPathTarget = true;
			}
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
			DeleteWaypointArrow();
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
	else
	{
		float movement = mSpeed * gFrameTime;
		mpObjModel->MoveLocalZ(movement);
		mWorldPos = DX::XMFLOAT3(mpObjModel->GetX(), mpObjModel->GetY(), mpObjModel->GetZ());
		mBoundingSphere.MoveTo(mWorldPos);
	}
}

void CGroundUnit::DetermineAirspace()
{
	// Calculate which airspace this unit is currently in
	// Get player manager
	CPlayerManager* pPManager = CStateControl::GetInstance()->GetPlayerManager();

	// Pointers to both players
	CRTSPlayer* pHuman = pPManager->GetHumanPlayer();
	CRTSPlayer* pAI = pPManager->GetAIPlayer(0);

	// Check AI player's airspace
	DX::XMFLOAT3 bottomLeft = pAI->GetPlayerGrid()->GetGridStartPos();
	DX::XMFLOAT3 topRight = pAI->GetPlayerGrid()->GetGridEndPos();
	if (mWorldPos.x > bottomLeft.x - 500.0f && mWorldPos.x < topRight.x + 500.0f && mWorldPos.z > bottomLeft.z - 500.0f && mWorldPos.z < topRight.z + 500.0f)
	{
		// Unit is in mars airspace
		mAirspace = AS_MARS;
		return;
	}

	// Check Human player's airspace
	bottomLeft = pHuman->GetPlayerGrid()->GetGridStartPos();
	topRight = pHuman->GetPlayerGrid()->GetGridEndPos();
	if (mWorldPos.x < bottomLeft.x - 500.0f || mWorldPos.x > topRight.x + 500.0f || mWorldPos.z < bottomLeft.z - 500.0f || mWorldPos.z > topRight.z + 500.0f)
	{
		// Unit is in mars airspace
		mAirspace = AS_EARTH;
		return;
	}

	// Unit is in no one's airspace
	AS_NONE;
}
