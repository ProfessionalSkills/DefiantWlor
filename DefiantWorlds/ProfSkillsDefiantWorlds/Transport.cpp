//-----------------------------------------------------
// FILE: Transport.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Transport.h"


//-----------------------------------------------------
// TRANSPORT CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CTransport::CTransport()
{
	mAgentInfo = SAgentData(GAV_TRANSPORT, "Transport");

	mHealth = 100.0f;
	mSpeed = 1.0f;
	mProductionTime = 25.0f;
	mProductionCost = 0.0f;
	mCurProductionTimeLeft = mProductionTime;
	mDamage = 1.0f;
	//mAttackParticleFX;
	//mDestroyParticleFX;
	mState = OBJ_CONSTRUCTING;
	//mDestGridSq;
	mIsMoving = false;
	mPopCost = 1;
}

CTransport::~CTransport()
{

}


//-----------------------------------------------------
// TRANSPORT CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CTransport::Attack(CGameAgent* target, float hitMod, float damageMod)
{
	return false;
}

void CTransport::Spawn()
{

}

//CTransport::void MoveTo(CTile* dest)


bool CTransport::Move()
{
	return false;
}

bool CTransport::Destroy()
{
	return false;
}

void CTransport::LoadModel(float x, float y, float z)
{
	return;
}
