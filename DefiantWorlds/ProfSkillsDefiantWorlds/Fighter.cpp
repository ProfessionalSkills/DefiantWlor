//-----------------------------------------------------
// FILE: Fighter.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Fighter.h"


//-----------------------------------------------------
// FIGHTER CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CFighter::CFighter()
{

}

CFighter::~CFighter()
{

}


//-----------------------------------------------------
// FIGHTER CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CFighter::Attack(CGameAgent* target)
{
	return false;
}

//CFighter::void MoveTo(CTile* dest)


bool CFighter::Move(float delta)
{
	return false;
}

bool CFighter::Destroy()
{
	return false;
}
