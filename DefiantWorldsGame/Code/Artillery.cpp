//-----------------------------------------------------
// FILE: Artillery.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Artillery.h"

IMesh* CArtillery::mspMshArtillery = nullptr;


//-----------------------------------------------------
// ARTILLERY CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CArtillery::CArtillery()
{
	mAgentInfo = SAgentData(GAV_ARTILLERY, "Artillery");
	mHealth = 100.0f;
	mSpeed = 1.0f;
	mProductionTime = 15.0f;
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

CArtillery::~CArtillery()
{
	
}


//-----------------------------------------------------
// ARTILLERY CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CArtillery::Attack(CGameAgent* target, float hitMod, float damageMod)
{
	return false;
}


//CArtillery::void MoveTo(CTile* dest)


bool CArtillery::Move()
{
	return false;
}

bool CArtillery::Destroy()
{
	return false;
}

IModel* CArtillery::CreateModel(DX::XMFLOAT3 pos)
{
	return mspMshArtillery->CreateModel(pos.x, pos.y, pos.z);
}
