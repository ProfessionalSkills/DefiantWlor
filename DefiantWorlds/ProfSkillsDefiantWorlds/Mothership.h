//-----------------------------------------------------
// FILE: Mothership.h
//-----------------------------------------------------

#ifndef _MOTHERSHIP_H_
#define _MOTHERSHIP_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "SpaceUnit.h"


//-----------------------------------------------------
// MOTHERSHIP CLASS - CHILD OF SPACE UNIT
//-----------------------------------------------------
class CMothership : public CSpaceUnit
{
private:
	// DATA
	//---------------------------


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CMothership();
	virtual ~CMothership();


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

#endif /* _MOTHERSHIP_H_ */