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
	static IMesh* mspMshSpaceFighter;
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
	void LoadModel(float x, float y, float z);
	void RemoveModel();
	// OVERRIDE METHODS
	//---------------------------
	void Spawn(CGrid* pGrid, SPointData pCentre);
	bool Attack(CGameAgent* target, float hitMod, float damageMod);
	//virtual void MoveTo(CTile* dest);
	bool Move();

private:
	bool Destroy();
};

#endif /* _SPACE_FIGHTER_H_ */