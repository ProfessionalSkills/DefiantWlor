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
CRTSAIPlayer::CRTSAIPlayer(EFactions playerFaction) : CRTSPlayer(playerFaction), UPDATE_TIME(0.1f)
{
	mpRandomiser = new CRandomiser();

	// Initialise first 10 tasks of AI player
	mpTaskQ.push(new CBuildRequest(Q_WORKER, 50));
	mpTaskQ.push(new CBuildRequest(Q_WORKER, 50));
	mpTaskQ.push(new CBuildRequest(Q_WORKER, 50));
	mpTaskQ.push(new CBuildRequest(Q_WORKER, 50));


	// Set default mUpdateTime
	mUpdateTime = UPDATE_TIME;
	mWaitTime = mpRandomiser->GetRandomFloat(1.0f, 3.0f);
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
		mWaitTime = mpRandomiser->GetRandomFloat(1.0f, 3.0f);
	}
	else
	{
		mWaitTime -= gFrameTime;
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
	}

	// If it was a unit being built, simply build unit & return true (for now)
	if (pStructure && agentNum != -1)
	{
		// Check if the structure is fully constructed
		if (pStructure->GetState() != OBJ_CONSTRUCTING)
		{
			pStructure->AddToQueue(agentNum);
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
	if (!PurchaseStructure(pStructure, mpPlayerGrid, pSpawnTile))
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
	// First check if the economy is at a reasonable level
	if (mNumMinerals < 2000)
	{
		// No need to continue enough
		return;
	}

	// FUTURE: Calculate relationship between all unit types to determine which to get?

	// Get a random request
	int object = mpRandomiser->GetRandomInt(static_cast<int>(Q_FIGHTER), static_cast<int>(Q_NUM) - 1);
	int priority = mpRandomiser->GetRandomInt(5, 50);
	mpTaskQ.push(new CBuildRequest(static_cast<EQueueObjectType>(object), priority));
}

void CRTSAIPlayer::DecreaseTopItem()
{
	// Cannot change data already in a map - have to remove it, change it, then add it back in
	CBuildRequest* pRequest = mpTaskQ.top();
	mpTaskQ.pop();
	pRequest->DecreasePriority();
	mpTaskQ.push(pRequest);
}
