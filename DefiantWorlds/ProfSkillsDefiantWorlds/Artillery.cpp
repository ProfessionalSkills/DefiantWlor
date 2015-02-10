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
	
}

CArtillery::~CArtillery()
{
	
}


//-----------------------------------------------------
// ARTILLERY CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CArtillery::Attack(CGameAgent* target)
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
