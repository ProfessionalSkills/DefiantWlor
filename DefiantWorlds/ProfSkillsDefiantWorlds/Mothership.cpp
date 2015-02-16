//-----------------------------------------------------
// FILE: Mothership.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Mothership.h"


//-----------------------------------------------------
// MOTHERSHIP CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CMothership::CMothership()
{
	mAgentInfo = SAgentData(GAV_MOTHERSHIP, "Mothership");
}

CMothership::~CMothership()
{

}


//-----------------------------------------------------
// MOTHERSHIP CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CMothership::Attack(CGameAgent* target)
{
	return false;
}

//CMothership::void MoveTo(CTile* dest)


bool CMothership::Move()
{
	return false;
}

bool CMothership::Destroy()
{
	return false;
}
