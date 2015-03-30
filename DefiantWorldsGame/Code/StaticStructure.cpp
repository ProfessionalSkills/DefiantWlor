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
bool CStaticStructure::Update(CRTSPlayer* pPlayer)
{
	return true;
}

void CStaticStructure::DisplayInfo(IFont* font)
{

}

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

void CStaticStructure::SaveStructure(std::ofstream& outFile)
{
	// Save to the output file all the required data
	outFile << mStructureType << " " << mGridPos.mPosX << " " << mGridPos.mPosY << " " << mFaction << " " << mState
		<< " " << mWorldPos.x << " " << mWorldPos.y << " " << mWorldPos.z << " " << mHealth << " " << 0 << " ";

	outFile << mOrientation << std::endl;
}
