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
		if (ResolveItem(mpTaskQ.top()->GetObjectType()))
		{
			// If item was resolved, remove it
			CBuildRequest* pRequest = mpTaskQ.top();
			SafeDelete(pRequest);
			mpTaskQ.pop();
		}

		// Reset update time
		mUpdateTime = UPDATE_TIME;
	}
	else
	{
		// Update the time until the AI is able to do another action
		mUpdateTime -= gFrameTime;
	}
}

bool CRTSAIPlayer::ResolveItem(EQueueObjectType qObject)
{
	// Check if the item is a building or an agent
	return false;
}
