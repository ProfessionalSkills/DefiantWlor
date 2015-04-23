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

	// Update rebel earth units
	for (auto iter = mpRebelEarthList.begin(); iter != mpRebelEarthList.end(); iter++)
	{
		// Call update function for this structure
		if (!(*iter)->Update())
		{
			// The current structure has been destroyed
			CGameAgent* tmp = (*iter);
			SafeDelete(tmp);
			mpRebelEarthList.erase(iter);
			break;
		}
	}

	// Update rebel mars units
	for (auto iter = mpRebelMarsList.begin(); iter != mpRebelMarsList.end(); iter++)
	{
		// Call update function for this structure
		if (!(*iter)->Update())
		{
			// The current structure has been destroyed
			CGameAgent* tmp = (*iter);
			SafeDelete(tmp);
			mpRebelMarsList.erase(iter);
			break;
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
		// Get a random unit to spawn
		EGameAgentVariations unit = static_cast<EGameAgentVariations>(gpRandomiser->GetRandomInt(GAV_FIGHTER, GAV_ARTILLERY));
		CGameAgent* pNewAgent = nullptr;

		// create a unit based on the unit chosen
		switch (unit)
		{
		case GAV_MOTHERSHIP:
		case GAV_ARTILLERY:
		case GAV_BOMBER:
			// Create a new bomber agent
			pNewAgent = new CBomber();

			// Target a random structure
			pNewAgent->SetAttackTarget(mpHuman->GetRandomStructure());

			// Set attributes for the new agent
			pNewAgent->SetWorldPos({ -2000.0f, 50.0f, 0.0f });
			pNewAgent->SetFaction(FAC_REBELS);
			pNewAgent->SetState(OBJ_BUILT);

			// Spawn the unit & store
			pNewAgent->LoadIModel();
			pNewAgent->CalculateBoundingSphere();
			mpRebelEarthList.push_back(pNewAgent);
			break;

		case GAV_TRANSPORT:
		case GAV_SPACE_FIGHTER:
		case GAV_FIGHTER:
			// Create a new fighter agent
			pNewAgent = new CFighter();

			// Target a random agent
			pNewAgent->SetAttackTarget(mpHuman->GetRandomAgent());

			// Check if there is no agents available to target
			if (!pNewAgent)
			{
				// target a structure instead
				pNewAgent->SetAttackTarget(mpHuman->GetRandomStructure());
			}

			// Set attributes for the new agent
			pNewAgent->SetWorldPos({ -2000.0f, 50.0f, 0.0f });
			pNewAgent->SetFaction(FAC_REBELS);
			pNewAgent->SetState(OBJ_BUILT);

			// Spawn the unit & store
			pNewAgent->LoadIModel();
			pNewAgent->CalculateBoundingSphere();
			mpRebelEarthList.push_back(pNewAgent);
			break;
		case GAV_WORKER:
		case GAV_INFANTRY:
		case GAV_TANK:
			// Create a new infantry agent
			pNewAgent = new CInfantry();

			// Target a random agent & store the type
			pNewAgent->SetAttackTarget(mpHuman->GetRandomAgent());
			EGameAgentVariations targetType = pNewAgent->GetAgentData()->mAgentType;

			// Check if there is no agents available to target
			if (!pNewAgent)
			{
				// target a structure instead
				pNewAgent->SetAttackTarget(mpHuman->GetRandomStructure());
			}
			// Also check to see if the agent selected is not an air unit as infantry cannot shoot them
			else if (targetType == GAV_FIGHTER || targetType == GAV_BOMBER)
			{
				// target a structure instead
				pNewAgent->SetAttackTarget(mpHuman->GetRandomStructure());
			}

			// Set attributes for the new agent
			pNewAgent->SetWorldPos({ -2000.0f, 1.0f, 0.0f });
			pNewAgent->SetFaction(FAC_REBELS);
			pNewAgent->SetState(OBJ_BUILT);

			// Spawn the unit & store
			pNewAgent->LoadIModel();
			pNewAgent->CalculateBoundingSphere();
			mpRebelEarthList.push_back(pNewAgent);
			break;
		}
	}
	else
	{
		// Reset units amount and time to invasion
		mEarthUnits = gpRandomiser->GetRandomInt(3, 7);
		mTimeToEarthInvasion = gpRandomiser->GetRandomFloat(40.0f, 80.0f);
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

void CPlayerManager::CheckRebelSelection(CStructure*& pStructure, CGameAgent*& pGameAgent, CMinerals*& pMinerals,
	DX::XMFLOAT3 origin, DX::XMFLOAT3 direction, float& curDist)
{
	float newDist = 0.0f;
	
	// Loop through all earth rebel Units
	for (auto iter = mpRebelEarthList.begin(); iter != mpRebelEarthList.end(); iter++)
	{
		// If there is a collision, return the pointer to that object
		if ((*iter)->RayCollision(origin, direction, newDist) && newDist < curDist)
		{
			pGameAgent = (*iter);
			curDist = newDist;

			// Set all other pointers to null
			pMinerals = nullptr;
			pStructure = nullptr;
		}
	}

	// Loop through all mars rebel Units
	for (auto iter = mpRebelMarsList.begin(); iter != mpRebelMarsList.end(); iter++)
	{
		// If there is a collision, return the pointer to that object
		if ((*iter)->RayCollision(origin, direction, newDist) && newDist < curDist)
		{
			pGameAgent = (*iter);
			curDist = newDist;

			// Set all other pointers to null
			pMinerals = nullptr;
			pStructure = nullptr;
		}
	}
}
