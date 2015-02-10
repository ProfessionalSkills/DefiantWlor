//-----------------------------------------------------
// FILE: SpaceState.h
//-----------------------------------------------------

#ifndef _SPACE_STATE_H_
#define _SPACE_STATE_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "PlayState.h"


//-----------------------------------------------------
// SPACE STATE CLASS
//-----------------------------------------------------
class CSpaceState : public CPlayState
{
private:

public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CSpaceState();
	virtual ~CSpaceState();


	// OVERRIDE METHODS
	//---------------------------
	void StateSetup() override;
	void StateUpdate() override;
	void StateCleanup() override;
	void StateSave() override;
	void StateLoad() override;
};


#endif /* _SPACE_STATE_H_ */
