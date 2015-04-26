//-----------------------------------------------------
// FILE: Turret.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "AATurret.h"

IMesh* CTurretStructure::mspMshTurret = nullptr;
IMesh* CTurretStructure::mspMshTurretShell = nullptr;


//-----------------------------------------------------
// STATIC STRUCTURE CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CTurretStructure::CTurretStructure(DX::XMFLOAT3 position)
{
	mpObjModel = mspMshTurret->CreateModel(position.x, 73.0f, position.z); //Position Turret at top of tower
	mpObjModel->SetSkin("bld-mt.jpg");
	mpObjModel->Scale(5.0f);
	mHealth = 150.0f;
	mRange = 150.0f;
	mWorldPos = { position.x, 73.0f, position.z };
	mState = OBJ_BUILT;
	mMaxHealth = 150.0f;
	mDamage = 150.0f;
	mAttackTimer = 3.0f;
	mFireRate = 0.3f;
	mStructureType = STR_AA;
	mHeight = 6.0f;

	mIsGroundType = true;

	CalculateBoundingBox();
}

CTurretStructure::~CTurretStructure()
{

}


//-----------------------------------------------------
// STATIC STRUCTURE CLASS OVERRIDE METHODS
//-----------------------------------------------------
void CTurretStructure::CalculateBoundingBox()
{
	mStructureBL = SPointData(-6, -6);
	mStructureTR = SPointData(6, 6);

	float top = mWorldPos.z + ((float)mStructureTR.mPosY);
	float bottom = mWorldPos.z + ((float)mStructureBL.mPosY);
	float right = mWorldPos.x + ((float)mStructureTR.mPosX);
	float left = mWorldPos.x + ((float)mStructureBL.mPosX);
	mBoundingBox = SBoundingCube(DX::XMFLOAT3(left, 0.0f, bottom), DX::XMFLOAT3(right, mWorldPos.y + mHeight, top));
}

