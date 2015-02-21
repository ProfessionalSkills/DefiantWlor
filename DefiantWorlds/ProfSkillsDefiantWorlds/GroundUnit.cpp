//-----------------------------------------------------
// FILE: GroundUnit.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "GroundUnit.h"


//-----------------------------------------------------
// GROUND UNIT CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CGroundUnit::CGroundUnit()
{

}

CGroundUnit::~CGroundUnit()
{

}


//-----------------------------------------------------
// GROUND UNIT CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CGroundUnit::Attack(CGameAgent* target, float hitMod, float damageMod)
{
	return false;
}

//CGroundUnit::void MoveTo(CTile* dest)

void CGroundUnit::Spawn()
{

}


bool CGroundUnit::Move()
{
	return false;
}

bool CGroundUnit::Destroy()
{
	return false;
}

void CGroundUnit::UnloadIModel()
{

}
