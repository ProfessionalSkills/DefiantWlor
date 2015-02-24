//-----------------------------------------------------
// FILE: Artillery.h
//-----------------------------------------------------

#ifndef _ARTILLERY_H_
#define _ARTILLERY_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "GroundUnit.h"


//-----------------------------------------------------
// ARTILLERY CLASS - CHILD OF GROUND UNIT
//-----------------------------------------------------
class CArtillery : public CGroundUnit
{
private:
	// DATA
	//---------------------------


public:
	static IMesh* mspMshArtillery;


	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CArtillery();
	virtual ~CArtillery();

	// ACCESSORS
	//---------------------------



	// MUTATORS
	//---------------------------



	// METHODS
	//---------------------------
	void UnloadIModel();

	// OVERRIDE METHODS
	//---------------------------
	bool Attack(CGameAgent* target, float hitMod, float damageMod);
	//virtual void MoveTo(CTile* dest);
	bool Move();

private:
	bool Destroy();
	IModel* CreateModel(DX::XMFLOAT3 pos) override;
};

#endif /* _ARTILLERY_H_ */
