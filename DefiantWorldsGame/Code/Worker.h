//-----------------------------------------------------
// FILE: Worker.h
//-----------------------------------------------------

#ifndef _WORKER_H_
#define _WORKER_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "GroundUnit.h"
#include "Minerals.h"


//-----------------------------------------------------
// WORKER CLASS - CHILD OF GROUND UNIT
//-----------------------------------------------------
class CWorker : public CGroundUnit
{
private:
	// DATA
	//---------------------------
	CMinerals* mpActiveMineral = nullptr;			// Stores a pointer to the mineral which is being harvested by this worker unit


public:
	static IMesh* mspMshWorker;
	static IMesh* mspMshWorkerBullet;

	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CWorker();
	virtual ~CWorker();


	// ACCESSORS
	//---------------------------
	inline CMinerals* GetMineral()
	{
		return mpActiveMineral;
	}


	// MUTATORS
	//---------------------------
	inline void SetMineral(CMinerals* pMineral)
	{
		mpActiveMineral = pMineral;
	}


	// METHODS
	//---------------------------
	//bool RepairBuilding(CStructure* structure);
	bool RepairUnit(CGroundUnit* unit);
	void UnloadIModel();
	void LoadIModel();

	// OVERRIDE METHODS
	//---------------------------
	virtual bool Attack(CGameObject* target, float hitMod, float damageMod);

private:
	virtual bool Destroy();
	IModel* CreateModel(DX::XMFLOAT3 pos) override;
};

#endif /* _WORKER_H_ */
