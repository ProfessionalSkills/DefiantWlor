//-----------------------------------------------------
// FILE: AIPlayer.h
//-----------------------------------------------------

#ifndef _AI_PLAYER_H_
#define _AI_PLAYER_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Player.h"


//-----------------------------------------------------
// AI PLAYER CLASS : CHILD OF BASE PLAYER CLASS
//-----------------------------------------------------
class CRTSAIPlayer : public CRTSPlayer
{
public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CRTSAIPlayer(EFactions playerFaction);
	~CRTSAIPlayer();


	// ACCESSORS
	//---------------------------


	// MUTATORS
	//---------------------------


	// METHODS
	//---------------------------
	void Update();


private:
	// PRIORIY QUEUE OF TASKS
	//---------------------------
	
};


#endif /* _AI_PLAYER_H_ */
