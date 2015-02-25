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
	static IMesh* mspMshInfantry;


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
	void UnloadIModel();
	void LoadIModel();

	// OVERRIDE METHODS
	//---------------------------
	bool Attack(CGameAgent* target, float hitMod, float damageMod);
	//virtual void MoveTo(CTile* dest);
	bool Move();

private:
	bool Destroy();
	IModel* CreateModel(DX::XMFLOAT3 pos) override;
};

#endif /* _INFANTRY_H_ */
