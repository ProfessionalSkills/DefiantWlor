//-----------------------------------------------------
// FILE: AirUnit.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "AirUnit.h"


//-----------------------------------------------------
// AIR UNIT CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CAirUnit::CAirUnit()
{

}

CAirUnit::~CAirUnit()
{

}


//-----------------------------------------------------
// AIR UNIT CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CAirUnit::Attack(CGameAgent* target, float hitMod, float damageMod)
{
	return false;
}

void CAirUnit::Spawn()
{

}

//CAirUnit::void MoveTo(CTile* dest)


bool CAirUnit::Move()
{
	return false;
}

bool CAirUnit::Destroy()
{
	return false;
}

void CAirUnit::UnloadIModel()
{

}
