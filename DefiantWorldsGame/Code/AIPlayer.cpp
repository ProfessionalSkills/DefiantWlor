//-----------------------------------------------------
// FILE: AIPlayer.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "AIPlayer.h"


//-----------------------------------------------------
// AI PLAYER CLASS CONSTRUCTOR & DESTRUCTOR
//-----------------------------------------------------
CRTSAIPlayer::CRTSAIPlayer(EFactions playerFaction, int startingResources, int difficulty) : CRTSPlayer(playerFaction, startingResources)
{
	// Set difficulty values
	switch (difficulty)
	{
	case 0:				// Easy
		// Initialise update time
		UPDATE_TIME = 1.0f;

		// Initialise wait time values
		MIN_WAIT = 2.0f;
		MAX_WAIT = 5.0f;

		// After waiting, determine how many new items are queued
		MIN_NEW_ITEMS = 0;
		MAX_NEW_ITEMS = 3;

		// Initialise first few tasks of AI player
		mpTaskQ.push(new CBuildRequest(Q_WORKER, 1));
		mpTaskQ.push(new CBuildRequest(Q_WORKER, 20));
		mpTaskQ.push(new CBuildRequest(Q_WORKER, 40));
		break;
	case 1:				// Medium
		// Initialise update time
		UPDATE_TIME = 0.6f;

		// Initialise wait time values
		MIN_WAIT = 1.0f;
		MAX_WAIT = 3.0f;

		// After waiting, determine how many new items are queued
		MIN_NEW_ITEMS = 1;
		MAX_NEW_ITEMS = 4;

		// Initialise first 4 tasks of AI player
		mpTaskQ.push(new CBuildRequest(Q_WORKER, 1));
		mpTaskQ.push(new CBuildRequest(Q_WORKER, 10));
		mpTaskQ.push(new CBuildRequest(Q_WORKER, 15));
		break;
	case 2:				// Difficult
		// Initialise update time
		UPDATE_TIME = 0.3f;

		// Initialise wait time values
		MIN_WAIT = 0.5f;
		MAX_WAIT = 2.0f;

		// After waiting, determine how many new items are queued
		MIN_NEW_ITEMS = 2;
		MAX_NEW_ITEMS = 5;

		// Initialise first 6 tasks of AI player
		mpTaskQ.push(new CBuildRequest(Q_WORKER, 1));
		mpTaskQ.push(new CBuildRequest(Q_WORKER, 1));
		mpTaskQ.push(new CBuildRequest(Q_WORKER, 1));
		mpTaskQ.push(new CBuildRequest(Q_CHANGE_TACTIC, 10));
		mpTaskQ.push(new CBuildRequest(Q_INFANTRY, 15));
		break;
	case 3:				// Insane
		// Initialise update time
		UPDATE_TIME = 0.1f;

		// Initialise wait time values
		MIN_WAIT = 0.1f;
		MAX_WAIT = 1.0f;

		// After waiting, determine how many new items are queued
		MIN_NEW_ITEMS = 3;
		MAX_NEW_ITEMS = 6;

		// Initialise first 11 tasks of AI player
		mpTaskQ.push(new CBuildRequest(Q_WORKER, 1));
		mpTaskQ.push(new CBuildRequest(Q_WORKER, 1));
		mpTaskQ.push(new CBuildRequest(Q_WORKER, 1));
		mpTaskQ.push(new CBuildRequest(Q_CHANGE_TACTIC, 2));
		mpTaskQ.push(new CBuildRequest(Q_INFANTRY, 2));
		mpTaskQ.push(new CBuildRequest(Q_FIGHTER, 4));
		mpTaskQ.push(new CBuildRequest(Q_SPACE_FIGHTER, 60));
		mpTaskQ.push(new CBuildRequest(Q_MOTHERSHIP, 80));
		break;
	}

	// Set default mUpdateTime
	mUpdateTime = UPDATE_TIME;
	mWaitTime = mpRandomiser->GetRandomFloat(MIN_WAIT, MAX_WAIT);
}

CRTSAIPlayer::~CRTSAIPlayer()
{

}


