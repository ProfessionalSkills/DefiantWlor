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
	virtual bool Attack(CGameAgent* target);
	//virtual void MoveTo(CTile* dest);
	virtual bool Move();

private:
	virtual bool Destroy();
};

#endif /* _FIGHTER_H_ */
