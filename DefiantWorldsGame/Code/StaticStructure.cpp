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
EErrorTypes CStaticStructure::AddToQueue(size_t agentIndex, CRTSPlayer* pPlayer)
{
	return ERR_NONE;
}

void CStaticStructure::UnloadIModel()
{

}
void CStaticStructure::LoadIModel()
{

}