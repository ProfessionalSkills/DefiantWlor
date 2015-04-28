//-----------------------------------------------------
// FILE: AIPlayer.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "AIPlayer.h"
#include "GameStateControl.h"


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

		// Set the minimum amount of cash needed before the player attempts to do construction
		MIN_CASH_FOR_UPDATE = 2000;
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

		// Set the minimum amount of cash needed before the player attempts to do construction
		MIN_CASH_FOR_UPDATE = 1750;
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

		// Set the minimum amount of cash needed before the player attempts to do construction
		MIN_CASH_FOR_UPDATE = 1500;
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
		mpTaskQ.push(new CBuildRequest(Q_WORKER, 1));
		mpTaskQ.push(new CBuildRequest(Q_WORKER, 1));
		mpTaskQ.push(new CBuildRequest(Q_CHANGE_TACTIC, 2));
		mpTaskQ.push(new CBuildRequest(Q_INFANTRY, 2));
		mpTaskQ.push(new CBuildRequest(Q_FIGHTER, 4));
		mpTaskQ.push(new CBuildRequest(Q_SPACE_FIGHTER, 60));
		mpTaskQ.push(new CBuildRequest(Q_MOTHERSHIP, 80));

		// Set the minimum amount of cash needed before the player attempts to do construction
		MIN_CASH_FOR_UPDATE = 1000;
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
		if (mpTaskQ.size() > 50)
		{
			// Too many items in the list - clear it
			for (int i = 0; i < 45; i++)
			{
				// Remove most of the items
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
	int numWorkers = 0;
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
					// Cache mineral
					CMinerals* pMineral = (*miterMineralsList);
					
					// If the mineral is not in use, harvest from it with the currently selected worker
					if (!pMineral->IsBeingUsed())
					{
						// Mineral not being used, so harvest from it
						pWorker->SetMineral(pMineral);
						pMineral->SetUsage(true);
						break;
					}
				}
			}

			// Increment number of workers
			numWorkers++;
		}
	}

	// Check how many workers there are
	if (numWorkers < 6)
	{
		mpTaskQ.push(new CBuildRequest(Q_WORKER, 1));
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
				// Add request for a barracks as top priority
				mpTaskQ.push(new CBuildRequest(Q_BARRACKS, 1));

				// Move the item down in priority
				return DecreaseTopItem();
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
				// Add request for a barracks as top priority
				mpTaskQ.push(new CBuildRequest(Q_BARRACKS, 1));
				// Move the item down in priority
				return DecreaseTopItem();
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
				// Add request for a barracks as top priority
				mpTaskQ.push(new CBuildRequest(Q_BARRACKS, 1));
				// Move the item down in priority
				return DecreaseTopItem();
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
				// Add request for a barracks as top priority
				mpTaskQ.push(new CBuildRequest(Q_BARRACKS, 1));
				// Move the item down in priority
				return DecreaseTopItem();
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
				// Add request for a barracks as top priority
				mpTaskQ.push(new CBuildRequest(Q_BARRACKS, 1));
				// Move the item down in priority
				return DecreaseTopItem();
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
				// Add request for a barracks as top priority
				mpTaskQ.push(new CBuildRequest(Q_BARRACKS, 1));
				// Move the item down in priority
				return DecreaseTopItem();
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
				// Add request for a barracks as top priority
				mpTaskQ.push(new CBuildRequest(Q_SPACE_CENTRE, 1));
				// Move the item down in priority
				return DecreaseTopItem();
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
				// Add request for a barracks as top priority
				mpTaskQ.push(new CBuildRequest(Q_SPACE_CENTRE, 1));
				// Move the item down in priority
				return DecreaseTopItem();
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
				// Add request for a barracks as top priority
				mpTaskQ.push(new CBuildRequest(Q_SPACE_CENTRE, 1));
				// Move the item down in priority
				return DecreaseTopItem();
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
				// Add request for a barracks as top priority
				mpTaskQ.push(new CBuildRequest(Q_SPACE_CENTRE, 1));
				// Move the item down in priority
				return DecreaseTopItem();
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
				// Add request for a barracks as top priority
				mpTaskQ.push(new CBuildRequest(Q_SPACE_CENTRE, 1));
				// Move the item down in priority
				return DecreaseTopItem();
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
				// Add request for a barracks as top priority
				mpTaskQ.push(new CBuildRequest(Q_SPACE_CENTRE, 1));
				// Move the item down in priority
				return DecreaseTopItem();
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
			// Check how many workers the player already has
			auto workers = mpUnitsMap.equal_range(GAV_WORKER);

			// Check that some structures exist
			int numWorkers = 0;
			if (workers.first != mpUnitsMap.end())
			{
				// Loop through each worker unit & increment the counter
				for (auto iter = workers.first; iter != workers.second; ++iter)
				{
					// Increment number of workers
					numWorkers++;
				}
			}
			
			if (numWorkers > 6)
			{
				return true;
			}

			// Tank is from barracks - get all instances of barracks
			auto range = mpStructuresMap.equal_range(STR_COM_CENTRE);

			// Check that at least one structure exists
			if (range.first == mpStructuresMap.end())
			{
				// Move the item down in priority
				return DecreaseTopItem();
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
				return DecreaseTopItem();
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
				// Add request for a barracks as top priority
				mpTaskQ.push(new CBuildRequest(Q_HELLIPAD, 1));
				// Move the item down in priority
				return DecreaseTopItem();
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
				// Add request for a barracks as top priority
				mpTaskQ.push(new CBuildRequest(Q_HELLIPAD, 1));
				// Move the item down in priority
				return DecreaseTopItem();
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
				// Add request for a barracks as top priority
				mpTaskQ.push(new CBuildRequest(Q_HELLIPAD, 1));
				// Move the item down in priority
				return DecreaseTopItem();
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
				// Add request for a barracks as top priority
				mpTaskQ.push(new CBuildRequest(Q_HELLIPAD, 1));
				// Move the item down in priority
				return DecreaseTopItem();
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

			// Check if the unit is in another airspace
			if (miterUnitsMap->second->GetAirspacePosition() != AS_MARS)
			{
				return true;
			}

			// Pick a random location to move the unit to
			DX::XMFLOAT3 newPos;

			DX::XMFLOAT3 start = mpPlayerGrid->GetGridStartPos();
			DX::XMFLOAT3 end = mpPlayerGrid->GetGridEndPos();

			newPos.x = mpRandomiser->GetRandomFloat(start.x, end.x);
			newPos.z = mpRandomiser->GetRandomFloat(start.z, end.z);

			// Check if the unit has an attack target and is therefore busy
			if (!miterUnitsMap->second->GetAttackTarget())
			{
				miterUnitsMap->second->SetPathTarget(newPos);
			}

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
				// Cache unit
				CGameAgent* pSelAgent = (*miterSelectedAgents);
				
				// Check to see if the unit picked is a worker unit which is busy harvesting
				if (pSelAgent->GetAgentData()->mAgentType == GAV_WORKER)
				{
					CWorker* pWorker = static_cast<CWorker*>(pSelAgent);
					if (!pWorker->GetMineral())
					{
						// There is no mineral as a target - safe to move. WILL LATER CHECK FOR OTHER THINGS THE WORKER COULD BE DOING
						pWorker->SetPathTarget(newPos);
					}
				}
				else if (pSelAgent->GetAirspacePosition() == AS_MARS)
				{
					// Check if the unit has an attack target and is therefore busy
					if (!pSelAgent->GetAttackTarget())
					{
						pSelAgent->SetPathTarget(newPos);
					}
				}
			}

			// Clear the selected agents list
			mpSelectedAgents.clear();

			return true;
			break;
		}
	case Q_SEND_TO_SPACE:
		{
			// Count how many units are not in space & not a worker
			int counter = 0;
			for (miterUnitsMap = mpUnitsMap.begin(); miterUnitsMap != mpUnitsMap.end(); miterUnitsMap++)
			{
				// Check if in space
				if (miterUnitsMap->second->GetState() != OBJ_INSPACE && miterUnitsMap->second->GetAgentData()->mAgentType != GAV_WORKER)
				{
					counter++;
				}
			}

			// Ensure there are at least 5 units in the world
			if (counter < 5)
			{
				// Return true to remove this as a task for the AI player
				return true;
			}
			
			// If there are enough units
			int size = mpUnitsMap.size();
			if (size == 0)
			{
				return true;
			}

			// Pick a random unit and move it to an arbitrary location
			int unitNum = mpRandomiser->GetRandomInt(0, size - 1);

			miterUnitsMap = mpUnitsMap.begin();
			std::advance(miterUnitsMap, unitNum);

			// Check to see if the unit picked is a worker unit which is busy harvesting
			if (miterUnitsMap->second->GetAgentData()->mAgentType == GAV_WORKER)
			{
				// Cannot send workers to space
				return true;
			}

			// Check if the unit is already in space
			if (miterUnitsMap->second->GetState() == OBJ_INSPACE)
			{
				// Try a different unit
				return true;
			}

			// if the unit is attacking, leave it alone
			if (miterUnitsMap->second->GetAttackTarget())
			{
				return true;
			}

			// Check if the unit is already on earth
			if (miterUnitsMap->second->GetAirspacePosition() == AS_EARTH)
			{
				return true;
			}

			// Get unit's position before it disappears
			DX::XMFLOAT3 pos = miterUnitsMap->second->GetWorldPos();

			// Attempt to put the unit into space
			if (PutUnitsOnShips(miterUnitsMap->second))
			{
				// Add new beam at the unit's position
				pPlayerManager->NewSpawnBeam(pos);
				// Success
				return true;
			}
			else
			{
				// Failed to put unit into space
				// Queue a new transport ship
				mpTaskQ.push(new CBuildRequest(Q_TRANSPORT, 1));
				return false;
			}
		}
		break;
	case Q_LAUNCH_FLEET:
		{
			// Player won previous battle
			if (mWonLastSpaceBattle)
			{
				return true;
			}
			
			// Check size of fleet
			if (mpSpaceUnitsList.size() < 9)
			{
				// Queue more space units
				mpTaskQ.push(new CBuildRequest(Q_TRANSPORT, 1));
				mpTaskQ.push(new CBuildRequest(Q_TRANSPORT, 1));
				mpTaskQ.push(new CBuildRequest(Q_SPACE_FIGHTER, 1));
				mpTaskQ.push(new CBuildRequest(Q_SPACE_FIGHTER, 1));
				mpTaskQ.push(new CBuildRequest(Q_SPACE_FIGHTER, 1));
				mpTaskQ.push(new CBuildRequest(Q_SPACE_FIGHTER, 1));

				return true;
			}

			// Check if there is at least one transport ship & mothership
			bool mothership = false;
			bool transport = false;
			for (auto iter = mpSpaceUnitsList.begin(); iter != mpSpaceUnitsList.end(); iter++)
			{
				// Check motherships
				if (!mothership)
				{
					if ((*iter)->GetAgentData()->mAgentType == GAV_MOTHERSHIP)
					{
						mothership = true;
					}
				}
				// Check transport
				if (!transport)
				{
					if ((*iter)->GetAgentData()->mAgentType == GAV_TRANSPORT)
					{
						transport = true;

						// Check for cargo
						CTransport* pCargo = static_cast<CTransport*>((*iter));
						if (!pCargo->GetCargoValue())
						{
							mpTaskQ.push(new CBuildRequest(Q_SEND_TO_SPACE, 1));
							mpTaskQ.push(new CBuildRequest(Q_SEND_TO_SPACE, 1));
							mpTaskQ.push(new CBuildRequest(Q_SEND_TO_SPACE, 1));
							mpTaskQ.push(new CBuildRequest(Q_SEND_TO_SPACE, 1));
							mpTaskQ.push(new CBuildRequest(Q_SEND_TO_SPACE, 1));
							transport = false;
						}
					}
				}

				if (transport && mothership)
				{
					break;
				}
			}

			// If either of those are still false, return & make more units
			if (!mothership)
			{
				mpTaskQ.push(new CBuildRequest(Q_MOTHERSHIP, 1));
				return true;
			}
			if (!transport)
			{
				mpTaskQ.push(new CBuildRequest(Q_TRANSPORT, 1));
				return true;
			}

			// Send attack
			pPlayerManager->SetAIPlayerLaunchedAttack(true);
			LaunchAttack();
			return true;
		}
		break;
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
		if (mNumMinerals < MIN_CASH_FOR_UPDATE || mPopLimit <= mCurPop)
		{
			// FUTURE: Calculate relationship between all unit types to determine which to get?
			// not enough minerals - choose an option that does not reqiure funds
			task = mpRandomiser->GetRandomInt(static_cast<int>(Q_MOVE_UNIT), static_cast<int>(Q_CHANGE_TACTIC));
			priority = mpRandomiser->GetRandomInt(10, 50);
			mpTaskQ.push(new CBuildRequest(static_cast<EQueueObjectType>(task), priority));
		}
		else
		{
			// Get a random request
			task = mpRandomiser->GetRandomInt(static_cast<int>(Q_FIGHTER), static_cast<int>(Q_CHANGE_TACTIC));
			priority = mpRandomiser->GetRandomInt(10, 40);
			mpTaskQ.push(new CBuildRequest(static_cast<EQueueObjectType>(task), priority));
		}
	}

	// Assess all worker units
	AssessWorkers();
}

bool CRTSAIPlayer::DecreaseTopItem()
{
	// Cannot change data already in a map - have to remove it, change it, then add it back in
	CBuildRequest* pRequest = mpTaskQ.top();
	mpTaskQ.pop();
	pRequest->DecreasePriority();
	mpTaskQ.push(pRequest);
	return pRequest->GetRejections() > 2;
}
