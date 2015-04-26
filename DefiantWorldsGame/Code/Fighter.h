//-----------------------------------------------------
// FILE: Fighter.h
//-----------------------------------------------------

#ifndef _FIGHTER_H_
#define _FIGHTER_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "AirUnit.h"


//-----------------------------------------------------
// FIGHTER CLASS - CHILD OF AIR UNIT
//-----------------------------------------------------
class CFighter : public CAirUnit
{
private:
	// DATA
	//---------------------------

	float mRotarSpeed;
public:
	static IMesh* mspMshFighter;
	static IMesh* mspMshFighterBullet;

	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CFighter();
	virtual ~CFighter();


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
	bool Update();

	void SetTargetTexture();
	void SetNormalTexture();


private:
	bool Destroy();
	IModel* CreateModel(DX::XMFLOAT3 pos) override;
};

#endif /* _FIGHTER_H_ */
