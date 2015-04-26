//-----------------------------------------------------
// FILE: Player.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Player.h"
#include "GameStateControl.h"

IMesh* CRTSPlayer::mspMshPillar = nullptr;


//-----------------------------------------------------
// BASE PLAYER CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CRTSPlayer::CRTSPlayer(EFactions playerFaction, int startingResources) : MINERAL_UPDATE_TIME(5.0f)
{
	mNumMinerals = startingResources;
	mpFleet = new CFleet();
	mPlayerFaction = playerFaction;
	mNumMothership = 0;
	mNumSpaceFighter = 0;
	mNumTransport = 0;
	mMineralBaseAddition = 100;
	mPopLimit = 250;
	mCurPop = 0;
	mTimeToMineralUpdate = MINERAL_UPDATE_TIME;
	mpPlayerGrid = nullptr;
	mpRandomiser = new CRandomiser();
	mWonLastSpaceBattle = false;

	// Get the player manager
	pPlayerManager = CStateControl::GetInstance()->GetPlayerManager();
}

CRTSPlayer::~CRTSPlayer()
{
	for (miterStructuresMap = mpStructuresMap.begin(); miterStructuresMap != mpStructuresMap.end(); miterStructuresMap++)
	{
		// Remove all models
		miterStructuresMap->second->UnloadIModel();
	}

	// Handle removal of the grids
	SafeDelete(mpPlayerGrid);
}


//-----------------------------------------------------
// PLAYER CLASS MUTATORS
//-----------------------------------------------------
bool CRTSPlayer::MineralTransaction(int amount)
{
	if (mNumMinerals + amount < 0)
	{
		// Not enough funds for transaction
		return false;
	}
	else
	{
		// Enough funds
		mNumMinerals += amount;
		return true;
	}
}

void CRTSPlayer::SetNumSpaceFighter(int numShips)
{
	mNumSpaceFighter = numShips;
}

void CRTSPlayer::SetNumTransport(int numShips)
{
	mNumTransport = numShips;
}

void CRTSPlayer::SetNumMothership(int numShips)
{
	mNumMothership = numShips;
}

bool CRTSPlayer::IsAlive()
{
	// Returns true if the player's command centre still has health
	// Get a pointer to the command centre
	// Infantry is from barracks - get all instances of barracks	
	auto range = mpStructuresMap.equal_range(STR_COM_CENTRE);

	// If there is no instance of a command centre, then this player has lost
	if (range.first == mpStructuresMap.end())
	{
		// Player has lost their command centre
		return false;
	}
	// Player's command centre remains standing
	return true;
}

CGameAgent* CRTSPlayer::GetRandomAgent()
{
	// Get the number of agents the player has
	int numAgents = mpUnitsMap.size();

	// If there are no units, return nullptr
	if (numAgents == 0) return nullptr;

	// Get a random index
	int index = gpRandomiser->GetRandomInt(0, numAgents - 1);

	// Advance the iterator by the index amount
	miterUnitsMap = mpUnitsMap.begin();
	std::advance(miterUnitsMap, index);

	// Return the pointer based on the index
	return miterUnitsMap->second;
}

CStructure* CRTSPlayer::GetRandomStructure()
{
	// Get the number of agents the player has
	int numStructures = mpStructuresMap.size();

	// If there are no units, return nullptr
	if (numStructures == 0) return nullptr;

	// Get a random index
	int index = gpRandomiser->GetRandomInt(0, numStructures - 1);

	// Advance the iterator by the index amount
	miterStructuresMap = mpStructuresMap.begin();
	std::advance(miterStructuresMap, index);

	// Return the pointer based on the index
	return miterStructuresMap->second;
}


