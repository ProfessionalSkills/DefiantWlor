//-----------------------------------------------------
// FILE: SpaceFighter.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "SpaceFighter.h"


//-----------------------------------------------------
// SPACE FIGHTER CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CSpaceFighter::CSpaceFighter()
{
	mAgentInfo = SAgentData(GAV_SPACE_FIGHTER, "Space Fighter");
}

CSpaceFighter::~CSpaceFighter()
{

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
		return true;
	}
	return false;
}

//CSpaceFighter::void MoveTo(CTile* dest)


bool CSpaceFighter::Move()
{
	return false;
}

bool CSpaceFighter::Destroy()
{
	return false;
}
