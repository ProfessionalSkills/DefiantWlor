//-----------------------------------------------------
// FILE: SpaceFighter.h
//-----------------------------------------------------

#ifndef _SPACE_FIGHTER_H_
#define _SPACE_FIGHTER_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "SpaceUnit.h"


//-----------------------------------------------------
// SPACE FIGHTER CLASS - CHILD OF SPACE UNIT
//-----------------------------------------------------
class CSpaceFighter : public CSpaceUnit
{
private:
	// DATA
	//---------------------------


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CSpaceFighter();
	virtual ~CSpaceFighter();


	// ACCESSORS
	//---------------------------



	// MUTATORS
	//---------------------------



	// METHODS
	//---------------------------


	// OVERRIDE METHODS
	//---------------------------
	bool Attack(CGameAgent* target, float hitMod, float damageMod) override;
	virtual bool Attack(CGameAgent* target);
	//virtual void MoveTo(CTile* dest);
	virtual bool Move();

private:
	virtual bool Destroy();
};

#endif /* _SPACE_FIGHTER_H_ */