bool CTurretStructure::Update(CRTSPlayer* pPlayer)
{
	// Determine state of the structure
	switch (mState)
	{
	case OBJ_CONSTRUCTING:
		// Change status of the building to 'built'
		mState = OBJ_BUILT;

		// Object still alive
		return true;

		break;

	case OBJ_BUILT:
		// Check if health below 50% for smoke system creation
		if (mHealth > 0.0f && ((mHealth / mMaxHealth) * 100.0f) <= 50.0f)
		{
			if (mWarningSmoke == nullptr)
			{
				mWarningSmoke = new CSmoke(mWorldPos, mHeight, 0.8f, mBoundingBox.GetWidthExtent() / 3.0f, mBoundingBox.GetLengthExtent() / 3.0f);
			}
			else
			{
				// Update smoke system
				mWarningSmoke->UpdateSystem();
			}
		}

		// Check if no health left
		if ((mHealth <= 0.0f))
		{
			if (mDestructionExplosion == nullptr)
			{
				SafeDelete(mWarningSmoke);
				mDestructionExplosion = new CExplosion({ mWorldPos.x, mWorldPos.y + 20.0f, mWorldPos.z }, 150, false);
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
		}

		if (mAttackTarget != nullptr) //If the turret has a target
		{
			// Check if target is dead
 			if (mAttackTarget->GetHealth() <= 0.0f)
			{
				mAttackTarget = nullptr;
			}
			// Check the target is not in space or constructing
			else if (!mAttackTarget->GetModel() || mAttackTarget->GetState() == OBJ_CONSTRUCTING)
			{
				mAttackTarget = nullptr;
			}
			else
			{
				//Get the distance between target and turret
				DX::XMFLOAT3 targetPos = mAttackTarget->GetWorldPos();
				float distX = mWorldPos.x - targetPos.x;
				float distZ = mWorldPos.z - targetPos.z;

				float distance = (distX * distX) + (distZ * distZ);

				//If the target is out of range, remove it as a target
				if (distance > (mRange * mRange))
				{
					mAttackTarget = nullptr;
				}
			}

			// If there is still a target, attack it
			if (mAttackTarget != nullptr)
			{
				Attack(mAttackTarget, 100, mDamage);
			}
		}

		break;

	case OBJ_DEAD:

		// Object no longer alive
		return false;

		break;
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
				SafeDelete(projectile);
				mpProjectiles.erase(iter);
				break;
			}
			else if (mAttackTarget->SphereCollision(projectile->mCollisionSphere)) //Point to Box collision between the projectile and the attack target
			{
				mAttackTarget->TakeDamage(mDamage);
				mpAttackExplosions.push_back(new CExplosion(position, 25, false));
				SafeDelete(projectile);
				mpProjectiles.erase(iter);
				break; //Breaks out of the loop as the vector size has been changed, comprimising the iterator loop
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

	return true;
}

void CTurretStructure::DisplayInfo(IFont* font)
{
	// Output selected building
	font->Draw("Anti-Air Turret", 610, 825, 0xff002930, kLeft, kTop);
}

EErrorTypes CTurretStructure::AddToQueue(size_t agentIndex, CRTSPlayer* pPlayer)
{
	return ERR_NONE;
}

void CTurretStructure::UnloadIModel()
{

}

void CTurretStructure::LoadIModel()
{

}

void CTurretStructure::SaveStructure(std::ofstream& outFile)
{
	// Save to the output file all the required data
	outFile << mStructureType << " " << mGridPos.mPosX << " " << mGridPos.mPosY << " " << mFaction << " " << mState
		<< " " << mWorldPos.x << " " << mWorldPos.y << " " << mWorldPos.z << " " << mHealth << " " << 0 << " ";

	outFile << mOrientation << std::endl;
}

void CTurretStructure::LoadStructure(std::ifstream& inFile, CGrid* pGrid, CRTSPlayer* pPlayer)
{
	// Ensure no model is already loaded for it
	UnloadIModel();

	// Store the required data for the structure
	int faction;
	int state;
	int pad;
	inFile >> mGridPos.mPosX >> mGridPos.mPosY >> faction >> state >> mWorldPos.x >> mWorldPos.y
		>> mWorldPos.z >> mHealth >> pad >> mOrientation;

	// Convert required values to enums
	mFaction = static_cast<EFactions>(faction);
	mState = static_cast<EObjectStates>(state);

	// Load the 3D model
	LoadIModel();
	CalculateBoundingBox();
}

bool CTurretStructure::Attack(CGameObject* pTarget, float hitMod, float damageMod)
{
	// The RayCollision function calculates this value for us - so it needs no starting value. Only to be defined.
	float distance;

	// Get the local Z axis of the turret
	DX::XMFLOAT4X4 objMatrix;
	DX::XMFLOAT4X4 turMatrix;
	mpObjModel->GetMatrix(&objMatrix.m[0][0]);
	mpObjModel->GetNode(2)->GetMatrix(&turMatrix.m[0][0]);

	// As the matrix of the turret is RELATIVE to the base model matrix, you have to multiply them together to get the ACTUAL matrix for the turret
	DX::XMFLOAT4X4 finalMatrix;
	DX::XMMATRIX matMul = DX::XMMatrixMultiply(DX::XMLoadFloat4x4(&objMatrix), DX::XMLoadFloat4x4(&turMatrix));
	DX::XMStoreFloat4x4(&finalMatrix, matMul);

	DX::XMFLOAT3 localZ{ finalMatrix.m[2][0], finalMatrix.m[2][1], finalMatrix.m[2][2] };

	DX::XMFLOAT3 target = mAttackTarget->GetWorldPos();

	// Normalise this local axis
	DX::XMVECTOR vecNormal = DX::XMVector4Normalize(DX::XMLoadFloat3(&localZ));
	DX::XMStoreFloat3(&localZ, vecNormal);
	DX::XMFLOAT3 pos = { mWorldPos.x, 75.0f, mWorldPos.z };

	// If the target is being looked at and is within range
	if (mAttackTarget->RayCollision(mWorldPos, localZ, distance) && distance <= mRange)
	{
		if (mAttackTimer >= (1.0f / mFireRate)) //Control rate of fire of the unit
		{
			SProjectile* newProjectile = new SProjectile();
			newProjectile->mModel = mspMshTurretShell->CreateModel(mWorldPos.x, 75.0f, mWorldPos.z);
			newProjectile->mDirection = localZ;
			newProjectile->mSpeed = 700.0f;
			newProjectile->mLifeTime = 3.0f;

			mpProjectiles.push_back(newProjectile);
			mAttackTimer = 0.0f;
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
		mpObjModel->GetNode(2)->RotateY(250.0f * gFrameTime);
	}
	else if (dotProduct < -0.001f)
	{
		mpObjModel->GetNode(2)->RotateY(-250.0f * gFrameTime);
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
			mpObjModel->GetNode(2)->RotateY(20.0f * gFrameTime);
		}
	}

	// Increment attack timer
	mAttackTimer += gFrameTime;
	return true;
}
