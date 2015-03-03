//-----------------------------------------------------
// FILE: SpaceUnit.h
//-----------------------------------------------------

#ifndef _GROUND_UNIT_H_
#define _GROUND_UNIT_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "GameAgent.h"


//-----------------------------------------------------
// GROUND UNIT CLASS - CHILD OF GAME AGENT
//-----------------------------------------------------
class CGroundUnit : public CGameAgent
{
private:
	// DATA
	//---------------------------


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CGroundUnit();
	virtual ~CGroundUnit();


	// ACCESSORS
	//---------------------------



	// MUTATORS
	//---------------------------



	// METHODS
	//---------------------------


	// OVERRIDE METHODS
	//---------------------------
	virtual bool Attack(CGameAgent* target, float hitMod, float damageMod);
	//virtual void MoveTo(CTile* dest);
	void Update();
	virtual void UnloadIModel()=0;
	virtual void LoadIModel() = 0;
	void LoadModel(float x, float y, float z)
	{

	}
	void Spawn(CGrid* pGrid, SPointData pCentre);
	bool LookingAt();

private:
	virtual bool Destroy();
	virtual IModel* CreateModel(DX::XMFLOAT3 pos) = 0;
};

#endif /* _GROUND_UNIT_H_ */
