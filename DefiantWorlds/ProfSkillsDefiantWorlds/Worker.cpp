//-----------------------------------------------------
// FILE: Worker.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Worker.h"


//-----------------------------------------------------
// WORKER CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CWorker::CWorker()
{
	mAgentInfo = SAgentData(GAV_WORKER, "Worker");

	mHealth = 100.0f;
	mSpeed = 1.0f;
	mProductionTime = 5.0f;
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

CWorker::~CWorker()
{

}


//-----------------------------------------------------
// WORKER CLASS METHODS
//-----------------------------------------------------
//bool RepairBuilding(CStructure* structure)

//bool CollectResources(CResources* resource)

bool RepairUnit(CGroundUnit* unit)
{
	return false;
}


//-----------------------------------------------------
// WORKER CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CWorker::Attack(CGameAgent* target, float hitMod, float damageMod)
{
	return false;
}

void CWorker::Spawn()
{

}

//CWorker::void MoveTo(CTile* dest)


bool CWorker::Move()
{
	return false;
}

bool CWorker::Destroy()
{
	return false;
}
