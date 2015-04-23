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

	mPlayerDataInitialised = false;

	// Get random invasion times & unit numbers
	mEarthUnits = gpRandomiser->GetRandomInt(3, 7);
	mMarsUnits = gpRandomiser->GetRandomInt(3, 7);
	mTimeToEarthInvasion = gpRandomiser->GetRandomFloat(20.0f, 60.0f);
	mTimeToMarsInvasion = gpRandomiser->GetRandomFloat(20.0f, 60.0f);
}

CPlayerManager::~CPlayerManager()
{
	RemovePlayers();
}


//-----------------------------------------------------
// PLAYER MANAGER CLASS METHODS
//-----------------------------------------------------
void CPlayerManager::CreatePlayers(EFactions humanFaction, int numAI, int startingResources, int difficulty)
{
	// Create the human player
	mpHuman = new CRTSPlayer(humanFaction, startingResources);

	// Store number of active AI Players
	mNumAI = numAI;

	// Create new AI players for each other faction
	mpAI[0] = new CRTSAIPlayer(FAC_THE_CRIMSON_LEGION, startingResources, difficulty);

	mPlayerDataInitialised = true;
}

void CPlayerManager::RemovePlayers()
{
	SafeDelete(mpHuman);
	SafeDelete(mpAI[0]);

	mPlayerDataInitialised = false;

	// Remove rebel units
	while (!mpRebelEarthList.empty())
	{
		CGameAgent* pAgent = mpRebelEarthList.back();
		SafeDelete(pAgent);
		mpRebelEarthList.pop_back();
	}

	while (!mpRebelMarsList.empty())
	{
		CGameAgent* pAgent = mpRebelMarsList.back();
		SafeDelete(pAgent);
		mpRebelMarsList.pop_back();
	}
}

int CPlayerManager::UpdatePlayers()
{
	// Update rebels
	// Check how much time has passed since the game has begun - currently checking for 40 seconds into the game
	if (mTimeSinceGameStart > 40.0f)
	{
		// Check to see if it is time to invade earth or mars
		if (mTimeToEarthInvasion < 0.0f)
		{
			// Let the player know an invasion is coming
			gpNewsTicker->AddNewElement("A rebel faction is attacking!", true);
			InvadeEarth();
		}
		else
		{
			mTimeToEarthInvasion -= gFrameTime;
		}
		if (mTimeToMarsInvasion < 0.0f)
		{
			InvadeMars();
		}
		else
		{
			mTimeToMarsInvasion -= gFrameTime;
		}
	}
	
	// Update human
	mpHuman->Update();

	// Loop through and update each AI
	for (int i = 0; i < mNumAI; i++)
	{
		mpAI[i]->Update();
	}

	// Check if the players are alive
	if (mpHuman->IsAlive())
	{
		// Then check if AI player is alive
		if (mpAI[0]->IsAlive())
		{
			// No deaths - return 0 to symbolise nothing is wrong
			return 0;
		}
		else
		{
			// An AI death - return 2
			return 2;
		}
	}
	else
	{
		// Player death - return 1 to symbolise this
		return 1;
	}
}

void CPlayerManager::SavePlayers(std::ofstream& outFile)
{
	// Save human
	mpHuman->SavePlayerData(outFile);

	// Loop through and save each AI
	for (int i = 0; i < mNumAI; i++)
	{
		mpAI[i]->SavePlayerData(outFile);
	}
}

void CPlayerManager::LoadPlayers(std::ifstream& inFile)
{
	// Human player first
	mpHuman->LoadPlayerData(inFile);

	// Loop through and load each AI
	for (int i = 0; i < mNumAI; i++)
	{
		mpAI[i]->LoadPlayerData(inFile);
	}
}

void CPlayerManager::InvadeEarth()
{
	// Check how many units have already been produced
	if (mpRebelEarthList.size() < mEarthUnits)
	{

	}
	else
	{
		// Reset units amount and time to invasion
		mEarthUnits = gpRandomiser->GetRandomInt(3, 7);
		mTimeToEarthInvasion = gpRandomiser->GetRandomFloat(20.0f, 60.0f);
	}
}

void CPlayerManager::InvadeMars()
{
	// Check how many units have already been produced
	if (mpRebelMarsList.size() < mMarsUnits)
	{

	}
	else
	{
		// Reset units amount and time to invasion
		mMarsUnits = gpRandomiser->GetRandomInt(3, 7);
		mTimeToMarsInvasion = gpRandomiser->GetRandomFloat(20.0f, 60.0f);
	}
}
