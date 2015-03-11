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
	static IMesh* mspMshTank;


	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CTank();
	virtual ~CTank();


	// STATIC MESH
	//---------------------------

	// ACCESSORS
	//---------------------------



	// MUTATORS
	//---------------------------



	// METHODS
	//---------------------------
	void UnloadIModel();
	void LoadIModel();

	// OVERRIDE METHODS
	//---------------------------
	bool Attack(CGameAgent* target, float hitMod, float damageMod);
	//void Spawn(CGrid* pGrid, SPointData pCentre);
	//virtual void MoveTo(CTile* dest);
	bool Move();

private:
	virtual bool Destroy();
	IModel* CreateModel(DX::XMFLOAT3 pos) override;
};

#endif /* _TANK_H_ */