//-----------------------------------------------------
// PLAYER CLASS METHODS
//-----------------------------------------------------
void CRTSPlayer::ConstructWalls()
{
	DX::XMFLOAT3 gridCentre = mpPlayerGrid->GetGridCentrePos();
	DX::XMFLOAT3 gridBottomLeft = mpPlayerGrid->GetGridStartPos();
	DX::XMFLOAT3 gridTopRight = mpPlayerGrid->GetGridEndPos();

	// Create the first wall
	CWall* pWall = new CWall(false);
	pWall->SetWorldPos(DX::XMFLOAT3(gridBottomLeft.x - 7.0f, 0.0f, gridTopRight.z / 4.0f));
	pWall->SetFaction(mPlayerFaction);
	pWall->LoadIModel();
	mpStructuresMap.insert(GS_MultiMap::value_type(pWall->GetStructureType(), pWall));

	// Create second wall
	pWall = new CWall(false);
	pWall->SetWorldPos(DX::XMFLOAT3(gridBottomLeft.x - 7.0f, 0.0f, gridTopRight.z * 3.0 / 4.0f));
	pWall->SetFaction(mPlayerFaction);
	pWall->LoadIModel();
	mpStructuresMap.insert(GS_MultiMap::value_type(pWall->GetStructureType(), pWall));

	// Create third wall
	pWall = new CWall(true);
	pWall->SetWorldPos(DX::XMFLOAT3((gridCentre.x - ((gridTopRight.x - gridCentre.x) / 2.0f)), 0.0f, gridTopRight.z + 7.0f));
	pWall->SetFaction(mPlayerFaction);
	pWall->LoadIModel();
	mpStructuresMap.insert(GS_MultiMap::value_type(pWall->GetStructureType(), pWall));

	// Create fourth wall
	pWall = new CWall(true);
	pWall->SetWorldPos(DX::XMFLOAT3((gridCentre.x + ((gridTopRight.x - gridCentre.x) / 2.0f)), 0.0f, gridTopRight.z + 7.0f));
	pWall->SetFaction(mPlayerFaction);
	pWall->LoadIModel();
	mpStructuresMap.insert(GS_MultiMap::value_type(pWall->GetStructureType(), pWall));

	// Create fifth wall
	pWall = new CWall(false);
	pWall->SetWorldPos(DX::XMFLOAT3(gridTopRight.x + 7.0f, 0.0f, gridTopRight.z / 4.0f));
	pWall->SetFaction(mPlayerFaction);
	pWall->LoadIModel();
	mpStructuresMap.insert(GS_MultiMap::value_type(pWall->GetStructureType(), pWall));

	// Create sixth wall
	pWall = new CWall(false);
	pWall->SetWorldPos(DX::XMFLOAT3(gridTopRight.x + 7.0f, 0.0f, gridTopRight.z * 3.0f / 4.0f));
	pWall->SetFaction(mPlayerFaction);
	pWall->LoadIModel();
	mpStructuresMap.insert(GS_MultiMap::value_type(pWall->GetStructureType(), pWall));

	// create Bottom wall
	// Create third wall
	pWall = new CWall(true);
	pWall->SetWorldPos(DX::XMFLOAT3((gridCentre.x - ((gridTopRight.x - gridCentre.x) / 2.0f)), 0.0f, gridBottomLeft.z - 7.0f));
	pWall->SetFaction(mPlayerFaction);
	pWall->LoadIModel();
	mpStructuresMap.insert(GS_MultiMap::value_type(pWall->GetStructureType(), pWall));

	// Create fourth wall
	pWall = new CWall(true);
	pWall->SetWorldPos(DX::XMFLOAT3((gridCentre.x + ((gridTopRight.x - gridCentre.x) / 2.0f)), 0.0f, gridBottomLeft.z - 7.0f));
	pWall->SetFaction(mPlayerFaction);
	pWall->LoadIModel();
	mpStructuresMap.insert(GS_MultiMap::value_type(pWall->GetStructureType(), pWall));

	// Pillars
	CreatePillars();
}

