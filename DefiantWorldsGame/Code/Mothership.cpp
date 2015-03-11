//-----------------------------------------------------
// FILE: Mothership.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Mothership.h"
IMesh* CMothership::mspMshMothership = nullptr;
IMesh* CMothership::mspMshSheild = nullptr;
//-----------------------------------------------------
// MOTHERSHIP CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CMothership::CMothership()
{
	mAgentInfo = SAgentData(GAV_MOTHERSHIP, "Mothership");

	//Game Values
	mMaxHealth = 100.0f;
	mHealth = 100.0f;
	mDamage = 4.0f;
	mHitChance = 0.5f;
	mFleetPosition = back;

	//Production Values
	mProductionTime = 15.0f;
	mProductionCost = 0.0f;
	mCurProductionTimeLeft = mProductionTime;
	mPopCost = 10;

	//Model Values
	mScale = 2.0f;
	mBuildCost = 2000;
	mUnitSpacing = 0.5f*(mScale*10);

	//Misc
	//mAttackParticleFX;
	//mDestroyParticleFX;
	mState = OBJ_CONSTRUCTING;
	//mDestGridSq;
	mIsMoving = false;
	hasTarget = false;
}

CMothership::~CMothership()
{
	UnloadIModel();
}

void CMothership::HitFlash()
{
	if (!mpTempShield)
	{
		mpTempShield = mspMshSheild->CreateModel(mWorldPos.x, mWorldPos.y, mWorldPos.z);
		mpTempShield->Scale(mScale + 0.1f);

		if (mWorldPos.x < 0.0f)
		{
			mpTempShield->RotateY(90.0f);
		}
		else
		{
			mpTempShield->RotateY(-90.0f);
		}

		mpTempShield->RotateX(-35.0f);
	}
}

void CMothership::UnloadFlash()
{
	// If there is a shield, remove it
	if (mpTempShield)
	{
		mspMshSheild->RemoveModel(mpTempShield);
		mpTempShield = nullptr;
	}
	UnloadLazer();
}


//-----------------------------------------------------
// MOTHERSHIP CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CMothership::Attack(CGameAgent* target, float hitMod, float damageMod)
{
	CRandomiser toHitRoll;
	if (toHitRoll.GetRandomFloat(1.0, 100.0) < (hitMod*mHitChance) * 100)
	{
		target->TakeDamage(mDamage*damageMod);
		FireLazer(target);
		CSpaceUnit* mpTemp = (CSpaceUnit*)(target);
		mpTemp->HitFlash();
		return true;
	}
	return false;
}
void CMothership::Spawn(CGrid* pGrid, SPointData pCentre)
{

}

float CMothership::GetUnitSpacing()
{
	return mUnitSpacing;
}

void CMothership::LoadModel(float x, float y, float z)
{
	mpObjModel = mspMshMothership->CreateModel(x, y, z);
	mWorldPos.x = x;
	mWorldPos.y = y;
	mWorldPos.z = z;

	if (x < 0.0f)
	{
		mpObjModel->RotateY(90.0f);
	}
	else
	{
		mpObjModel->RotateY(-90.0f);
		mSpeed = -mSpeed;
	}

	mpObjModel->RotateX(-35.0f);
	mpObjModel->Scale(mScale);
}

void CMothership::UnloadIModel()
{
	if (mpObjModel != 0)
	{
		mspMshMothership->RemoveModel(mpObjModel);
		mpObjModel = nullptr;
	}
	if (mpTempShield != 0)
	{
		mspMshSheild->RemoveModel(mpTempShield);
		mpTempShield = nullptr;
	}
	UnloadLazer();
}



//CMothership::void MoveTo(CTile* dest)


bool CMothership::Move()
{
	mpObjModel->MoveX(mSpeed*gFrameTime);
	mWorldPos.x += mSpeed*gFrameTime;
	return false;
}

bool CMothership::Destroy()
{
	return false;
}

