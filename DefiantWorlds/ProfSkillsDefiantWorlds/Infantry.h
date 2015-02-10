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
	virtual bool Attack(CGameAgent* target);
	//virtual void MoveTo(CTile* dest);
	virtual bool Move();

private:
	virtual bool Destroy();
};

#endif /* _INFANTRY_H_ */
