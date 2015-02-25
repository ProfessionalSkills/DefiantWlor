//-----------------------------------------------------
// FILE: Worker.h
//-----------------------------------------------------

#ifndef _WORKER_H_
#define _WORKER_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "GroundUnit.h"


//-----------------------------------------------------
// WORKER CLASS - CHILD OF GROUND UNIT
//-----------------------------------------------------
class CWorker : public CGroundUnit
{
private:
	// DATA
	//---------------------------


public:
	static IMesh* mspMshWorker;


	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CWorker();
	virtual ~CWorker();


	// ACCESSORS
	//---------------------------



	// MUTATORS
	//---------------------------



	// METHODS
	//---------------------------
	//bool RepairBuilding(CStructure* structure);
	//bool CollectResources(CResources* resource);
	bool RepairUnit(CGroundUnit* unit);
	void UnloadIModel();
	void LoadIModel();

	// OVERRIDE METHODS
	//---------------------------
	virtual bool Attack(CGameAgent* target, float hitMod, float damageMod);
	//virtual void MoveTo(CTile* dest);
	virtual bool Move();

private:
	virtual bool Destroy();
	IModel* CreateModel(DX::XMFLOAT3 pos) override;
};

#endif /* _WORKER_H_ */