EErrorTypes CRTSPlayer::PurchaseStructure(CStructure* pStructure, CGrid* pGrid, CTile* pTile)
{
	// Check whether new building can be afforded
	if (mNumMinerals - pStructure->GetBuildCost() < 0)
	{
		// Not enough minerals - return relevant error
		return ERR_NO_MINERALS;
	}

	// Check if building area is free
	if (!pStructure->TestStructureArea(pGrid, pTile))
	{
		// Something blocking construction - return relevant error
		return ERR_NO_SPACE;
	}

	// Check to ensure no vehicles in the nearby area - maybe?

	// Make purchase
	MineralTransaction(-pStructure->GetBuildCost());

	// Everything fine - build & add to vector
	pStructure->CreateStructure(pGrid);
	pStructure->SetBuildLocation(pTile->GetGridPos());
	pStructure->SetFaction(mPlayerFaction);
	mpStructuresMap.insert(GS_MultiMap::value_type(pStructure->GetStructureType(), pStructure));
	return ERR_NONE;
}

bool CRTSPlayer::PopLimitReached(int popValue)
{
	if ((mCurPop + popValue) > mPopLimit)
	{
		return true;
	}
	return false;
}

bool CRTSPlayer::QueueUnit(CStructure* structure, CGameAgent* unit)
{
	return false;
}

void CRTSPlayer::LaunchAttack()
{
	mpSpaceUnitsList = (*mpFleet->LaunchFleet(&mpSpaceUnitsList));
}

void CRTSPlayer::CheckGameObjectSelection(CStructure*& pStructure, CGameAgent*& pGameAgent,
	CMinerals*& pMinerals, DX::XMFLOAT3 origin, DX::XMFLOAT3 direction, float& curDist, bool toCheckMinerals)
{
	float newDist = 0;
	
	// First check for a selection of minerals, if it has been requested
	if (toCheckMinerals)
	{
		for (miterMineralsList = mpMineralsList.begin(); miterMineralsList != mpMineralsList.end(); miterMineralsList++)
		{
			// If there is a collision, return the pointer to that object
			if ((*miterMineralsList)->RayCollision(origin, direction, newDist) && newDist < curDist)
			{
				pMinerals = (*miterMineralsList);
				curDist = newDist;

				// Set all other pointers to null
				pStructure = nullptr;
				pGameAgent = nullptr;
			}
		}
	}

	// Loop through all Units
	for (miterUnitsMap = mpUnitsMap.begin(); miterUnitsMap != mpUnitsMap.end(); miterUnitsMap++)
	{
		// If there is a collision, return the pointer to that object
		if (miterUnitsMap->second->RayCollision(origin, direction, newDist) && newDist < curDist)
		{
			pGameAgent = miterUnitsMap->second;
			curDist = newDist;

			// Set all other pointers to null
			pMinerals = nullptr;
			pStructure = nullptr;
		}
	}

	// Loop through all structures
	for (miterStructuresMap = mpStructuresMap.begin(); miterStructuresMap != mpStructuresMap.end(); miterStructuresMap++)
	{
		// If there is a collision, return the pointer to that object
		if (miterStructuresMap->second->RayCollision(origin, direction, newDist) && newDist < curDist)
		{
			pStructure = miterStructuresMap->second;

			string mMusicFile = "";
			DX::XMFLOAT3 mSourcePos;
			DX::XMFLOAT3 mSourceVel;
			DX::XMFLOAT3 listenerPos;
			DX::XMFLOAT3 listenerVel;
			if (pStructure->GetSelectSound() == nullptr)
			{
				mSourcePos = { pStructure->GetWorldPos().x, pStructure->GetWorldPos().y, pStructure->GetWorldPos().z };
				mSourceVel = { 0.0f, 0.0f, 0.0f };
				listenerPos = { pStructure->GetWorldPos().x, 50.0f, pStructure->GetWorldPos().z };
				listenerVel = { 0.0f, 0.0f, 0.0f };

				switch (pStructure->GetStructureType())
				{
				case STR_BARRACKS:
					mMusicFile = "Barracks.Select.wav"; //Sets the music file
					break;
				case STR_COM_CENTRE:
					mMusicFile = "Barracks.Select.wav";
					break;

				case STR_HELLIPAD:
					mMusicFile = "Helipad.Select.wav";
					break;

				case STR_SPACE_CENTRE:
					mMusicFile = "SpaceCentre.Select.wav";
					break;
				}

				float volume = CStateControl::GetInstance()->GetSettingsManager()->GetEffectsVolume();		// MAKE SURE TO INCLUDE GameStateControl in the
																											// .cpp file ONLY otherwise you'll get cyclic redundancy
				pStructure->SetSelectSound(new CSound(mMusicFile, mSourcePos, mSourceVel, false, volume, listenerPos, listenerVel));
			}

			pStructure->GetSelectSound()->PlaySound();
			curDist = newDist;

			// Set all other pointers to null
			pMinerals = nullptr;
			pGameAgent = nullptr;
		}
	}
}

