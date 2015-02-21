//-----------------------------------------------------
// FILE: Infantry.h
//-----------------------------------------------------

#ifndef _INFANTRY_H_
#define _INFANTRY_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "GroundUnit.h"


//-----------------------------------------------------
// INFANTRY CLASS - CHILD OF GROUND UNIT
//-----------------------------------------------------
class CInfantry : public CGroundUnit
{
private:
	// DATA
	//---------------------------


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CInfantry();
	virtual ~CInfantry();


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

#endif /* _INFANTRY_H_ */
