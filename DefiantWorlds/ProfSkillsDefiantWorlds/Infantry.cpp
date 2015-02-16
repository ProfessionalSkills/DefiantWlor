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
	mAgentInfo = SAgentData(GAV_INFANTRY, "Infantry");
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


bool CInfantry::Move()
{
	return false;
}

bool CInfantry::Destroy()
{
	return false;
}