bool CRTSPlayer::CheckDragSelection(DX::BoundingBox& box, std::vector<CGameAgent*>& selectedAgents)
{
	bool unitSelected = false;	// Becomes true if at least one unit has been selected

	// Loop through all Units
	for (miterUnitsMap = mpUnitsMap.begin(); miterUnitsMap != mpUnitsMap.end(); miterUnitsMap++)
	{
		// If there is a collision, return the pointer to that object
		if (miterUnitsMap->second->DragBoxCollision(box))
		{
			// Raise selection flag
			unitSelected = true;

			// Add pointer to this agent to the list of selected agents
			selectedAgents.push_back(miterUnitsMap->second);
		}
	}

	return unitSelected;
}

void CRTSPlayer::Update()
{
	// Update mineral amount - check if the timer has been reached
	if (mTimeToMineralUpdate <= 0.0f)
	{
		// Reset timer & begin counter for how many worker units there are
		mTimeToMineralUpdate = MINERAL_UPDATE_TIME;
		int numWorkers = 0;

		// Count how many workers the player has
		auto range = mpUnitsMap.equal_range(GAV_WORKER);

		// Check that some structures exist
		if (range.first != mpUnitsMap.end())
		{
			// Loop through each worker unit & increment the counter
			for (auto iter = range.first; iter != range.second; ++iter)
			{
				numWorkers++;

				// Check if this worker is currently harvesting minerals
				// Cast into a CWorker pointer
				CWorker* pWorker = static_cast<CWorker*>(iter->second);
				if (pWorker->IsHarvestingMineral())
				{
					// Increment minerals by 100
					MineralTransaction(mMineralBaseAddition);
				}
			}
		}
	}
	else
	{
		// Decrease timer
		mTimeToMineralUpdate -= gFrameTime;
	}
	
	// Loop through all structures & update them
	for (miterStructuresMap = mpStructuresMap.begin(); miterStructuresMap != mpStructuresMap.end(); miterStructuresMap++)
	{
		// Call update function for this structure
		if (!miterStructuresMap->second->Update(this))
		{
			// The current structure has been destroyed
			CStructure* tmp = miterStructuresMap->second;
			SafeDelete(tmp);
			mpStructuresMap.erase(miterStructuresMap);
			break;
		}
	}

	// Clear the airspace before more units are added in in the next section
	if (mPlayerFaction == FAC_EARTH_DEFENSE_FORCE)
	{
		pPlayerManager->ClearEarthAirspace();
	}
	else
	{
		pPlayerManager->ClearMarsAirspace();
	}

	// Loop through all units (NOT SPACE) & update them
	for (miterUnitsMap = mpUnitsMap.begin(); miterUnitsMap != mpUnitsMap.end(); miterUnitsMap++)
	{
		// The current unit has been destroyed
		CGameAgent* pAgent = miterUnitsMap->second;
		
		if (!pAgent->Update())
		{
			// Before deleting, check if the agent is a worker unit
			if (pAgent->GetAgentData()->mAgentType == GAV_WORKER)
			{
				// Check if the worker is responsible for any mineral deposits
				CWorker* pWorker = static_cast<CWorker*>(pAgent);
				if (pWorker->GetMineral())
				{
					pWorker->GetMineral()->SetUsage(false);
				}
			}

			SafeDelete(pAgent);
			mpUnitsMap.erase(miterUnitsMap);
			break;
		}
		else
		{
			// Compare faction of this unit against the player
			if (pAgent->GetFaction() != mPlayerFaction)
			{
				// Check if it's in the wrong airspace
				if (mPlayerFaction == FAC_EARTH_DEFENSE_FORCE)
				{
					if (pAgent->GetAirspacePosition() == AS_MARS)
					{
						pPlayerManager->AddToMarsAirspace(pAgent);
					}
				}
				else if (mPlayerFaction == FAC_THE_CRIMSON_LEGION)
				{
					if (pAgent->GetAirspacePosition() == AS_EARTH)
					{
						pPlayerManager->AddToEarthAirspace(pAgent);
					}
				}
			}

			// Check attach update time
			if (mTimeToAttackCheckUpdate <= 0.0f)
			{
				// Check if there are any units in this player's airspace - 10% chance of unit attacking those in their airspace
				if (!pAgent->GetAttackTarget() && mpRandomiser->GetRandomFloat(0.0f, 100.0f) > 90.0f)
				{
					CGameAgent* pTarget = nullptr;
					int index = 0;
					
					if (mpAirspaceAgents.size())
					{
						// Pick a random index of these units interfering
						index = mpRandomiser->GetRandomInt(0, mpAirspaceAgents.size() - 1);

						// Check target is viable for targeting
						pTarget = mpAirspaceAgents[index];

						if (pTarget->GetHealth() > 0.0f)
						{
							// Give the target to this unit
							pAgent->SetAttackTarget(pTarget);
						}
					}
				}

				// Reset timer
				mTimeToAttackCheckUpdate = 0.3f;
			}
			else
			{
				mTimeToAttackCheckUpdate -= gFrameTime;
			}

			// Check for wall collisions - units should not be able to move through walls
			if (mTimeToWallCheckUpdate <= 0.0f)
			{
				// Reset wall check timer
				mTimeToWallCheckUpdate = 0.3f;
			}
			else
			{
				mTimeToWallCheckUpdate -= gFrameTime;
			}
		}
	}

	for (auto turret : mpBaseTurretList)
	{
		turret->Update(this);
		float distance = 9999.0f;
		if (turret->GetAttackTarget() == nullptr)
		{
			for (auto enemy : mpAirspaceAgents)
			{
				if (enemy->GetObjectType() == Q_BOMBER || enemy->GetObjectType() == Q_FIGHTER)
				{
					float distX = turret->GetWorldPos().x - enemy->GetWorldPos().x;
					float distY = turret->GetWorldPos().y - enemy->GetWorldPos().y;
					float distZ = turret->GetWorldPos().z - enemy->GetWorldPos().z;

					distance = ((distX * distX) + (distY * distY) + (distZ * distZ));
					if (distance <= (turret->GetRange() * turret->GetRange()))
					{
						turret->SetAttackTarget(enemy);
						break;
					}
				}
			}
		}
		
	}
}

