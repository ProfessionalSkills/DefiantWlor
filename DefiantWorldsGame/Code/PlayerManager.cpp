//-----------------------------------------------------
// FILE: Player.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "PlayerManager.h"


//-----------------------------------------------------
// PLAYER MANAGER CLASS CONSTRUCTOR & DESTRUCTOR
//-----------------------------------------------------
CPlayerManager::CPlayerManager()
{

}

CPlayerManager::~CPlayerManager()
{
	RemovePlayers();
}


//-----------------------------------------------------
// PLAYER MANAGER CLASS METHODS
//-----------------------------------------------------
void CPlayerManager::CreatePlayers(EFactions humanFaction, int numAI)
{
	// Create the human player
	mpHuman = new CPlayer(humanFaction);

	// Store number of active AI Players
	mNumAI = numAI;

	// Create new AI players for each other faction
	mpAI[0] = new CPlayer(FAC_THE_CRIMSON_LEGION);
}

void CPlayerManager::RemovePlayers()
{
	SafeDelete(mpHuman);
	SafeDelete(mpAI[0]);
}

void CPlayerManager::UpdatePlayers()
{
	// Update human
	mpHuman->Update();

	// Loop through and update each AI
	for (int i = 0; i < mNumAI; i++)
	{
		mpAI[i]->Update();
	}
}
