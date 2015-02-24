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
// BASE CLASS FOR PLAYERS
//-----------------------------------------------------
class CRTSPlayer
{
protected:
	// DATA
	//---------------------------
	std::string mName;
	EFactions mPlayerFaction;
	int mNumMinerals;
	int mCurPopLimit;
	int mCurPop;


	// LISTS
	//---------------------------
	std::vector<CStructure*> mpStructureList;
	std::vector<CGameAgent*> mpWorldUnitsList;
	std::vector<CSpaceUnit*> mpSpaceUnitsList;

	std::vector<CStructure*>::iterator mpiterStructures;
	std::vector<CGameAgent*>::iterator mpiterGameAgents;
	std::vector<CSpaceUnit*>::iterator mpiterSpaceUnits;

	CFleet* mpFleet;


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CRTSPlayer(EFactions playerFaction);
	~CRTSPlayer();


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

	inline CFleet* GetFleet()
	{
		return mpFleet;
	}

	inline std::vector<CSpaceUnit*> GetSpaceUnitList()
	{
		return mpSpaceUnitsList;
	}

	inline int GetAmountOfStructures()
	{
		return mpStructureList.size();
	}

	inline EFactions GetPlayerFaction()
	{
		return mPlayerFaction;
	}


	// MUTATORS
	//---------------------------
	inline void SetPlayerName(std::string name)
	{
		mName = name;
	}

	inline void SetSpaceUnitList(std::vector<CSpaceUnit*> newUnits)
	{
		for (int i = 0; i < newUnits.size(); i++)
		{
			mpSpaceUnitsList.push_back(newUnits[i]);
		}
	}

	bool MineralTransaction(int amount);


	// METHODS
	//---------------------------
	bool PurchaseStructure(CStructure* pStructure, CGrid* pGrid, CTile* pTile);
	CStructure* CheckStructureSelection(DX::XMFLOAT3 pos);
	bool QueueUnit(CStructure* structure, CGameAgent* unit);
	void LaunchAttack();
	void Update();
	void LoadStructureModels();
	void UnloadStructureModels();
};


#endif /* _PLAYER_H_ */
