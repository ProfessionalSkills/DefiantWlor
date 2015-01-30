//-----------------------------------------------------
// FILE: WorldState.h
//-----------------------------------------------------

#ifndef _WORLD_STATE_H_
#define _WORLD_STATE_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "BaseGameState.h"


//-----------------------------------------------------
// WORLD STATE CLASS
//-----------------------------------------------------
class CWorldState : public IGameState
{
private:


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CWorldState();
	virtual ~CWorldState();


	// OVERRIDE METHODS
	//---------------------------
	void StateSetup() override;
	void StateUpdate(const float inDelta) override;
	void StateCleanup() override;
	void StateSave() override;
	void StateLoad() override;
};


#endif /* _WORLD_STATE_H_ */
