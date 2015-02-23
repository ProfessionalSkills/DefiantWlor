//-----------------------------------------------------
// FILE: GameState.h
//-----------------------------------------------------

#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Common.h"

//-----------------------------------------------------
// INTERFACE CLASS FOR GAME STATES
//-----------------------------------------------------
class CGameState
{
protected:

public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CGameState();
	virtual ~CGameState();


	// VIRTUAL METHODS
	//---------------------------
	virtual void StateSetup() = 0;
	virtual void StateUpdate() = 0;
	virtual void StateCleanup() = 0;
	virtual void StateSave() = 0;
	virtual void StateLoad() = 0;
};


#endif /* _GAME_STATE_H_ */
