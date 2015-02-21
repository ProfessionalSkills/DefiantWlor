//-----------------------------------------------------
// FILE: Artillery.h
//-----------------------------------------------------

#ifndef _ARTILLERY_H_
#define _ARTILLERY_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "GroundUnit.h"


//-----------------------------------------------------
// ARTILLERY CLASS - CHILD OF GROUND UNIT
//-----------------------------------------------------
class CArtillery : public CGroundUnit
{
private:
	// DATA
	//---------------------------


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CArtillery();
	virtual ~CArtillery();


	// ACCESSORS
	//---------------------------



	// MUTATORS
	//---------------------------



	// METHODS
	//---------------------------


	// OVERRIDE METHODS
	//---------------------------
	bool Attack(CGameAgent* target, float hitMod, float damageMod);
	void Spawn();
	//virtual void MoveTo(CTile* dest);
	bool Move();

private:
	bool Destroy();
};

#endif /* _ARTILLERY_H_ */
