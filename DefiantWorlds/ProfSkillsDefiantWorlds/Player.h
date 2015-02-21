//-----------------------------------------------------
// FILE: Player.h
//-----------------------------------------------------

#ifndef _PLAYER_H_
#define _PLAYER_H_


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

#include "Fleet.h"				// Space Ship container

#include "Minerals.h"			// Resource

#include "House.h"				// Static structure
#include "Barracks.h"			// Production structure
#include "ComCentre.h"			// Production structure
#include "Hellipad.h"			// Production structure
#include "SpaceCentre.h"		// Production structure


//-----------------------------------------------------
// PLAYER CLASS
//-----------------------------------------------------
class CPlayer
{
public:
	// PUBLIC DESTRUCTOR
	//---------------------------
	~CPlayer();


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

	inline CGrid* GetFactionGrid()
	{
		return mpFactionGrid;
	}

	inline CFleet* GetFleet()
	{
		return mpFleet;
	}


	// MUTATORS
	//---------------------------
	inline void SetPlayerName(std::string name)
	{
		mName = name;
	}

	inline void SetFactionGrid(CGrid* pFactionGrid)
	{
		mpFactionGrid = pFactionGrid;
	}

	bool MineralTransaction(int amount);

	
	// METHODS
	//---------------------------
	virtual bool PurchaseStructure(CStructure* pStructure, CGrid* pGrid, CTile* pTile);
	CStructure* CheckStructureSelection(DX::XMFLOAT3 pos);
	bool QueueUnit(CStructure* structure, CGameAgent* unit);
	void LaunchAttack();


	// VIRTUAL METHODS
	//---------------------------
	void Update();


private:
	// PRIVATE CONSTRUCTOR
	//---------------------------
	CPlayer();


	// DATA
	//---------------------------
	std::string mName;
	int         mNumMinerals;
	int         mCurPopLimit;
	int         mCurPop;

	CGrid*  mpFactionGrid;
	CFleet* mpFleet;


	// LISTS
	//---------------------------
	std::vector<CStructure*> mpStructureList;
	std::vector<CGameAgent*> mpWorldUnitsList;
	std::vector<CSpaceUnit*> mpSpaceUnitsList;

	std::vector<CStructure*>::iterator mpiterStructures;
	std::vector<CGameAgent*>::iterator mpiterGameAgents;
	std::vector<CSpaceUnit*>::iterator mpiterSpaceUnits;
};


#endif /* _BASE_PLAYER_H_ */
