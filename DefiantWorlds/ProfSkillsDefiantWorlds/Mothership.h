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
	bool Attack(CGameAgent* target, float hitMod, float damageMod);
	//virtual void MoveTo(CTile* dest);
	bool Move();

private:
	bool Destroy();
};

#endif /* _MOTHERSHIP_H_ */