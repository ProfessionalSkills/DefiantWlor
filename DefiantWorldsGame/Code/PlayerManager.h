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
	// CONSTANTS
	//---------------------------
	static const int MAX_AI_NUM = 1;


	// CONSTRUCTOR & DESTRUCTOR
	//---------------------------
	CPlayerManager();
	~CPlayerManager();


	// METHODS
	//---------------------------
	void CreatePlayers(EFactions humanFaction, int numAI);
	void RemovePlayers();


	// ACCESSORS
	//---------------------------
	inline CPlayer* GetHumanPlayer()
	{
		return mpHuman;
	}

	inline CPlayer* GetAIPlayer(int index)
	{
		return mpAI[index];
	}


	// MUTATORS
	//---------------------------


private:
	// PLAYERS
	//---------------------------
	CPlayer* mpHuman;
	CPlayer* mpAI[MAX_AI_NUM];
};


#endif /* _PLAYER_MANAGER_H_ */