//-----------------------------------------------------
// AI PLAYER CLASS METHODS
//-----------------------------------------------------
void CRTSAIPlayer::Update()
{
	// Call the base player's version of update first
	CRTSPlayer::Update();

	// Get the next item on AI's TODO list & send it to the resolution function
	// for the AI to attempt to resolve the item on the list
	// Check if it is time for the AI to perform an action
	if (mUpdateTime <= 0.0f)
	{
		// Check to see if there is data in the list
		if (!mpTaskQ.empty())
		{
			if (ResolveItem(mpTaskQ.top()->GetObjectType()))
			{
				// If item was resolved, remove it
				CBuildRequest* pRequest = mpTaskQ.top();
				SafeDelete(pRequest);
				mpTaskQ.pop();
			}
		}

		// Reset update time
		mUpdateTime = CRTSAIPlayer::UPDATE_TIME;
	}
	else
	{
		// Update the time until the AI is able to do another action
		mUpdateTime -= gFrameTime;
	}

	// Assess the situation
	if (mWaitTime <= 0.0f)
	{
		AssessSituation();
		mWaitTime = mpRandomiser->GetRandomFloat(MIN_WAIT, MAX_WAIT);
	}
	else
	{
		mWaitTime -= gFrameTime;
	}
}

void CRTSAIPlayer::AssessWorkers()
{
	// Get a list of worker units
	auto range = mpUnitsMap.equal_range(GAV_WORKER);

	// Check that some structures exist
	if (range.first != mpUnitsMap.end())
	{
		// Loop through each worker unit & increment the counter
		for (auto iter = range.first; iter != range.second; ++iter)
		{
			// Check if the worker unit has no active mineral
			// Cast into a worker unit poiner
			CWorker* pWorker = static_cast<CWorker*>(iter->second);
			if (!pWorker->GetMineral())
			{
				// Identify a mineral it can harvest
				for (miterMineralsList = mpMineralsList.begin(); miterMineralsList != mpMineralsList.end(); miterMineralsList++)
				{
					// If the mineral is not in use, harvest from it with the currently selected worker
					if (!(*miterMineralsList)->IsBeingUsed())
					{
						// Mineral not being used, so harvest from it
						pWorker->SetMineral((*miterMineralsList));
						(*miterMineralsList)->SetUsage(true);
						pWorker->SetPathTarget((*miterMineralsList)->GetWorldPos());
						break;
					}
				}
			}
		}
	}
}

