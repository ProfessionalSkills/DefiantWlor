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
	static IMesh* mspMshArtilleryShell;

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
	void LoadIModel();


	// OVERRIDE METHODS
	//---------------------------
	bool Attack(CGameObject* target, float hitMod, float damageMod);
	void SetTargetTexture();
	void SetNormalTexture();


private:
	bool Destroy();
	IModel* CreateModel(DX::XMFLOAT3 pos) override;
};

#endif /* _ARTILLERY_H_ */
