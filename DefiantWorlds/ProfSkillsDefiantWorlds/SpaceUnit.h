//-----------------------------------------------------
// FILE: SpaceUnit.h
//-----------------------------------------------------

#ifndef _SPACE_UNIT_H_
#define _SPACE_UNIT_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "GameAgent.h"


//-----------------------------------------------------
// SPACE UNIT CLASS - CHILD OF GAME AGENT
//-----------------------------------------------------
class CSpaceUnit : public CGameAgent
{
private:
	// DATA
	//---------------------------
	

public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CSpaceUnit();
	virtual ~CSpaceUnit();


	// ACCESSORS
	//---------------------------
	


	// MUTATORS
	//---------------------------
	


	// METHODS
	//---------------------------
	

	// OVERRIDE METHODS
	//---------------------------
	virtual bool Attack(CGameAgent* target);
	//virtual void MoveTo(CTile* dest);
	virtual bool Move(float delta);

private:
	virtual bool Destroy();
};

#endif /* _SPACE_UNIT_H_ */