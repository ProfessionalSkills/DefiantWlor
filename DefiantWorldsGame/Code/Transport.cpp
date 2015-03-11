//-----------------------------------------------------
// FILE: Transport.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Transport.h"
IMesh* CTransport::mspMshTransport = nullptr;
IMesh* CTransport::mspMshSheild = nullptr;

//-----------------------------------------------------
// TRANSPORT CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CTransport::CTransport()
{
	//Game Values
	mMaxHealth = 100.0f;
	mHealth = 100.0f;
	mDamage = 0.5f;
	mHitChance = 0.4f;
	mFleetPosition = centre;
	mPopCost = 1;	

	//Production Values
	mProductionTime = 15.5f;
	mProductionCost = 0.0f;
	mCurProductionTimeLeft = mProductionTime;
	
	//Model Values
	mScale = 0.2f;
	mBuildCost = 1400;
	mUnitSpacing = 0.1f*(10*mScale);
	
	//Misc
	//mAttackParticleFX;
	//mDestroyParticleFX;
	mState = OBJ_CONSTRUCTING;
	//mDestGridSq;
	mIsMoving = false;
	hasTarget = false;
	mAgentInfo = SAgentData(GAV_TRANSPORT, "Transport");


}

CTransport::~CTransport()
{
	UnloadIModel();
}

//-----------------------------------------------------
// TRANSPORT CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CTransport::Attack(CGameAgent* target, float hitMod, float damageMod)
{
	CRandomiser toHitRoll;
	if (toHitRoll.GetRandomFloat(1.0, 100.0) < (hitMod*mHitChance) * 100)
	{
		target->TakeDamage(mDamage*damageMod);
		return true;
	}
	return false;
}

void CTransport::Spawn(CGrid* pGrid, SPointData pCentre)
{

}

float CTransport::GetUnitSpacing()
{
	return mUnitSpacing;
}

void CTransport::HitFlash()
{
	if (!mpTempShield)
	{
		mpTempShield = mspMshSheild->CreateModel(mWorldPos.x, mWorldPos.y, mWorldPos.z);
		mpTempShield->Scale(mScale + 0.03f);

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

void CTransport::UnloadFlash()
{
	// If there is a shield, remove it
	if (mpTempShield)
	{
		mspMshSheild->RemoveModel(mpTempShield);
		mpTempShield = nullptr;
	}
}

void CTransport::LoadModel(float x, float y, float z)
{
	mpObjModel = mspMshTransport->CreateModel(x, y, z);

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


void CTransport::UnloadIModel()
{
	if (mpObjModel != 0)
	{
		mspMshTransport->RemoveModel(mpObjModel);
		mpObjModel = nullptr;
	}
	if (mpTempShield != 0)
	{
		mspMshSheild->RemoveModel(mpTempShield);
		mpTempShield = nullptr;
	}
}

bool CTransport::Move()
{
	mpObjModel->MoveX(mSpeed*gFrameTime);
	mWorldPos.x += mSpeed*gFrameTime;
	return false;
}

bool CTransport::Destroy()
{
	return false;
}