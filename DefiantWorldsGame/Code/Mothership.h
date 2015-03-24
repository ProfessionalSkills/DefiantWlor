//-----------------------------------------------------
// FILE: Mothership.h
//-----------------------------------------------------

#ifndef _MOTHERSHIP_H_
#define _MOTHERSHIP_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "SpaceUnit.h"


//-----------------------------------------------------
// MOTHERSHIP CLASS - CHILD OF SPACE UNIT
//-----------------------------------------------------
class CMothership : public CSpaceUnit
{
private:
	// DATA
	//---------------------------


public:
	static IMesh* mspMshMothership;
	static IMesh* mspMshSheild;
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CMothership();
	virtual ~CMothership();


	// ACCESSORS
	//---------------------------

	// MUTATORS
	//---------------------------



	// METHODS
	//---------------------------


	// OVERRIDE METHODS
	//---------------------------
	bool Attack(CGameObject* target, float hitMod, float damageMod);
	void Spawn(CGrid* pGrid, SPointData pCentre);
	//virtual void MoveTo(CTile* dest);
	void LoadModel(float x, float y, float z);
	void UnloadIModel();
	void HitFlash();
	void UnloadFlash();
	float GetUnitSpacing();
	void MoveY(float yChange);

private:
	bool Destroy();
};

#endif /* _MOTHERSHIP_H_ */