void CRTSPlayer::CreateResourcePiles()
{
	SPointData spawnTile;
	bool tileUsed = true;
	CTile* pTile = nullptr;
	CMinerals* pNewMineral = nullptr;
	
	// Create 3 resource piles around the grid area for the player
	int numDeposits = CStateControl::GetInstance()->GetSettingsManager()->GetMineralDepositsValue();
	for (int i = 0; i < numDeposits; i++)
	{
		// Until a free tile is found
		while (tileUsed)
		{
			// Choose a random spawn tile
			spawnTile = { mpRandomiser->GetRandomInt(0, GRID_SIZE_X), mpRandomiser->GetRandomInt(0, GRID_SIZE_Y) };
			
			// Check if the spawn tile is used or attempts to spawn off of the grid
			pTile = mpPlayerGrid->GetTileData(spawnTile);
			if (pTile == nullptr)
			{
				tileUsed = true;
			}
			else
			{
				tileUsed = pTile->IsTileUsed();
			}
		}

		// Found a tile not in use - spawn the tile here
		pNewMineral = new CMinerals();
		pNewMineral->CreateResource(mpPlayerGrid, spawnTile);
		mpMineralsList.push_back(pNewMineral);

		// Ensure tile is labelled as used
		pTile->SetTileUsage(true);
		tileUsed = true;
	}
}

