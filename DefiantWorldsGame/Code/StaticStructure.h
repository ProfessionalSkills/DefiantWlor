//-----------------------------------------------------
// FILE: StaticStructure.h
//-----------------------------------------------------

#ifndef _STATIC_STRUCTURE_H_
#define _STATIC_STRUCTURE_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Structure.h"


//-----------------------------------------------------
// STATIC STRUCTURE CLASS - CHILD OF STRUCTURE
//-----------------------------------------------------
class CStaticStructure : public CStructure
{
private:
	// DATA
	//---------------------------


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CStaticStructure();
	virtual ~CStaticStructure();


	// ACCESSORS
	//---------------------------
	int GetQueueSize()
	{
		return 0;
	}


	// MUTATORS
	//---------------------------



	// METHODS
	//---------------------------


	// OVERRIDE METHODS
	//---------------------------
	// Adds a unit to the queue based on the index which has been passed in
	virtual EErrorTypes AddToQueue(size_t agentIndex, CRTSPlayer* pPlayer);
	// Removes a unit in the queue at the given index
	virtual void RemoveFromQueue(size_t agentIndex, CRTSPlayer* pPlayer)
	{

	}

	// Returns a pointer to the queue of units
	virtual std::deque<CGameAgent*>* GetQueue()
	{
		return nullptr;
	}

	virtual void SetBuiltModel()
	{

	}

	// Method called each frome - boolean returns is false when the object has been destroyed
	virtual bool Update(CRTSPlayer* pPlayer);
	virtual void DisplayInfo(IFont* font);
	virtual void UnloadIModel();
	virtual void LoadIModel();
};

#endif /* _STATIC_STRUCTURE_H_ */
