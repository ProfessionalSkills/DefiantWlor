//-----------------------------------------------------
// FILE: Mothership.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Mothership.h"
IMesh* CMothership::mspMshMothership = nullptr;

//-----------------------------------------------------
// MOTHERSHIP CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CMothership::CMothership()
{
	mAgentInfo = SAgentData(GAV_MOTHERSHIP, "Mothership");

	mHealth = 100.0f;
	mSpeed = 1.0f;
	mProductionTime = 40.0f;
	mProductionCost = 0.0f;
	mCurProductionTimeLeft = mProductionTime;
	mDamage = 4.0f;
	//mAttackParticleFX;
	//mDestroyParticleFX;
	mState = OBJ_CONSTRUCTING;
	//mDestGridSq;
	mIsMoving = false;
	mPopCost = 1;
	mHitChance = 0.5f;
	mScale = 0.4f;
}

CMothership::~CMothership()
{

}


//-----------------------------------------------------
// MOTHERSHIP CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CMothership::Attack(CGameAgent* target, float hitMod, float damageMod)
{
	return false;
}
void CMothership::Spawn(CGrid* pGrid, SPointData pCentre)
{

}

void CMothership::LoadModel(float x, float y, float z)
{
	mpObjModel = mspMshMothership->CreateModel(x, y, z);
	//mpObjModel->SetSkin("Spaceship02Battlecruiser.jpg");
	if (x < 0.0f)
	{
		mpObjModel->RotateLocalY(90.0f);
	}
	else
	{
		mpObjModel->RotateLocalY(-90.0f);
	}
	mpObjModel->Scale(mScale);
}


//CMothership::void MoveTo(CTile* dest)


bool CMothership::Move()
{
	return false;
}

bool CMothership::Destroy()
{
	return false;
}

