//-----------------------------------------------------
// FILE: AIPlayer.h
//-----------------------------------------------------

#ifndef _AI_PLAYER_H_
#define _AI_PLAYER_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Player.h"
#include "BuildRequest.h"

struct SSortByPriority
{
	bool operator() (CBuildRequest* lhs, CBuildRequest* rhs) const
	{
		return lhs->GetObjectPriority() > rhs->GetObjectPriority();
	}
};


//-----------------------------------------------------
// AI PLAYER CLASS : CHILD OF BASE PLAYER CLASS
//-----------------------------------------------------
class CRTSAIPlayer : public CRTSPlayer
{
public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CRTSAIPlayer(EFactions playerFaction, int startingResources, int difficulty);
	~CRTSAIPlayer();


	// ACCESSORS
	//---------------------------



	// MUTATORS
	//---------------------------



	// METHODS
	//---------------------------
	// base update function called each frame
	void Update() override;


private:
	// This function is for attempting to resolve the item at the top of
	// the AI player's priority queue of things to do. Returns success of
	// implementing the item (i.e. enough funds, chose a successful position, etc.)
	bool ResolveItem(EQueueObjectType qObject);
	void AssessWorkers();

	// As you cannot adjust values within a priority queue wthout removing, editing, then pushing back on again, this function
	// deals with that process for the top() element of the priority queue
	bool DecreaseTopItem();

	// Function to determine when to request additional units
	void AssessSituation();


	// CONSTANTS
	//---------------------------
	float UPDATE_TIME;
	float MIN_WAIT;
	float MAX_WAIT;
	int MIN_NEW_ITEMS;
	int MAX_NEW_ITEMS;

	int MIN_CASH_FOR_UPDATE;


	// CONTAINERS
	//---------------------------
	std::priority_queue<CBuildRequest*, std::deque<CBuildRequest*>, SSortByPriority> mpTaskQ;
	std::vector<CGameAgent*> mpSelectedAgents;
	std::vector<CGameAgent*>::iterator miterSelectedAgents;


	// MISC
	//---------------------------
	float mUpdateTime;			// Time between the AI doing tasks - can be determined by difficulty level perhaps?
	float mWaitTime;
};


#endif /* _AI_PLAYER_H_ */