void CRTSPlayer::LoadPlayerGridModels()
{
	// Structures
	for (miterStructuresMap = mpStructuresMap.begin(); miterStructuresMap != mpStructuresMap.end(); miterStructuresMap++)
	{
		miterStructuresMap->second->LoadIModel();
	}

	// Units
	for (miterUnitsMap = mpUnitsMap.begin(); miterUnitsMap != mpUnitsMap.end(); miterUnitsMap++)
	{
		miterUnitsMap->second->LoadIModel();
	}

	// Resources
	for (miterMineralsList = mpMineralsList.begin(); miterMineralsList != mpMineralsList.end(); miterMineralsList++)
	{
		(*miterMineralsList)->LoadIModel();
	}

	// Pillars
	CreatePillars();
}

void CRTSPlayer::UnloadPlayerGridModels()
{
	// Structures
	for (miterStructuresMap = mpStructuresMap.begin(); miterStructuresMap != mpStructuresMap.end(); miterStructuresMap++)
	{
		miterStructuresMap->second->UnloadIModel();
	}

	// Units
	for (miterUnitsMap = mpUnitsMap.begin(); miterUnitsMap != mpUnitsMap.end(); miterUnitsMap++)
	{
		miterUnitsMap->second->UnloadIModel();
	}

	// Resources
	for (miterMineralsList = mpMineralsList.begin(); miterMineralsList != mpMineralsList.end(); miterMineralsList++)
	{
		(*miterMineralsList)->UnloadIModel();
	}

	// Pillars
	RemovePillars();
}

void CRTSPlayer::SavePlayerData(std::ofstream& outFile)
{
	// Save all of the player information to the file given
	outFile << mNumMinerals << " " << mPlayerFaction << " " << mNumSpaceFighter << " " << mNumTransport << " " << mNumMothership << std::endl;

	// Save the grid information for this player
	mpPlayerGrid->SaveTiles(outFile);

	// Save number of structures
	outFile << mpStructuresMap.size() << std::endl;

	// For each structure, save its data
	for (miterStructuresMap = mpStructuresMap.begin(); miterStructuresMap != mpStructuresMap.end(); miterStructuresMap++)
	{
		miterStructuresMap->second->SaveStructure(outFile);
	}

	// Save number of world units
	outFile << mpUnitsMap.size() << std::endl;

	// For each world unit, save its data
	for (miterUnitsMap = mpUnitsMap.begin(); miterUnitsMap != mpUnitsMap.end(); miterUnitsMap++)
	{
		miterUnitsMap->second->SaveAgent(outFile);
	}

	// Save number of space units
	outFile << mpSpaceUnitsList.size() << std::endl;

	// For each space unit, save its data
	for (mpiterSpaceUnits = mpSpaceUnitsList.begin(); mpiterSpaceUnits != mpSpaceUnitsList.end(); mpiterSpaceUnits++)
	{
		(*mpiterSpaceUnits)->SaveAgent(outFile);
	}

	// Save number of resources
	outFile << mpMineralsList.size() << std::endl;

	// Loop through all resources and save them
	for (miterMineralsList = mpMineralsList.begin(); miterMineralsList != mpMineralsList.end(); miterMineralsList++)
	{
		SPointData mineralPos = (*miterMineralsList)->GetGridPos();
		outFile << mineralPos.mPosX << " " << mineralPos.mPosY << std::endl;
	}
}

