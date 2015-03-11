//-----------------------------------------------------
// FILE: SpaceFighter.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "SpaceFighter.h"
IMesh* CSpaceFighter::mspMshSpaceFighter = nullptr;
IMesh* CSpaceFighter::mspMshSheild = nullptr;

//-----------------------------------------------------
// SPACE FIGHTER CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CSpaceFighter::CSpaceFighter()
{
	mAgentInfo = SAgentData(GAV_SPACE_FIGHTER, "Space Fighter");

	//Game Values
	mMaxHealth = 100.0f;
	mHealth = 100.0f;
	mSpeed = 250.0f;
	mDamage = 1.0f;
	mPopCost = 1;
	mHitChance = 0.7f;
	mFleetPosition = front;

	//Production Values
	mProductionTime = 15.0f;
	mProductionCost = 0.0f;
	mCurProductionTimeLeft = mProductionTime;

	//Model Values
	mScale = 0.3f;
	mBuildCost = 1000;
	mUnitSpacing = 4.0f;

	//Misc
	//mAttackParticleFX;
	//mDestroyParticleFX;
	mState = OBJ_CONSTRUCTING;
	//mDestGridSq;
	mIsMoving = false;

	// Sound

}

CSpaceFighter::~CSpaceFighter()
{
	UnloadIModel();
}

//-----------------------------------------------------
// SPACE FIGHTER CLASS METHODS
//-----------------------------------------------------
void CSpaceFighter::LoadModel(float x,float y, float z)
{
	mpObjModel = mspMshSpaceFighter->CreateModel(x, y, z);
	mWorldPos.x = x;
	mWorldPos.y = y;
	mWorldPos.z = z;
	string mMusicFile = "shoot.wav";
	DX::XMFLOAT3 mSourcePos = { mWorldPos.x, mWorldPos.y, mWorldPos.z };
	DX::XMFLOAT3 mSourceVel = { 0.0F, 0.0f, 0.0f };
	DX::XMFLOAT3 listenerPos = { 0.0f, 0.0f, -30.0f };
	DX::XMFLOAT3 listenerVel = { 0.0f, 0.0f, 0.0f };
	if (mWorldPos.x<0.0f)  mSourceVel.x = { 100.0f};
	else  mSourceVel.x = {-100.0f};
	mGenSound = new CSound(mMusicFile, mSourcePos, mSourceVel, false, 0.2f, listenerPos, listenerVel);

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

void CSpaceFighter::UnloadIModel()
{
	if (mpObjModel != 0)
	{
		mspMshSpaceFighter->RemoveModel(mpObjModel);
		mpObjModel = nullptr;
	}
	if (mpTempShield != 0)
	{
		mspMshSheild->RemoveModel(mpTempShield);
		mpTempShield = nullptr;
	}
}

//-----------------------------------------------------
// SPACE FIGHTER CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CSpaceFighter::Attack(CGameAgent* target, float hitMod, float damageMod)
{
	CRandomiser toHitRoll;
	if (toHitRoll.GetRandomFloat(1.0,100.0) < (hitMod*mHitChance) * 100)
	{
		target->TakeDamage(mDamage*damageMod);
		mGenSound->PlaySound();
		CSpaceUnit* mpTemp = (CSpaceUnit*)(target);
		mpTemp->HitFlash();
		return true;
	}
	return false;
}

void CSpaceFighter::Spawn(CGrid* pGrid, SPointData pCentre)
{

}

void CSpaceFighter::HitFlash()
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
			mSpeed = -mSpeed;
		}

		mpTempShield->RotateX(-35.0f);
	}
}

void CSpaceFighter::UnloadFlash()
{
	// If there is a shield, remove it
	if (mpTempShield)
	{
		mspMshSheild->RemoveModel(mpTempShield);
		mpTempShield = nullptr;
	}
}

//CSpaceFighter::void MoveTo(CTile* dest)


bool CSpaceFighter::Move()
{
	mpObjModel->MoveX(mSpeed*gFrameTime);
	mWorldPos.x += mSpeed*gFrameTime;
	return false;
}

bool CSpaceFighter::Destroy()
{
	return false;
}
