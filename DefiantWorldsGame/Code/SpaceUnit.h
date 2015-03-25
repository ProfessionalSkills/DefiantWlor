//-----------------------------------------------------
// FILE: SpaceUnit.h
//-----------------------------------------------------

#ifndef _SPACE_UNIT_H_
#define _SPACE_UNIT_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "GameAgent.h"
#include "Randomiser.h"


//-----------------------------------------------------
// SPACE UNIT CLASS - CHILD OF GAME AGENT
//-----------------------------------------------------


class CSpaceUnit : public CGameAgent
{
protected:
	// DATA
	//---------------------------
	float mHitChance;
	float mUnitSpacing;
	IModel* mpTempShield;
	IModel* mpTempLazer;
	const float mChargeTimeMax;//max time a laser should charge for
	const float mChargeTimeMin;//minnimum time a lazer should charge for
	float mChargeTime;//time laser is currently charging for
	CRandomiser* mpToHitRoll;

public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CSpaceUnit();
	virtual ~CSpaceUnit();
	static IMesh* mspMshLazer;


	// ACCESSORS
	//---------------------------


	// MUTATORS
	//---------------------------

	// METHODS
	//---------------------------

	// OVERRIDE METHODS
	//---------------------------
	virtual bool Attack(CGameObject* target,float hitMod, float damageMod);
	virtual void Spawn(CGrid* pGrid, SPointData pCentre);
	virtual bool Update();
	//virtual void MoveTo(CTile* dest);
	virtual void Move();
	virtual void UnloadIModel();
	virtual void LoadIModel();
	virtual void LoadModel(float x, float y, float z);
	virtual bool Destroy();
	virtual void HitFlash()=0;
	virtual void UnloadFlash() = 0;
	virtual void MoveY(float yChange)=0;
	void FireLazer(CGameObject* target);
	void UnloadLazer();
};

#endif /* _SPACE_UNIT_H_ */
