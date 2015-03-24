//-----------------------------------------------------
// FILE: AirUnit.h
//-----------------------------------------------------

#ifndef _AIR_UNIT_H_
#define _AIR_UNIT_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "GameAgent.h"


//-----------------------------------------------------
// AIR UNIT CLASS - CHILD OF GAME AGENT
//-----------------------------------------------------
class CAirUnit : public CGameAgent
{
private:
	// DATA
	//---------------------------
	float mYaw;

public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CAirUnit();
	virtual ~CAirUnit();


	// OVERRIDE METHODS
	//---------------------------
	virtual bool Attack(CGameObject* target, float hitMod, float damageMod);
	//virtual void MoveTo(CTile* dest);
	virtual void Move();
	virtual bool Update();
	bool LookingAt();
	virtual void UnloadIModel() = 0;
	virtual void LoadIModel() = 0;

	void LoadModel(float x, float y, float z)
	{

	}

	void Spawn(CGrid* pGrid, SPointData pCentre);

private:
	virtual bool Destroy();
	virtual IModel* CreateModel(DX::XMFLOAT3 pos) = 0;
};

#endif /* _AIR_UNIT_H_ */
