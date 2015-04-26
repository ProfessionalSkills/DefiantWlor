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
	static IMesh* mspMshInfantryBullet;

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
	bool Attack(CGameObject* pTarget, float hitMod, float damageMod);
	void SetTargetTexture();
	void SetNormalTexture();
	bool Update();


private:
	bool Destroy();
	IModel* CreateModel(DX::XMFLOAT3 pos) override;
};

#endif /* _INFANTRY_H_ */
