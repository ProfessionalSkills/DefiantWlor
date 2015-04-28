//-----------------------------------------------------
// FILE: Mothership.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Mothership.h"
#include "GameStateControl.h"

IMesh* CMothership::mspMshMothership = nullptr;
IMesh* CMothership::mspMshSheild = nullptr;
ALuint CMothership::alAttackSound = 0;
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

bool CMothership::StoreUnits(CGameAgent* unit)
{
	return false;
}

void CMothership::LoadModel(float x, float y, float z)
{
	mOrientation = 180.0f;

	mpObjModel = mspMshMothership->CreateModel(x, y, z);
	mWorldPos.x = x;
	mWorldPos.y = y;
	mWorldPos.z = z;

	mChargeTime = mChargeTimeMax;
	mChargingLazers = false;
	mCharged = false;
	mFiredLazer = false;
	ALuint alExplosionBuffer = alutCreateBufferFromFile("SpaceShip_Explosion.wav");
	DX::XMFLOAT3 mSourcePos = { mWorldPos.x, mWorldPos.y, mWorldPos.z };
	DX::XMFLOAT3 mSourceVel = { 0.0F, 0.0f, 0.0f };
	DX::XMFLOAT3 listenerPos = { 0.0f, 0.0f, -150.0f };
	DX::XMFLOAT3 listenerVel = { 0.0f, 0.0f, 0.0f };
	if (mWorldPos.x<0.0f)  mSourceVel.x = { 100.0f };
	else  mSourceVel.x = { -100.0f };
	float volume = CStateControl::GetInstance()->GetSettingsManager()->GetEffectsVolume();		// MAKE SURE TO INCLUDE GameStateControl in the

	// .cpp file ONLY otherwise you'll get cyclic redundancy
	mGenSound = new CSound(alAttackSound, mSourcePos, mSourceVel, false, volume, listenerPos, listenerVel);
	mSoundExplosion = new CSound(alExplosionBuffer, mSourcePos, mSourceVel, false, volume, listenerPos, listenerVel);

	if (mWorldPos.x < 0)
	{
		mpObjModel->RotateY(-90.0f);
	}
	else
	{
		mpObjModel->RotateY(90.0f);
	}
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