void CRTSPlayer::LoadPlayerData(std::ifstream& inFile)
{
	// Load player specific data
	int faction = 0;
	int minerals = 0;
	inFile >> minerals >> faction >> mNumSpaceFighter >> mNumTransport >> mNumMothership;
	// give more minerals than they will need
	mNumMinerals = 99999;

	// Convert faction to enumeration
	mPlayerFaction = static_cast<EFactions>(faction);

	// Load grid tiles
	mpPlayerGrid->LoadTiles(inFile);

	// Load in structures
	int numStructures;
	inFile >> numStructures;
	int structureType;
	EGameStructureTypes sType;
	for (int i = 0; i < numStructures; i++)
	{
		// Determine type
		inFile >> structureType;
		sType = static_cast<EGameStructureTypes>(structureType);
		CStructure* pLoadedStructure = nullptr;
		switch (sType)
		{
		case STR_HOUSE:
			pLoadedStructure = new CHouse();
			break;
		case STR_COM_CENTRE:
			pLoadedStructure = new CComCentre();
			break;
		case STR_SPACE_CENTRE:
			pLoadedStructure = new CSpaceCentre();
			break;
		case STR_BARRACKS:
			pLoadedStructure = new CBarracks();
			break;
		case STR_HELLIPAD:
			pLoadedStructure = new CHellipad();
			break;
		case STR_WALL:
			pLoadedStructure = new CWall();
			break;
		}

		// Load saved information for the structure
		pLoadedStructure->LoadStructure(inFile, mpPlayerGrid, this);

		// Add to structures map
		mpStructuresMap.insert(GS_MultiMap::value_type(sType, pLoadedStructure));
	}

	// Load in world units
	int numWorldUnits;
	inFile >> numWorldUnits;
	int worldUnitType;
	EGameAgentVariations wuType;
	for (int i = 0; i < numWorldUnits; i++)
	{
		// Determine type
		inFile >> worldUnitType;
		wuType = static_cast<EGameAgentVariations>(worldUnitType);
		CGameAgent* pLoadedUnit = nullptr;
		switch (wuType)
		{
		case GAV_ARTILLERY:
			pLoadedUnit = new CArtillery();
			break;
		case GAV_BOMBER:
			pLoadedUnit = new CBomber();
			break;
		case GAV_FIGHTER:
			pLoadedUnit = new CFighter();
			break;
		case GAV_INFANTRY:
			pLoadedUnit = new CInfantry();
			break;
		case GAV_TANK:
			pLoadedUnit = new CTank();
			break;
		case GAV_WORKER:
			pLoadedUnit = new CWorker();
			break;
		}

		// Load saved information for the structure
		pLoadedUnit->LoadAgent(inFile);

		// Add to structures map
		mpUnitsMap.insert(GA_MultiMap::value_type(wuType, pLoadedUnit));
	}

	// Load in space units
	int numSpaceUnits;
	inFile >> numSpaceUnits;
	int spaceUnitType;
	EGameAgentVariations suType;
	for (int i = 0; i < numSpaceUnits; i++)
	{
		// Determine type
		inFile >> spaceUnitType;
		suType = static_cast<EGameAgentVariations>(spaceUnitType);
		CGameAgent* pLoadedUnit = nullptr;
		switch (suType)
		{
		case GAV_MOTHERSHIP:
			pLoadedUnit = new CMothership();
			break;
		case GAV_SPACE_FIGHTER:
			pLoadedUnit = new CSpaceFighter();
			break;
		case GAV_TRANSPORT:
			pLoadedUnit = new CTransport();
			break;
		}

		// Load saved information for the structure
		pLoadedUnit->LoadAgent(inFile);

		// Add to structures map
		mpSpaceUnitsList.push_back(pLoadedUnit);
	}

	// Load minerals
	int numMinerals;
	inFile >> numMinerals;
	for (int i = 0; i < numMinerals; i++)
	{
		SPointData spawnTile;
		inFile >> spawnTile.mPosX >> spawnTile.mPosY;
		CMinerals* pNewMineral = new CMinerals();
		pNewMineral->CreateResource(mpPlayerGrid, spawnTile);
		mpMineralsList.push_back(pNewMineral);
	}

	// Store correct amount of minerals
	mNumMinerals = minerals;

	// Create the pillars
	CreatePillars();
}

