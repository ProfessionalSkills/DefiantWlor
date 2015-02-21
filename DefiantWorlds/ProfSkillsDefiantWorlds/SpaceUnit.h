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

public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CSpaceUnit();
	virtual ~CSpaceUnit();


	// ACCESSORS
	//---------------------------
	


	// MUTATORS
	//---------------------------
	


	// METHODS
	//---------------------------
	

	// OVERRIDE METHODS
	//---------------------------
	virtual bool Attack(CGameAgent* target,float hitMod, float damageMod);
	virtual void Spawn();
	//virtual void MoveTo(CTile* dest);
	virtual bool Move();
	virtual void UnloadIModel();
	//virtual void LoadModel(float x, float y, float z);


private:
	virtual bool Destroy();
};

#endif /* _SPACE_UNIT_H_ */
