//-----------------------------------------------------
// FILE: AirUnit.h
//-----------------------------------------------------

#ifndef _AIR_UNIT_H_
#define _AIR_UNIT_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "GameAgent.h"


//-----------------------------------------------------
// AIR UNIT CLASS - CHILD OF GAME AGENT
//-----------------------------------------------------
class CAirUnit : public CGameAgent
{
private:
	// DATA
	//---------------------------


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CAirUnit();
	virtual ~CAirUnit();


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

#endif /* _AIR_UNIT_H_ */
