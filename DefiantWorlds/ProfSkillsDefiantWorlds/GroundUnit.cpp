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
bool CGroundUnit::Attack(CGameAgent* target)
{
	return false;
}

//CGroundUnit::void MoveTo(CTile* dest)


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