bool CRTSAIPlayer::ResolveItem(EQueueObjectType qObject)
{
	int lowest = MAX_QUEUE_SIZE;
	int qSize = 0;

	// Building and agent pointer
	CStructure* pStructure = nullptr;
	int agentNum = -1;

	CTile* pSpawnTile = nullptr;
	
	// Check if the item is a building or an agent
	switch (qObject)
	{
	case Q_BARRACKS:
		pStructure = new CBarracks();
		break;

	case Q_HELLIPAD:
		pStructure = new CHellipad();
		break;

	case Q_SPACE_CENTRE:
		pStructure = new CSpaceCentre();
		break;

	case Q_HOUSE:
		// NOT YET IMPLEMENTED
		return true;
		break;

	case Q_INFANTRY:
		{
			// Infantry is from barracks - get all instances of barracks	
			auto range = mpStructuresMap.equal_range(STR_BARRACKS);

			// Check that some structures exist
			if (range.first == mpStructuresMap.end())
			{
				// Move the item down in priority
				DecreaseTopItem();
				// Add request for a barracks as top priority
				mpTaskQ.push(new CBuildRequest(Q_BARRACKS, 1));
				return false;
			}

			for (auto iter = range.first; iter != range.second; ++iter)
			{
				// Find one with the lowest queue
				qSize = iter->second->GetQueueSize();
				if (qSize < lowest)
				{
					lowest = qSize;
					pStructure = iter->second;
				}
			}

			// Check if the lowest is equal to the maximum queue size
			if (lowest == MAX_QUEUE_SIZE)
			{
				// There are no available buildings - construct more!
				// Move the item down in priority
				DecreaseTopItem();
				// Add request for a barracks as top priority
				mpTaskQ.push(new CBuildRequest(Q_BARRACKS, 1));
				return false;
			}
			else if (lowest >= MAX_QUEUE_SIZE / 2)
			{
				// Buildings are strained - construct more
				// Add request for a barracks, but not at huge priority
				mpTaskQ.push(new CBuildRequest(Q_BARRACKS, 8));
			}

			// Index of the agent to be produced
			agentNum = 0;
		}
		break;

	case Q_ARTILLERY:
		{
			// Artillery is from barracks - get all instances of barracks
			auto range = mpStructuresMap.equal_range(STR_BARRACKS);

			// Check that at least one structure exists
			if (range.first == mpStructuresMap.end())
			{
				// Move the item down in priority
				DecreaseTopItem();
				// Add request for a barracks as top priority
				mpTaskQ.push(new CBuildRequest(Q_BARRACKS, 1));
				return false;
			}

			for (auto iter = range.first; iter != range.second; ++iter)
			{
				// Find one with the lowest queue
				qSize = iter->second->GetQueueSize();
				if (qSize < lowest)
				{
					lowest = qSize;
					pStructure = iter->second;
				}
			}

			// Check if the lowest is equal to the maximum queue size
			if (lowest == MAX_QUEUE_SIZE)
			{
				// There are no available buildings - construct more!
				// Move the item down in priority
				DecreaseTopItem();
				// Add request for a barracks as top priority
				mpTaskQ.push(new CBuildRequest(Q_BARRACKS, 1));
				return false;
			}
			else if (lowest >= MAX_QUEUE_SIZE / 2)
			{
				// Buildings are strained - construct more
				// Add request for a barracks, but not at huge priority
				mpTaskQ.push(new CBuildRequest(Q_BARRACKS, 8));
			}

			// Index of the agent to be produced
			agentNum = 1;
		}
		break;

	case Q_TANK:
		{
			// Tank is from barracks - get all instances of barracks
			auto range = mpStructuresMap.equal_range(STR_BARRACKS);

			// Check that at least one structure exists
			if (range.first == mpStructuresMap.end())
			{
				// Move the item down in priority
				DecreaseTopItem();
				// Add request for a barracks as top priority
				mpTaskQ.push(new CBuildRequest(Q_BARRACKS, 1));
				return false;
			}

			for (auto iter = range.first; iter != range.second; ++iter)
			{
				// Find one with the lowest queue
				qSize = iter->second->GetQueueSize();
				if (qSize < lowest)
				{
					lowest = qSize;
					pStructure = iter->second;
				}
			}

			// Check if the lowest is equal to the maximum queue size
			if (lowest == MAX_QUEUE_SIZE)
			{
				// There are no available buildings - construct more!
				// Move the item down in priority
				DecreaseTopItem();
				// Add request for a barracks as top priority
				mpTaskQ.push(new CBuildRequest(Q_BARRACKS, 1));
				return false;
			}
			else if (lowest >= MAX_QUEUE_SIZE / 2)
			{
				// Buildings are strained - construct more
				// Add request for a barracks, but not at huge priority
				mpTaskQ.push(new CBuildRequest(Q_BARRACKS, 8));
			}

			// Index of the agent to be produced
			agentNum = 2;
		}
		break;

	case Q_MOTHERSHIP:
		{
			// Tank is from barracks - get all instances of barracks
			auto range = mpStructuresMap.equal_range(STR_SPACE_CENTRE);

			// Check that at least one structure exists
			if (range.first == mpStructuresMap.end())
			{
				// Move the item down in priority
				DecreaseTopItem();
				// Add request for a barracks as top priority
				mpTaskQ.push(new CBuildRequest(Q_SPACE_CENTRE, 1));
				return false;
			}

			for (auto iter = range.first; iter != range.second; ++iter)
			{
				// Find one with the lowest queue
				qSize = iter->second->GetQueueSize();
				if (qSize < lowest)
				{
					lowest = qSize;
					pStructure = iter->second;
				}
			}

			// Check if the lowest is equal to the maximum queue size
			if (lowest == MAX_QUEUE_SIZE)
			{
				// There are no available buildings - construct more!
				// Move the item down in priority
				DecreaseTopItem();
				// Add request for a barracks as top priority
				mpTaskQ.push(new CBuildRequest(Q_SPACE_CENTRE, 1));
				return false;
			}
			else if (lowest >= MAX_QUEUE_SIZE / 2)
			{
				// Buildings are strained - construct more
				// Add request for a barracks, but not at huge priority
				mpTaskQ.push(new CBuildRequest(Q_SPACE_CENTRE, 8));
			}

			// Index of the agent to be produced
			agentNum = 2;
		}

		break;

	case Q_TRANSPORT:
		{
			// Tank is from barracks - get all instances of barracks
			auto range = mpStructuresMap.equal_range(STR_SPACE_CENTRE);

			// Check that at least one structure exists
			if (range.first == mpStructuresMap.end())
			{
				// Move the item down in priority
				DecreaseTopItem();
				// Add request for a barracks as top priority
				mpTaskQ.push(new CBuildRequest(Q_SPACE_CENTRE, 1));
				return false;
			}

			for (auto iter = range.first; iter != range.second; ++iter)
			{
				// Find one with the lowest queue
				qSize = iter->second->GetQueueSize();
				if (qSize < lowest)
				{
					lowest = qSize;
					pStructure = iter->second;
				}
			}

			// Check if the lowest is equal to the maximum queue size
			if (lowest == MAX_QUEUE_SIZE)
			{
				// There are no available buildings - construct more!
				// Move the item down in priority
				DecreaseTopItem();
				// Add request for a barracks as top priority
				mpTaskQ.push(new CBuildRequest(Q_SPACE_CENTRE, 1));
				return false;
			}
			else if (lowest >= MAX_QUEUE_SIZE / 2)
			{
				// Buildings are strained - construct more
				// Add request for a barracks, but not at huge priority
				mpTaskQ.push(new CBuildRequest(Q_SPACE_CENTRE, 8));
			}

			// Index of the agent to be produced
			agentNum = 1;
		}

		break;

	case Q_SPACE_FIGHTER:
		{
			// Tank is from barracks - get all instances of barracks
			auto range = mpStructuresMap.equal_range(STR_SPACE_CENTRE);

			// Check that at least one structure exists
			if (range.first == mpStructuresMap.end())
			{
				// Move the item down in priority
				DecreaseTopItem();
				// Add request for a barracks as top priority
				mpTaskQ.push(new CBuildRequest(Q_SPACE_CENTRE, 1));
				return false;
			}

			for (auto iter = range.first; iter != range.second; ++iter)
			{
				// Find one with the lowest queue
				qSize = iter->second->GetQueueSize();
				if (qSize < lowest)
				{
					lowest = qSize;
					pStructure = iter->second;
				}
			}

			// Check if the lowest is equal to the maximum queue size
			if (lowest == MAX_QUEUE_SIZE)
			{
				// There are no available buildings - construct more!
				// Move the item down in priority
				DecreaseTopItem();
				// Add request for a barracks as top priority
				mpTaskQ.push(new CBuildRequest(Q_SPACE_CENTRE, 1));
				return false;
			}
			else if (lowest >= MAX_QUEUE_SIZE / 2)
			{
				// Buildings are strained - construct more
				// Add request for a barracks, but not at huge priority
				mpTaskQ.push(new CBuildRequest(Q_SPACE_CENTRE, 8));
			}

			// Index of the agent to be produced
			agentNum = 0;
		}

		break;

	case Q_WORKER:
		{
			// Tank is from barracks - get all instances of barracks
			auto range = mpStructuresMap.equal_range(STR_COM_CENTRE);

			// Check that at least one structure exists
			if (range.first == mpStructuresMap.end())
			{
				// Move the item down in priority
				DecreaseTopItem();
				return false;
			}

			for (auto iter = range.first; iter != range.second; ++iter)
			{
				// Find one with the lowest queue
				qSize = iter->second->GetQueueSize();
				if (qSize < lowest)
				{
					lowest = qSize;
					pStructure = iter->second;
				}
			}

			// Check if the lowest is equal to the maximum queue size
			if (lowest == MAX_QUEUE_SIZE)
			{
				// Move the item down in priority
				DecreaseTopItem();
				return false;
			}

			// Index of the agent to be produced
			agentNum = 0;
		}

		break;

	case Q_FIGHTER:
		{
			// Tank is from barracks - get all instances of barracks
			auto range = mpStructuresMap.equal_range(STR_HELLIPAD);

			// Check that at least one structure exists
			if (range.first == mpStructuresMap.end())
			{
				// Move the item down in priority
				DecreaseTopItem();
				// Add request for a barracks as top priority
				mpTaskQ.push(new CBuildRequest(Q_HELLIPAD, 1));
				return false;
			}

			for (auto iter = range.first; iter != range.second; ++iter)
			{
				// Find one with the lowest queue
				qSize = iter->second->GetQueueSize();
				if (qSize < lowest)
				{
					lowest = qSize;
					pStructure = iter->second;
				}
			}

			// Check if the lowest is equal to the maximum queue size
			if (lowest == MAX_QUEUE_SIZE)
			{
				// There are no available buildings - construct more!
				// Move the item down in priority
				DecreaseTopItem();
				// Add request for a barracks as top priority
				mpTaskQ.push(new CBuildRequest(Q_HELLIPAD, 1));
				return false;
			}
			else if (lowest >= MAX_QUEUE_SIZE / 2)
			{
				// Buildings are strained - construct more
				// Add request for a barracks, but not at huge priority
				mpTaskQ.push(new CBuildRequest(Q_HELLIPAD, 8));
			}

			// Index of the agent to be produced
			agentNum = 0;
		}

		break;

	case Q_BOMBER:
		{
			// Tank is from barracks - get all instances of barracks
			auto range = mpStructuresMap.equal_range(STR_HELLIPAD);

			// Check that at least one structure exists
			if (range.first == mpStructuresMap.end())
			{
				// Move the item down in priority
				DecreaseTopItem();
				// Add request for a barracks as top priority
				mpTaskQ.push(new CBuildRequest(Q_HELLIPAD, 1));
				return false;
			}

			for (auto iter = range.first; iter != range.second; ++iter)
			{
				// Find one with the lowest queue
				qSize = iter->second->GetQueueSize();
				if (qSize < lowest)
				{
					lowest = qSize;
					pStructure = iter->second;
				}
			}

			// Check if the lowest is equal to the maximum queue size
			if (lowest == MAX_QUEUE_SIZE)
			{
				// There are no available buildings - construct more!
				// Move the item down in priority
				DecreaseTopItem();
				// Add request for a barracks as top priority
				mpTaskQ.push(new CBuildRequest(Q_HELLIPAD, 1));
				return false;
			}
			else if (lowest >= MAX_QUEUE_SIZE / 2)
			{
				// Buildings are strained - construct more
				// Add request for a barracks, but not at huge priority
				mpTaskQ.push(new CBuildRequest(Q_HELLIPAD, 8));
			}

			// Index of the agent to be produced
			agentNum = 1;
		}

		break;

	case Q_CHANGE_TACTIC:
		{
			// Generate a random number from 1 - 3 to determine which tactic to use
			int tactic = mpRandomiser->GetRandomInt((int)Tactics::None, (int)Tactics::Rapid);

			// Determine chosen tactic & set it
			Tactics selTactic = (Tactics)tactic;
			mpFleet->SetTactic(selTactic);

			return true;
			break;
		}
	case Q_MOVE_UNIT:
		{
			// If there are enough units
			int size = mpUnitsMap.size();
			if (size == 0)
			{
				return false;
			}

			// Pick a random unit and move it to an arbitrary location
			int unitNum = mpRandomiser->GetRandomInt(0, size - 1);

			miterUnitsMap = mpUnitsMap.begin();
			std::advance(miterUnitsMap, unitNum);

			// Check to see if the unit picked is a worker unit which is busy harvesting
			if (miterUnitsMap->second->GetAgentData()->mAgentType == GAV_WORKER)
			{
				CWorker* pWorker = static_cast<CWorker*>(miterUnitsMap->second);
				if (pWorker->GetMineral())
				{
					// There is a mineral as a target - leave
					return true;
				}
			}

			// Pick a random location to move the unit to
			DX::XMFLOAT3 newPos;

			DX::XMFLOAT3 start = mpPlayerGrid->GetGridStartPos();
			DX::XMFLOAT3 end = mpPlayerGrid->GetGridEndPos();

			newPos.x = mpRandomiser->GetRandomFloat(start.x, end.x);
			newPos.z = mpRandomiser->GetRandomFloat(start.z, end.z);

			miterUnitsMap->second->SetPathTarget(newPos);

			return true;
			break;
		}
	case Q_MOVE_UNIT_GROUP:
		{
			// If there are enough units
			int size = mpUnitsMap.size();
			if (size < 4)
			{
				return false;
			}

			// Pick a random number of units to select - minimum of 3
			int num = mpRandomiser->GetRandomInt(2, size - 1);
			int unit = 0;

			// Loop through 'num' amount of times to select that many units
			for (int i = 0; i < num; i++)
			{
				// Selected a random unit and add it to the selected list of units
				unit = mpRandomiser->GetRandomInt(0, size - 1);
				miterUnitsMap = mpUnitsMap.begin();
				std::advance(miterUnitsMap, unit);

				mpSelectedAgents.push_back(miterUnitsMap->second);
			}

			// Pick a random location to move the unit to
			DX::XMFLOAT3 newPos;

			DX::XMFLOAT3 start = mpPlayerGrid->GetGridStartPos();
			DX::XMFLOAT3 end = mpPlayerGrid->GetGridEndPos();

			newPos.x = mpRandomiser->GetRandomFloat(start.x, end.x);
			newPos.z = mpRandomiser->GetRandomFloat(start.z, end.z);

			// Loop through the selected agents and set their new position
			for (miterSelectedAgents = mpSelectedAgents.begin(); miterSelectedAgents != mpSelectedAgents.end(); miterSelectedAgents++)
			{
				// Check to see if the unit picked is a worker unit which is busy harvesting
				if (miterUnitsMap->second->GetAgentData()->mAgentType == GAV_WORKER)
				{
					CWorker* pWorker = static_cast<CWorker*>(miterUnitsMap->second);
					if (!pWorker->GetMineral())
					{
						// There is no mineral as a target - safe to move. WILL LATER CHECK FOR OTHER THINGS THE WORKER COULD BE DOING
						(*miterSelectedAgents)->SetPathTarget(newPos);
					}
				}
				else
				{
					// Every other unit just move - WILL CHANGE LATER FOR COMBAT PURPOSES
					(*miterSelectedAgents)->SetPathTarget(newPos);
				}
			}

			// Clear the selected agents list
			mpSelectedAgents.clear();

			return true;
			break;
		}
	}

	// If it was a unit being built, simply build unit & return true (for now)
	if (pStructure && agentNum != -1)
	{
		// Check if the structure is fully constructed
		if (pStructure->AddToQueue(agentNum, this) == ERR_NONE)
		{
			return true;
		}

		// Building not yet ready - simply return false
		return false;
	}

	// Get a random grid position in which to spawn the unit & get the respective tile
	int gridX = mpRandomiser->GetRandomInt(1, GRID_SIZE_X - 1);
	int gridY = mpRandomiser->GetRandomInt(1, GRID_SIZE_Y - 1);

	pSpawnTile = mpPlayerGrid->GetTileData(SPointData(gridX, gridY));
	pStructure->SetWorldPos(pSpawnTile->GetWorldPos());

	// Check if the tile is already being used
	if (pSpawnTile->IsTileUsed())
	{
		pStructure->UnloadIModel();
		SafeDelete(pStructure);
		return false;
	}

	// Attempt to create the object
	// Check for sufficient funds *****
	// Check success of building purchase
	if (PurchaseStructure(pStructure, mpPlayerGrid, pSpawnTile) != ERR_NONE)
	{
		pStructure->UnloadIModel();
		SafeDelete(pStructure);
		return false;
	}

	// SUCCESS
	return true;
}

