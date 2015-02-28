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
	// Initialise null pointers
	mpHuman = nullptr;

	// Loop through and update each AI
	for (int i = 0; i < MAX_AI_NUM; i++)
	{
		mpAI[i] = nullptr;
	}
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
	mpHuman = new CRTSPlayer(humanFaction);

	// Store number of active AI Players
	mNumAI = numAI;

	// Create new AI players for each other faction
	mpAI[0] = new CRTSAIPlayer(FAC_THE_CRIMSON_LEGION);
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
