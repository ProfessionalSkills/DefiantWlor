//-----------------------------------------------------
// FILE: Bomber.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Bomber.h"


//-----------------------------------------------------
// BOMBER CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CBomber::CBomber()
{

}

CBomber::~CBomber()
{

}


//-----------------------------------------------------
// BOMBER CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CBomber::Attack(CGameAgent* target)
{
	return false;
}

//CBomber::void MoveTo(CTile* dest)


bool CBomber::Move(float delta)
{
	return false;
}

bool CBomber::Destroy()
{
	return false;
}
