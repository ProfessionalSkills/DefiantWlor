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
	mDamage = 80.0f;
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
	mHasPathTarget = false;
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
		mpTempShield->RotateX(-35.0f);
	}
	else
	{
		mpTempShield->SetPosition(mWorldPos.x, mWorldPos.y, mWorldPos.z);
	}
}

void CMothership::UnloadFlash()
{
	// If there is a shield, remove it
	if (mpTempShield)
	{
		mpTempShield->SetY(-5000);
	}
	UnloadLazer();
}


//-----------------------------------------------------
// MOTHERSHIP CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CMothership::Attack(CGameObject* target, float hitMod, float damageMod)
{
	if (mpToHitRoll->GetRandomFloat(1.0, 100.0) < (hitMod*mHitChance) * 100)
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
	if (mpTempLazer != 0)
	{
		mspMshLazer->RemoveModel(mpTempLazer);
		mpTempLazer = 0;
	}
}

bool CMothership::Destroy()
{
	return false;
}

void CMothership::MoveY(float yChange)
{
	mWorldPos.y += yChange;
	mpObjModel->SetY(mWorldPos.y);
}

