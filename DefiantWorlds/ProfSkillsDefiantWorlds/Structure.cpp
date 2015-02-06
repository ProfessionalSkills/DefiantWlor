//-----------------------------------------------------
// FILE: Structure.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Structure.h"


//-----------------------------------------------------
// STRUCTURE CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CStructure::CStructure()
{

}

CStructure::~CStructure()
{

}


//-----------------------------------------------------
// STRUCTURE CLASS MUTATORS
//-----------------------------------------------------
bool CStructure::TakeDamage(float amount)
{
	return false;
}

bool CStructure::Repair(float amount)
{
	return false;
}


//-----------------------------------------------------
// STRUCTURE CLASS METHODS
//-----------------------------------------------------
void CStructure::CreateStructure(DX::XMFLOAT3 pos)
{

}

void CStructure::CreateStructure(SPointData pos)
{

}

bool CStructure::Build(float delta)
{
	// Update build timer
	// Simply scale y (proportionally to building height)
	// Reset scale then set new scale proportional to how far through building time it is
	return false;
}
