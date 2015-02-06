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
bool CAirUnit::Attack(CGameAgent* target)
{
	return false;
}

//CAirUnit::void MoveTo(CTile* dest)


bool CAirUnit::Move(float delta)
{
	return false;
}

bool CAirUnit::Destroy()
{
	return false;
}