void CRTSAIPlayer::AssessSituation()
{
	int task = 0;
	int priority = 0;
	int numRepeat = mpRandomiser->GetRandomInt(MIN_NEW_ITEMS, MAX_NEW_ITEMS);
	
	// Repeat this function a random number of times (influenced by difficulty level)
	for (int i = 0; i < numRepeat; i++)
	{
		// First check if the economy is at a reasonable level
		if (mNumMinerals < 1500 || mPopLimit <= mCurPop)
		{
			// FUTURE: Calculate relationship between all unit types to determine which to get?
			// not enough minerals - choose an option that does not reqiure funds
			task = mpRandomiser->GetRandomInt(static_cast<int>(Q_MOVE_UNIT), static_cast<int>(Q_CHANGE_TACTIC));
			priority = mpRandomiser->GetRandomInt(2, 50);
			mpTaskQ.push(new CBuildRequest(static_cast<EQueueObjectType>(task), priority));
		}
		else
		{
			// Get a random request
			task = mpRandomiser->GetRandomInt(static_cast<int>(Q_FIGHTER), static_cast<int>(Q_CHANGE_TACTIC));
			priority = mpRandomiser->GetRandomInt(2, 40);
			mpTaskQ.push(new CBuildRequest(static_cast<EQueueObjectType>(task), priority));
		}
	}

	// Assess all worker units
	AssessWorkers();
}

void CRTSAIPlayer::DecreaseTopItem()
{
	// Cannot change data already in a map - have to remove it, change it, then add it back in
	CBuildRequest* pRequest = mpTaskQ.top();
	mpTaskQ.pop();
	pRequest->DecreasePriority();
	mpTaskQ.push(pRequest);
}
