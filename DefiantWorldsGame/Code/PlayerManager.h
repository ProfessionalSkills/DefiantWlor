//-----------------------------------------------------
// FILE: PlayerManager.h
//-----------------------------------------------------

#ifndef _PLAYER_MANAGER_H_
#define _PLAYER_MANAGER_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Player.h"


//-----------------------------------------------------
// PLAYER MANAGER CLASS
//-----------------------------------------------------
class CPlayerManager
{
public:
	// CONSTRUCTOR & DESTRUCTOR
	//---------------------------
	CPlayerManager();
	~CPlayerManager();


	// METHODS
	//---------------------------
	void CreatePlayers(EFactions humanFaction, int numAI);
	void RemovePlayers();
	void UpdatePlayers();


	// ACCESSORS
	//---------------------------
	inline CRTSPlayer* GetHumanPlayer()
	{
		return mpHuman;
	}

	inline CRTSPlayer* GetAIPlayer(int index = 1)
	{
		return mpAI[index];
	}


	// MUTATORS
	//---------------------------


private:
	// PLAYERS
	//---------------------------
	CRTSPlayer* mpHuman;
	CRTSPlayer* mpAI[MAX_AI_NUM];

	int mNumAI;
};


#endif /* _PLAYER_MANAGER_H_ */
