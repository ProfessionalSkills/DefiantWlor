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
protected:
	int mTurretNode;
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
	virtual bool Attack(CGameObject* target, float hitMod, float damageMod) = 0;
	//virtual void MoveTo(CTile* dest);
	virtual bool Update();
	virtual void UnloadIModel()=0;
	virtual void LoadIModel() = 0;
	void LoadModel(float x, float y, float z)
	{

	}

	virtual void SetTargetTexture()
	{

	}

	virtual void SetNormalTexture()
	{

	}

	void Spawn(CGrid* pGrid, SPointData pCentre);
	bool LookingAt(DX::XMFLOAT3 targetPosition);
	virtual void Move();
	void DetermineAirspace();

private:
	virtual IModel* CreateModel(DX::XMFLOAT3 pos) = 0;
};

#endif /* _GROUND_UNIT_H_ */
