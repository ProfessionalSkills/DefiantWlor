//-----------------------------------------------------
// FILE: Tank.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Tank.h"


//-----------------------------------------------------
// TANK CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CTank::CTank()
{

}

CTank::~CTank()
{

}


//-----------------------------------------------------
// TANK CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CTank::Attack(CGameAgent* target)
{
	return false;
}

//CTank::void MoveTo(CTile* dest)


bool CTank::Move(float delta)
{
	return false;
}

bool CTank::Destroy()
{
	return false;
}
