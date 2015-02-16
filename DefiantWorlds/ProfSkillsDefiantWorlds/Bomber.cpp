//-----------------------------------------------------
// FILE: Bomber.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Bomber.h"


//-----------------------------------------------------
// BOMBER CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CBomber::CBomber()
{
	mAgentInfo = SAgentData(GAV_BOMBER, "Bomber");

	mHealth;
	mSpeed;
	mProductionTime;
	mProductionCost;
	mCurProductionTimeLeft;				
	mDamage;
	//mAttackParticleFX;
	//mDestroyParticleFX;
	mState;
	//mDestGridSq;
	mIsMoving;
	mPopCost;
}

CBomber::~CBomber()
{

}


//-----------------------------------------------------
// BOMBER CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CBomber::Attack(CGameAgent* target)
{
	return false;
}

//CBomber::void MoveTo(CTile* dest)


bool CBomber::Move()
{
	return false;
}

bool CBomber::Destroy()
{
	return false;
}
