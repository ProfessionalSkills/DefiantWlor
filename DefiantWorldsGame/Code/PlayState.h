//-----------------------------------------------------
// FILE: SpaceState.h
//-----------------------------------------------------

#ifndef _PLAY_STATE_H_
#define _PLAY_STATE_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "BaseGameState.h"


//-----------------------------------------------------
// PLAY STATE CLASS
//-----------------------------------------------------
class CPlayState : public CGameState
{
protected:


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CPlayState();
	virtual ~CPlayState();


	// ACCESSORS
	//---------------------------


	// MUTATORS
	//---------------------------



	// VIRTUAL METHODS
	//---------------------------
	virtual void StateSetup();
	virtual void StateUpdate();
	virtual void StateCleanup();
	virtual void StateSave();
	virtual void StateLoad();


	// METHODS
	//---------------------------
};


#endif /* _PLAY_STATE_H_ */