void CRTSPlayer::CreatePillars()
{
	// Get important positions from the player's grid
	DX::XMFLOAT3 bottomLeft = mpPlayerGrid->GetGridStartPos();
	DX::XMFLOAT3 centre = mpPlayerGrid->GetGridCentrePos();
	DX::XMFLOAT3 topRight = mpPlayerGrid->GetGridEndPos();

	// Create the pillars and rotate them
	mpPillars[0] = mspMshPillar->CreateModel(bottomLeft.x - 7.5f, 0.0f, bottomLeft.z - 7.5f);
	mpPillars[0]->RotateY(-135.0f);

	mpPillars[1] = mspMshPillar->CreateModel(bottomLeft.x - 15.0f, 0.0f, centre.z);
	mpPillars[1]->RotateY(-90.0f);

	mpPillars[2] = mspMshPillar->CreateModel(bottomLeft.x - 7.5f, 0.0f, topRight.z + 7.5f);
	mpPillars[2]->RotateY(-45.0f);

	mpPillars[3] = mspMshPillar->CreateModel(centre.x, 0.0f, topRight.z + 15.0f);

	mpPillars[4] = mspMshPillar->CreateModel(topRight.x + 7.5f, 0.0f, topRight.z + 7.5f);
	mpPillars[4]->RotateY(45.0f);

	mpPillars[5] = mspMshPillar->CreateModel(topRight.x + 15.0f, 0.0f, centre.z);
	mpPillars[5]->RotateY(90.0f);

	mpPillars[6] = mspMshPillar->CreateModel(topRight.x + 7.5f, 0.0f, bottomLeft.z - 7.5f);
	mpPillars[6]->RotateY(135.0f);

	mpPillars[7] = mspMshPillar->CreateModel(centre.x, 0.0f, bottomLeft.z - 15.0f);
	mpPillars[7]->RotateY(180.0f);

	DX::XMFLOAT3 bottomLeftTurret = {bottomLeft.x - 7.5f, 0.0f, bottomLeft.z - 7.5f};
	DX::XMFLOAT3 bottomRightTurret = { topRight.x + 7.5f, 0.0f, bottomLeft.z - 7.5f };
	DX::XMFLOAT3 topLeftTurret = { bottomLeft.x - 7.5f, 0.0f, topRight.z + 7.5f };
	DX::XMFLOAT3 topRightTurret = { topRight.x + 7.5f, 0.0f, topRight.z + 7.5f };

	mpBaseTurretList[0] = new CTurretStructure(bottomLeftTurret);
	mpBaseTurretList[1] = new CTurretStructure(bottomRightTurret);
	mpBaseTurretList[2] = new CTurretStructure(topLeftTurret);
	mpBaseTurretList[3] = new CTurretStructure(topRightTurret);
	// Perform scales and texture changes to all pillars
	for (int i = 0; i < 8; i++)
	{
		mpPillars[i]->Scale(0.5f);
		mpPillars[i]->ScaleY(0.5f);

		if (mPlayerFaction == FAC_THE_CRIMSON_LEGION) mpPillars[i]->SetSkin("bld-mt-mars.jpg");
	}
}

void CRTSPlayer::RemovePillars()
{
	for (int i = 0; i < 8; i++)
	{
		mspMshPillar->RemoveModel(mpPillars[i]);
	}
}
