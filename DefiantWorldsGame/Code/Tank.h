//-----------------------------------------------------
// FILE: Tank.h
//-----------------------------------------------------

#ifndef _TANK_H_
#define _TANK_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "GroundUnit.h"


//-----------------------------------------------------
// TANK CLASS - CHILD OF GROUND UNIT
//-----------------------------------------------------
class CTank : public CGroundUnit
{
private:
	// DATA
	//---------------------------


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CTank();
	virtual ~CTank();


	// STATIC MESH
	//---------------------------
	static IMesh* mpObjMesh;

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
	virtual bool Destroy();
};

#endif /* _TANK_H_ */
