//-----------------------------------------------------
// FILE: AIPlayer.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "AIPlayer.h"


//-----------------------------------------------------
// MENU STATE CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CAIPlayer::CAIPlayer() : CPlayer()
{

}

CAIPlayer::~CAIPlayer()
{

}


//-----------------------------------------------------
// AI PLAYER CLASS METHODS
//-----------------------------------------------------
CStructure* CAIPlayer::CheckStructureSelection(DX::XMFLOAT3 pos)
{
	// Loop through all structures
	for (mpiterStructures = mpStructureList.begin(); mpiterStructures != mpStructureList.end(); mpiterStructures++)
	{
		// If there is a collision, return the pointer to that object
		if ((*mpiterStructures)->PointCollision(pos))
		{
			return (*mpiterStructures);
		}
	}

	return nullptr;
}


//-----------------------------------------------------
// HUMAN PLAYER CLASS OVERRIDE METHODS
//-----------------------------------------------------
void CAIPlayer::Update()
{
	// Loop through all structures & update them
	for (mpiterStructures = mpStructureList.begin(); mpiterStructures != mpStructureList.end(); mpiterStructures++)
	{
		(*mpiterStructures)->Update();
	}
}
