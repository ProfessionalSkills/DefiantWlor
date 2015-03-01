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
CRTSAIPlayer::CRTSAIPlayer(EFactions playerFaction) : CRTSPlayer(playerFaction), UPDATE_TIME(5.0f)
{
	// Initialise first 10 tasks of AI player
	mpTaskQ.push(new CBuildRequest(Q_BARRACKS, 20));
	mpTaskQ.push(new CBuildRequest(Q_HELLIPAD, 15));
	mpTaskQ.push(new CBuildRequest(Q_SPACE_CENTRE, 30));

	// Set default mUpdateTime
	mUpdateTime = UPDATE_TIME;
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
	if (mUpdateTime <= 0)
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
}

bool CRTSAIPlayer::ResolveItem(EQueueObjectType qObject)
{
	// Building and agent pointer
	CStructure* pStructure = nullptr;
	CGameAgent* pGameAgent = nullptr;

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
	}

	// Get a random grid position in which to spawn the unit & get the respective tile
	CRandomiser randomiser;
	int gridX = randomiser.GetRandomInt(1, GRID_SIZE_X - 1);
	int gridY = randomiser.GetRandomInt(1, GRID_SIZE_Y - 1);

	pSpawnTile = mpPlayerGrid->GetTileData(SPointData(gridX, gridY));

	// Check if the tile is already being used
	if (pSpawnTile->IsTileUsed())
	{
		return false;
	}

	// Attempt to create the object
	// Check if unit or building ******
	// Check for sufficient funds *****
	// Check success of building purchase
	if (!PurchaseStructure(pStructure, mpPlayerGrid, pSpawnTile))
	{
		return false;
	}

	// SUCCESS
	return true;
}
