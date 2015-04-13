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
	mSpeed = 1.0f;
	mProductionTime = 15.0f;
	mProductionCost = 0.0f;
	mCurProductionTimeLeft = mProductionTime;
	mDamage = 100.0f;
	mFireRate = 4.0f;
	mAttackTimer = 1.0f / mFireRate;
	mRange = 150.0f;
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
	DX::XMFLOAT3 localY{ objMatrix.m[1][0], objMatrix.m[1][1], objMatrix.m[1][2] };
	// Normalise this local axis
	DX::XMVECTOR vecNormal = DX::XMVector4Normalize(DX::XMLoadFloat3(&localZ));
	DX::XMStoreFloat3(&localZ, vecNormal);

	vecNormal = DX::XMVector4Normalize(DX::XMLoadFloat3(&localY));
	DX::XMStoreFloat3(&localY, vecNormal);

	// If the target is being looked at and is within range
	if (mAttackTarget->RayCollision(mWorldPos, localZ, distance) && mAttackTarget->RayCollision(mWorldPos, localY, distance) && distance <= (mRange* mRange))
	{
		if (mAttackTimer >= (1.0f / mFireRate)) //Control rate of fire of the unit
		{
			SProjectile* newProjectile = new SProjectile();
			newProjectile->mModel = mspMshFighterBullet->CreateModel(mWorldPos.x, mpObjModel->GetY(), mWorldPos.z);
			newProjectile->mDirection = localZ;
			newProjectile->mSpeed = 800.0f;

			mpProjectiles.push_back(newProjectile);
			mAttackTimer = 0.0f;
		}
	}

	// Do this bit all the time so that if the target is moving, it follows it whilst still firing (for added effect)
	DX::XMFLOAT3 vectorZ = { (target->GetWorldPos().x - mWorldPos.x), 0.0f, (target->GetWorldPos().z - mWorldPos.z) };

	// Normalise vectorZ
	vecNormal = DX::XMVector4Normalize(DX::XMLoadFloat3(&vectorZ));
	DX::XMStoreFloat3(&vectorZ, vecNormal);

	//vecNormal = DX::XMVector4Normalize(DX::XMLoadFloat3(&vectorY));
	//DX::XMStoreFloat3(&vectorY, vecNormal);

	DX::XMFLOAT3 localX = { objMatrix.m[0][0], objMatrix.m[0][1], objMatrix.m[0][2] };
	// Normalise this local axis
	vecNormal = DX::XMVector4Normalize(DX::XMLoadFloat3(&localX));
	DX::XMStoreFloat3(&localX, vecNormal);

	vecNormal = DX::XMVector4Normalize(DX::XMLoadFloat3(&localZ));
	DX::XMStoreFloat3(&localZ, vecNormal);

	float dotProduct = Dot(localX, vectorZ);

	if (dotProduct > 0.001f)
	{
		mpObjModel->RotateY(150.0f * gFrameTime);
	}
	else if (dotProduct < -0.001f)
	{
		mpObjModel->RotateY(-150.0f * gFrameTime);
	}

	dotProduct = Dot(localZ, vectorZ);

	if (dotProduct > 0.001f)
	{
		mpObjModel->RotateLocalX(150.0f * gFrameTime);
	}
	else if (dotProduct < -0.001f)
	{
		mpObjModel->RotateLocalX(-150.0f * gFrameTime);
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
			mpObjModel->RotateY(150.0f * gFrameTime);
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
