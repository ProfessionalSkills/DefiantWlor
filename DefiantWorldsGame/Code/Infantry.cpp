//-----------------------------------------------------
// FILE: Infantry.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Infantry.h"

IMesh* CInfantry::mspMshInfantry = nullptr;


//-----------------------------------------------------
// INFANTRY CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CInfantry::CInfantry()
{
	mAgentInfo = SAgentData(GAV_INFANTRY, "Infantry");
	mHealth = 100.0f;
	mSpeed = 1.0f;
	mProductionTime = 10.0f;
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

CInfantry::~CInfantry()
{

}

//-----------------------------------------------------
// INFANTRY CLASS METHODS
//-----------------------------------------------------

void CInfantry::UnloadIModel()
{
	if (mpObjModel != 0)
	{
		mspMshInfantry->RemoveModel(mpObjModel);
		mpObjModel = nullptr;
	}
}


//-----------------------------------------------------
// INFANTRY CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CInfantry::Attack(CGameAgent* target, float hitMod, float damageMod)
{
	return false;
}

//CInfantry::void MoveTo(CTile* dest)


bool CInfantry::Move()
{
	return false;
}

bool CInfantry::Destroy()
{
	return false;
}

IModel* CInfantry::CreateModel(DX::XMFLOAT3 pos)
{
	return mspMshInfantry->CreateModel(pos.x, pos.y, pos.z);
}
