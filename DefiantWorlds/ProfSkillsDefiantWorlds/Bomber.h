//-----------------------------------------------------
// FILE: Bomber.h
//-----------------------------------------------------

#ifndef _BOMBER_H_
#define _BOMBER_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "AirUnit.h"


//-----------------------------------------------------
// BOMBER CLASS - CHILD OF AIR UNIT
//-----------------------------------------------------
class CBomber : public CAirUnit
{
private:
	// DATA
	//---------------------------


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CBomber();
	virtual ~CBomber();


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
	virtual bool Move();

private:
	virtual bool Destroy();
};

#endif /* _BOMBER_H_ */
