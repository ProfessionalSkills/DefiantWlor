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


	// OVERRIDE METHODS
	//---------------------------
	virtual bool Attack(CGameAgent* target);
	//virtual void MoveTo(CTile* dest);
	virtual bool Move(float delta);

private:
	virtual bool Destroy();
};

#endif /* _WORKER_H_ */
