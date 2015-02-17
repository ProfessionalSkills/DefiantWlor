//-----------------------------------------------------
// FILE: Artillery.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Artillery.h"


//-----------------------------------------------------
// ARTILLERY CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CArtillery::CArtillery()
{
	mAgentInfo = SAgentData(GAV_ARTILLERY, "Artillery");
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
