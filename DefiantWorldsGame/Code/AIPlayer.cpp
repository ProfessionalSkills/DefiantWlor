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
CRTSAIPlayer::CRTSAIPlayer(EFactions playerFaction) : CRTSPlayer(playerFaction)
{
	// Initialise first 10 tasks of AI player
	mpTaskQ.push(new CBuildRequest((EQueueObjectType)0x103, 20));
	unsigned int ti = mpTaskQ.top()->GetObjectType();
	EQueueObjectType t = (EQueueObjectType)ti;
}

CRTSAIPlayer::~CRTSAIPlayer()
{

}


void CRTSAIPlayer::Update()
{
	// Call the base player's version of update first
	CRTSPlayer::Update();

	// Do any AI specific updates
}
