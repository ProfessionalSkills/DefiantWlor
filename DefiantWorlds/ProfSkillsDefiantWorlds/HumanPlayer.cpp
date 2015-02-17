//-----------------------------------------------------
// FILE: HumanPlayer.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "HumanPlayer.h"


//-----------------------------------------------------
// MENU STATE CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CHumanPlayer::CHumanPlayer() : CPlayer()
{
	// When initialised, create the command centre at the centre of the respective grid
	
}

CHumanPlayer::~CHumanPlayer()
{
	
}


//-----------------------------------------------------
// HUMAN PLAYER CLASS METHODS
//-----------------------------------------------------
CStructure* CHumanPlayer::CheckStructureSelection(DX::XMFLOAT3 pos)
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
void CHumanPlayer::Update()
{
	// Loop through all structures & update them
	for (mpiterStructures = mpStructureList.begin(); mpiterStructures != mpStructureList.end(); mpiterStructures++)
	{
		(*mpiterStructures)->Update();
	}
}
