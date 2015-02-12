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

}

CHumanPlayer::~CHumanPlayer()
{
	
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
