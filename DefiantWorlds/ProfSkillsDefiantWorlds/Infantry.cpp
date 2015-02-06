//-----------------------------------------------------
// FILE: Infantry.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Infantry.h"


//-----------------------------------------------------
// INFANTRY CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CInfantry::CInfantry()
{

}

CInfantry::~CInfantry()
{

}


//-----------------------------------------------------
// INFANTRY CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CInfantry::Attack(CGameAgent* target)
{
	return false;
}

//CInfantry::void MoveTo(CTile* dest)


bool CInfantry::Move(float delta)
{
	return false;
}

bool CInfantry::Destroy()
{
	return false;
}
