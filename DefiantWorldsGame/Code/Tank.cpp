//-----------------------------------------------------
// FILE: Tank.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Tank.h"

IMesh* CTank::mspMshTank = nullptr;

//-----------------------------------------------------
// TANK CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CTank::CTank()
{
	mAgentInfo = SAgentData(GAV_TANK, "Tank");
	mHealth = 100.0f;
	mSpeed = 1.0f;
	mProductionTime = 2.0f;
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

CTank::~CTank()
{

}

//-----------------------------------------------------
// TANK CLASS METHODS
//-----------------------------------------------------

void CTank::UnloadIModel()
{
	if (mpObjModel != 0)
	{
		mspMshTank->RemoveModel(mpObjModel);
	}
}

//-----------------------------------------------------
// TANK CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CTank::Attack(CGameAgent* target, float hitMod, float damageMod)
{
	return false;
}

//CTank::void MoveTo(CTile* dest)


bool CTank::Move()
{
	return false;
}

bool CTank::Destroy()
{
	return false;
}

IModel* CTank::CreateModel(DX::XMFLOAT3 pos)
{
	return mspMshTank->CreateModel(pos.x, pos.y, pos.z);
}
