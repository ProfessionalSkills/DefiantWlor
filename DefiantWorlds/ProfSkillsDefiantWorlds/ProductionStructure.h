//-----------------------------------------------------
// FILE: ProductionStructure.h
//-----------------------------------------------------

#ifndef _PRODUCTION_STRUCTURE_H_
#define _PRODUCTION_STRUCTURE_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Structure.h"
#include "GameAgent.h"


//-----------------------------------------------------
// CONSTANTS
//-----------------------------------------------------
const int MAX_QUEUE_SIZE = 5;


//-----------------------------------------------------
// STRUCTURE
//-----------------------------------------------------
struct SAgentData
{
	EGameAgentsTypes mAgentType;
	std::string	     mAgentName;

	SAgentData(EGameAgentsTypes agentType, std::string agentName)
	{
		mAgentType = agentType;
		mAgentName = agentName;
	}
};

//-----------------------------------------------------
// PRODUCTION STRUCTURE CLASS - CHILD OF STRUCTURE
//-----------------------------------------------------
class CProductionStructure : public CStructure
{
protected:
	// DATA
	//---------------------------
	std::list<SAgentData> mRespectiveAgentsList;
	std::queue<CGameAgent*> mpProductionQueue;

	std::list<SAgentData>::iterator miterRespectiveAgents;


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

	void Update() override;
	void DisplayInfo(IFont* font) override;


	// OVERRIDE METHODS
	//---------------------------
	virtual void SetBuiltModel();
	virtual void UnloadIModel();
	virtual bool Destroy();
};

#endif /* _PRODUCTION_STRUCTURE_H_ */
