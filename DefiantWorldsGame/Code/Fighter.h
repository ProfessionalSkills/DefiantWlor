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
	bool Attack(CGameAgent* target, float hitMod, float damageMod);
	//virtual void MoveTo(CTile* dest);
	bool Move();
	bool Update();

private:
	bool Destroy();
	IModel* CreateModel(DX::XMFLOAT3 pos) override;
};

#endif /* _FIGHTER_H_ */
