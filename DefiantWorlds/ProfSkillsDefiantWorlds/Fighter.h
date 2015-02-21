//-----------------------------------------------------
// FILE: Fighter.h
//-----------------------------------------------------

#ifndef _FIGHTER_H_
#define _FIGHTER_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "AirUnit.h"


//-----------------------------------------------------
// FIGHTER CLASS - CHILD OF AIR UNIT
//-----------------------------------------------------
class CFighter : public CAirUnit
{
private:
	// DATA
	//---------------------------


public:
	static IMesh* mspMshFighter;
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CFighter();
	virtual ~CFighter();


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

#endif /* _FIGHTER_H_ */
