//-----------------------------------------------------
// FILE: ProductionStructure.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "ProductionStructure.h"


//-----------------------------------------------------
// PRODUCTION STRUCTURE CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CProductionStructure::CProductionStructure()
{

}

CProductionStructure::~CProductionStructure()
{

}


//-----------------------------------------------------
// PRODUCTION STRUCTURE CLASS METHODS
//-----------------------------------------------------
//bool CProductionStructure::AddToQueue(CGameAgent* agent)
bool CProductionStructure::RemoveFromQueue()
{
	return false;
}

bool CProductionStructure::UpdateProduction(float delta)
{
	return false;
}

//CGameAgent* CProductionStructure::CreateAgent()


//-----------------------------------------------------
// PRODUCTION STRUCTURE CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CProductionStructure::Destroy()
{
	return false;
}
