//-----------------------------------------------------
// FILE: ProductionStructure.h
//-----------------------------------------------------

#ifndef _PRODUCTION_STRUCTURE_H_
#define _PRODUCTION_STRUCTURE_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Structure.h"



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
	std::deque<CGameAgent*> mpProductionQueue;
	std::deque<CGameAgent*>::iterator miterProdQ;

	std::list<CGameAgent*>::iterator miterRespectiveAgents;


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CProductionStructure();
	virtual ~CProductionStructure();


	// ACCESSORS
	//---------------------------
	int GetQueueSize()
	{
		return mpProductionQueue.size();
	}


	// MUTATORS
	//---------------------------



	// METHODS
	//---------------------------
	EErrorTypes AddToQueue(size_t agentIndex, CRTSPlayer* pPlayer);
	void RemoveFromQueue(size_t agentIndex, CRTSPlayer* pPlayer);
	std::deque<CGameAgent*>* GetQueue();
	bool SpawnFront(CRTSPlayer* pPlayer);
	bool UpdateProduction();
	CGameAgent* CreateAgent();		// Called when agent at front of production queue is finished

	bool Update(CRTSPlayer* pPlayer) override;
	void DisplayInfo(IFont* font) override;

	// Save the structure's details
	virtual void SaveStructure(std::ofstream& outFile) override;
	virtual void LoadStructure(std::ifstream& inFile, CGrid* pGrid, CRTSPlayer* pPlayer) override;


	// OVERRIDE METHODS
	//---------------------------
	virtual void SetBuiltModel();
	virtual void UnloadIModel();
	virtual void LoadIModel()=0;
};

#endif /* _PRODUCTION_STRUCTURE_H_ */
