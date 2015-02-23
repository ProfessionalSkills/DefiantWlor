//-----------------------------------------------------
// FILE: SpaceState.h
//-----------------------------------------------------

#ifndef _PLAY_STATE_H_
#define _PLAY_STATE_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "BaseGameState.h"
#include "AIPlayer.h"
#include "HumanPlayer.h"


//-----------------------------------------------------
// PLAY STATE CLASS
//-----------------------------------------------------
class CPlayState : public CGameState
{
protected:
	CHumanPlayer* mpHumanPlayer;
	CAIPlayer*    mpAIPlayer;
	bool mPlayersCreated;


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CPlayState();
	virtual ~CPlayState();


	// ACCESSORS
	//---------------------------
	inline CHumanPlayer* GetHumanPlayer()
	{
		return mpHumanPlayer;
	}

	inline CAIPlayer* GetAIPlayer()
	{
		return mpAIPlayer;
	}

	inline bool ArePlayersCreated()
	{
		return mPlayersCreated;
	}


	// MUTATORS
	//---------------------------
	inline void SetHumanPlayer(CHumanPlayer* player)
	{
		mpHumanPlayer = player;
	}

	inline void SetAIPlayer(CAIPlayer* player)
	{
		mpAIPlayer = player;
	}


	// VIRTUAL METHODS
	//---------------------------
	virtual void StateSetup();
	virtual void StateUpdate();
	virtual void StateCleanup();
	virtual void StateSave();
	virtual void StateLoad();


	// METHODS
	//---------------------------
	void CreatePlayers();
	void RemovePlayers();
};


#endif /* _PLAY_STATE_H_ */
