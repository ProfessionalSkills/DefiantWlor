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

	inline CRTSPlayer* GetAIPlayer(int index = 0)
	{
		return mpAI[index];
	}

	inline bool ArePlayersInitialised()
	{
		return mPlayerDataInitialised;
	}


	// MUTATORS
	//---------------------------
	inline void PlayersInitialised()
	{
		mPlayerDataInitialised = true;
	}

	inline void PlayersUnloaded()
	{
		mPlayerDataInitialised = false;
	}


private:
	// PLAYERS
	//---------------------------
	CRTSPlayer* mpHuman;
	CRTSPlayer* mpAI[MAX_AI_NUM];

	int mNumAI;

	bool mPlayerDataInitialised;
};


#endif /* _PLAYER_MANAGER_H_ */
