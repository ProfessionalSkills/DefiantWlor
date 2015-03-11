//-----------------------------------------------------
// FILE: Transport.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Transport.h"
IMesh* CTransport::mspMshTransport = nullptr;


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
	mScale = 0.1f;
	mBuildCost = 1400;
	mUnitSpacing = 1.7f;
	
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
	}

	mpObjModel->RotateX(90.0f);

	mpObjModel->Scale(mScale);
}


void CTransport::UnloadIModel()
{
	if (mpObjModel != 0)
	{
		mspMshTransport->RemoveModel(mpObjModel);
		mpObjModel = nullptr;
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