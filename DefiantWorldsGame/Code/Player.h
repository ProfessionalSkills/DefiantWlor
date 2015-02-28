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

	int mNumSpaceFighter;
	int mNumMothership;
	int mNumTransport;


	CGrid* mpPlayerGrid;


	// LISTS
	//---------------------------
	std::vector<CStructure*> mpStructureList;
	std::vector<CGameAgent*> mpWorldUnitsList;
	std::vector<CGameAgent*> mpSpaceUnitsList;

	std::vector<CStructure*>::iterator mpiterStructures;
	std::vector<CGameAgent*>::iterator mpiterGameAgents;
	std::vector<CGameAgent*>::iterator mpiterSpaceUnits;

	CFleet* mpFleet;


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CRTSPlayer(EFactions playerFaction);
	virtual ~CRTSPlayer();


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

	inline std::vector<CGameAgent*>* GetSpaceUnitList()
	{
		return &mpSpaceUnitsList;
	}

	inline std::vector<CGameAgent*>* GetWorldUnitList()
	{
		return &mpWorldUnitsList;
	}

	inline int GetAmountOfStructures()
	{
		return mpStructureList.size();
	}

	inline EFactions GetPlayerFaction()
	{
		return mPlayerFaction;
	}

	inline CGrid* GetPlayerGrid()
	{
		return mpPlayerGrid;
	}

	inline int GetNumSpaceFighter()
	{
		return mNumSpaceFighter;
	}

	inline int GetNumTransport()
	{
		return mNumTransport;
	}

	inline int GetNumMothership()
	{
		return mNumMothership;
	}


	// MUTATORS
	//---------------------------
	inline void SetPlayerName(std::string name)
	{
		mName = name;
	}

	inline void SetSpaceUnitList(std::vector<CGameAgent*>* newUnits)
	{
		for (size_t i = 0; i < newUnits->size(); i++)
		{
			mpSpaceUnitsList.push_back((*newUnits)[i]);
		}
	}

	inline void StorePlayerGridState(CGrid* pGrid)
	{
		mpPlayerGrid = pGrid;
	}

	bool MineralTransaction(int amount);

	void SetNumSpaceFighter(int NumShips);
	void SetNumTransport(int NumShips);
	void SetNumMothership(int NumShips);


	// METHODS
	//---------------------------
	bool PurchaseStructure(CStructure* pStructure, CGrid* pGrid, CTile* pTile);
	CStructure* CheckStructureSelection(DX::XMFLOAT3 origin, DX::XMFLOAT3 direction);
	bool QueueUnit(CStructure* structure, CGameAgent* unit);
	void LaunchAttack();
	virtual void Update();
	void LoadStructureModels();
	void UnloadStructureModels();
	void LoadUnitModels();
	void UnloadUnitModels();
};


#endif /* _PLAYER_H_ */
