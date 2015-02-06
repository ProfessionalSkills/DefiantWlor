//-----------------------------------------------------
// FILE: SpaceFighter.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "SpaceFighter.h"


//-----------------------------------------------------
// SPACE FIGHTER CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CSpaceFighter::CSpaceFighter()
{

}

CSpaceFighter::~CSpaceFighter()
{

}


//-----------------------------------------------------
// SPACE FIGHTER CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CSpaceFighter::Attack(CGameAgent* target)
{
	return false;
}

//CSpaceFighter::void MoveTo(CTile* dest)


bool CSpaceFighter::Move(float delta)
{
	return false;
}

bool CSpaceFighter::Destroy()
{
	return false;
}
