//-----------------------------------------------------
// FILE: GameAgent.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "GameAgent.h"


//-----------------------------------------------------
// GAME AGENT CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CGameAgent::CGameAgent()
{

}

CGameAgent::~CGameAgent()
{

}


//-----------------------------------------------------
// GAME AGENT CLASS MUTATORS
//-----------------------------------------------------
bool CGameAgent::TakeDamage(float amount)
{
	mHealth -= amount;
	return false;
}

bool CGameAgent::Heal(float amount)
{
	mHealth += amount;
	return false;
}


//-----------------------------------------------------
// GAME AGENT CLASS METHODS
//-----------------------------------------------------
bool CGameAgent::Construct()
{
	// Reduce construction time left
	mCurProductionTimeLeft -= gFrameTime;

	if (mCurProductionTimeLeft >= 0)
	{
		return false;
	}
	
	return true;
}