//-----------------------------------------------------
// FILE: SpaceUnit.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "SpaceUnit.h"


//-----------------------------------------------------
// SPACE UNIT CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CSpaceUnit::CSpaceUnit()
{

}

CSpaceUnit::~CSpaceUnit()
{

}


//-----------------------------------------------------
// SPACE UNIT CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CSpaceUnit::Attack(CGameAgent* target)
{
	return false;
}

//CSpaceUnit::void MoveTo(CTile* dest)


bool CSpaceUnit::Move()
{
	return false;
}

bool CSpaceUnit::Destroy()
{
	return false;
}
