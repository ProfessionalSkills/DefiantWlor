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
// SPACE UNIT CLASS ACCESSOR METHODS
//-----------------------------------------------------


//-----------------------------------------------------
// SPACE UNIT CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CSpaceUnit::Attack(CGameAgent* target, float hitMod, float damageMod)
{
	return false;
}

void CSpaceUnit::Spawn(CGrid* pGrid, SPointData pCentre)
{

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

void CSpaceUnit::UnloadIModel()
{

}

void CSpaceUnit::LoadIModel()
{

}

void CSpaceUnit::LoadModel(float x, float y, float z)
{

}