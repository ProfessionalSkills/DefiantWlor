//-----------------------------------------------------
// FILE: BasePlayer.h
//-----------------------------------------------------

#ifndef _BASE_PLAYER_H_
#define _BASE_PLAYER_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "SpaceFighter.h"		// Space unit
#include "Mothership.h"			// Space unit
#include "Transport.h"			// Space unit
#include "Worker.h"				// Ground unit
#include "Infantry.h"			// Ground unit
#include "Tank.h"				// Ground unit	
#include "Artillery.h"			// Ground unit
#include "Bomber.h"				// Air unit
#include "Fighter.h"			// Air unit

#include "Minerals.h"			// Resource

#include "House.h"				// Static structure
#include "Barracks.h"			// Production structure
#include "ComCentre.h"			// Production structure
#include "Hellipad.h"			// Production structure
#include "SpaceCentre.h"		// Production structure


//-----------------------------------------------------
// BASE CLASS FOR PLAYERS
//-----------------------------------------------------
class CPlayer
{
protected:
	// DATA
	//---------------------------
	std::string mName;
	int         mNumMinerals;
	int         mCurPopLimit;
	int         mCurPop;
	bool        mIsCommsDestroyed;
	

	// LISTS
	//---------------------------
	std::vector<CStructure*> mpStructureList;
	std::vector<CGameAgent*> mpWorldUnitsList;
	std::vector<CSpaceUnit*> mpSpaceUnitsList;

	std::vector<CStructure*>::iterator mpiterStructures;
	std::vector<CGameAgent*>::iterator mpiterGameAgents;
	std::vector<CSpaceUnit*>::iterator mpiterSpaceUnits;


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CPlayer();
	virtual ~CPlayer();


	// ACCESSORS
	//---------------------------
	inline std::string GetPlayerName()
	{
		return mName;
	}

	inline int GetMineralAmount()
	{
		return mNumMinerals;
	}

	inline int GetCurrentPop()
	{
		return mCurPop;
	}

	inline int GetCurrentPopLimit()
	{
		return mCurPopLimit;
	}

	inline bool GetCommsState()
	{
		return mIsCommsDestroyed;
	}


	// MUTATORS
	//---------------------------
	inline void SetPlayerName(std::string name)
	{
		mName = name;
	}

	bool MineralTransaction(int amount);


	// METHODS
	//---------------------------
	bool PurchaseStructure(CStructure* pStructure, CGrid* pGrid, CTile* pTile);
	bool QueueUnit(CStructure* structure, CGameAgent* unit);


	// VIRTUAL METHODS
	//---------------------------
	virtual void Update() = 0;
};


#endif /* _BASE_PLAYER_H_ */
