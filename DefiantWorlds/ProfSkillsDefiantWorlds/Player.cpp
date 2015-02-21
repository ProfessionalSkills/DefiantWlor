//-----------------------------------------------------
// FILE: Player.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Player.h"


//-----------------------------------------------------
// MENU STATE CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CPlayer::CPlayer()
{
	
}

CPlayer::~CPlayer()
{

}


//-----------------------------------------------------
// HUMAN PLAYER CLASS METHODS
//-----------------------------------------------------
CStructure* CPlayer::CheckStructureSelection(DX::XMFLOAT3 pos)
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
void CPlayer::Update()
{
	// Loop through all structures & update them
	for (mpiterStructures = mpStructureList.begin(); mpiterStructures != mpStructureList.end(); mpiterStructures++)
	{
		if (!(*mpiterStructures)->Update())
		{
			// The current structure has been destroyed
			CStructure* tmp = (*mpiterStructures);
			SafeDelete(tmp);
			mpStructureList.erase(mpiterStructures);
			break;
		}
	}
}
