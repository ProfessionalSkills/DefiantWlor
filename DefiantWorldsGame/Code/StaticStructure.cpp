//-----------------------------------------------------
// FILE: StaticStructure.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "StaticStructure.h"


//-----------------------------------------------------
// STATIC STRUCTURE CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CStaticStructure::CStaticStructure()
{

}

CStaticStructure::~CStaticStructure()
{

}


//-----------------------------------------------------
// STATIC STRUCTURE CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CStaticStructure::AddToQueue(size_t agentIndex, CRTSPlayer* pPlayer)
{
	return false;
}

void CStaticStructure::UnloadIModel()
{

}
void CStaticStructure::LoadIModel()
{

}