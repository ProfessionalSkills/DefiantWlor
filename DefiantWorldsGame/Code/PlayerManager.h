//-----------------------------------------------------
// FILE: PlayerManager.h
//-----------------------------------------------------

#ifndef _PLAYER_MANAGER_H_
#define _PLAYER_MANAGER_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "AIPlayer.h"


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

	inline CRTSAIPlayer* GetAIPlayer(int index = 0)
	{
		return mpAI[index];
	}

	inline bool ArePlayersInitialised()
	{
		return mPlayerDataInitialised;
	}


private:
	// PLAYERS
	//---------------------------
	CRTSPlayer* mpHuman;
	CRTSAIPlayer* mpAI[MAX_AI_NUM];

	int mNumAI;

	bool mPlayerDataInitialised;
};


#endif /* _PLAYER_MANAGER_H_ */
