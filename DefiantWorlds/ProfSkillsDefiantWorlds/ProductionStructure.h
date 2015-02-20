//-----------------------------------------------------
// FILE: ProductionStructure.h
//-----------------------------------------------------

#ifndef _PRODUCTION_STRUCTURE_H_
#define _PRODUCTION_STRUCTURE_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Structure.h"

#include "Artillery.h"
#include "Tank.h"
#include "Infantry.h"

#include "Worker.h"

#include "Bomber.h"
#include "Fighter.h"

#include "SpaceFighter.h"
#include "Mothership.h"
#include "Transport.h"


//-----------------------------------------------------
// CONSTANTS
//-----------------------------------------------------
const int MAX_QUEUE_SIZE = 5;


//-----------------------------------------------------
// PRODUCTION STRUCTURE CLASS - CHILD OF STRUCTURE
//-----------------------------------------------------
class CProductionStructure : public CStructure
{
protected:
	// DATA
	//---------------------------
	std::list<CGameAgent*> mRespectiveAgentsList;
	std::queue<CGameAgent*> mpProductionQueue;

	std::list<CGameAgent*>::iterator miterRespectiveAgents;


	// UNIT SPAWNING VARIABLES
	//---------------------------
	SPointData mGridSpawnLoc;			// The single grid square in which units spawn (relative to building centre)
	DX::XMFLOAT3 mWorldSpawnLoc;		// The calculated world position of the above grid square


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CProductionStructure();
	virtual ~CProductionStructure();


	// ACCESSORS
	//---------------------------
public:


	// MUTATORS
	//---------------------------



	// METHODS
	//---------------------------
	bool AddToQueue(int agentIndex);
	bool RemoveFromQueue();
	bool UpdateProduction();
	CGameAgent* CreateAgent();		// Called when agent at front of production queue is finished
	void UpdateKeyPresses();

	bool Update() override;
	void DisplayInfo(IFont* font) override;


	// OVERRIDE METHODS
	//---------------------------
	void CreateStructure(CGrid* pGrid) override;
	bool TestStructureArea(CGrid* pGrid, CTile* pTile) override;
	void Destroy() override;

	virtual void SetBuiltModel();
	virtual void UnloadIModel();
};

#endif /* _PRODUCTION_STRUCTURE_H_ */
