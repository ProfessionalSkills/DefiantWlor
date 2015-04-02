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
	static IMesh* mspMshSheild;

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
	void UnloadIModel();

	// OVERRIDE METHODS
	//---------------------------
	void Spawn(CGrid* pGrid, SPointData pCentre);
	bool Attack(CGameObject* target, float hitMod, float damageMod);
	void HitFlash();
	void UnloadFlash();
	float GetUnitSpacing();
	void MoveY(float yChange);
	bool StoreUnits(CGameAgent* unit);

private:
	bool Destroy();
};

#endif /* _SPACE_FIGHTER_H_ */