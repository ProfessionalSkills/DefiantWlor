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
	static IMesh* mspMshBomber;
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
	bool Attack(CGameAgent* target, float hitMod, float damageMod);
	void Spawn(CGrid* pGrid, SPointData pCentre);
	//virtual void MoveTo(CTile* dest);
	bool Move();

private:
	bool Destroy();
};

#endif /* _BOMBER_H_ */
