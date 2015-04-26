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
	bool mRaised = false;				// Stores whether the wall is raised to allow units to pass
	float mRaisedAmount = 0.0f;			// Stores how high off the ground this wall is
	float mRaisedTimer = 0.0f;


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

	inline bool IsRaised()
	{
		return mRaised;
	}


	// MUTATORS
	//---------------------------
	inline void SetRaised(bool raised)
	{
		mRaised = raised;
		mRaisedTimer = 0.0f;
	}


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

	// Save the structure's details
	virtual void SaveStructure(std::ofstream& outFile) override;
	virtual void LoadStructure(std::ifstream& inFile, CGrid* pGrid, CRTSPlayer* pPlayer) override;
};

#endif /* _STATIC_STRUCTURE_H_ */
