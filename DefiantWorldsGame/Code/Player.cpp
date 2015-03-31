//-----------------------------------------------------
// FILE: Player.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Player.h"
#include "GameStateControl.h"


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

	//CSpaceFighter* Temp;
	//for (int i = 0; i <25; i++)
	//{
	//	Temp = new CSpaceFighter();
	//	Temp->SetFaction(mPlayerFaction);
	//	mpSpaceUnitsList.push_back(Temp);
	//}
	//CTransport* temp;
	//for (int i = 0; i < 5; i++)
	//{
	//	temp = new CTransport();
	//	temp->SetFaction(mPlayerFaction);
	//	mpSpaceUnitsList.push_back(temp);
	//}

	//mpSpaceUnitsList.push_back(new CMothership());
	//mpSpaceUnitsList.back()->SetFaction(mPlayerFaction);
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
	DX::XMFLOAT3 origin, DX::XMFLOAT3 direction)
{
	float curDist = 99999.0f;
	float newDist = 0;
	
	// FIRST CHECK UNITS
	pGameAgent = nullptr;

	// Loop through all Units
	for (miterUnitsMap = mpUnitsMap.begin(); miterUnitsMap != mpUnitsMap.end(); miterUnitsMap++)
	{
		// If there is a collision, return the pointer to that object
		if (miterUnitsMap->second->RayCollision(origin, direction, newDist) && newDist < curDist)
		{
			pGameAgent = miterUnitsMap->second;
			curDist = newDist;
		}
	}


	// THEN CHECK BUILDINGS
	pStructure = nullptr;

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
		}
	}

	// If structure is pointing at something, game agent cannot
	if (pStructure)
	{
		pGameAgent = nullptr;
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

		// Add base amount
		MineralTransaction(mMineralBaseAddition);

		// Count how many workers the player has
		// Infantry is from barracks - get all instances of barracks	
		auto range = mpUnitsMap.equal_range(GAV_WORKER);

		// Check that some structures exist
		if (range.first != mpUnitsMap.end())
		{
			// Loop through each worker unit & increment the counter
			for (auto iter = range.first; iter != range.second; ++iter)
			{
				numWorkers++;
			}
		}

		// use the number of workers to determine bonus
		MineralTransaction(static_cast<float>(numWorkers * mMineralBaseAddition));
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

	// Loop through all units (NOT SPACE) & update them
	for (miterUnitsMap = mpUnitsMap.begin(); miterUnitsMap != mpUnitsMap.end(); miterUnitsMap++)
	{
		if (!miterUnitsMap->second->Update())
		{
			// The current unit has been destroyed
			CGameAgent* tmp = miterUnitsMap->second;
			SafeDelete(tmp);
			mpUnitsMap.erase(miterUnitsMap);
			break;
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
	for (int i = 0; i < 3; i++)
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
}
