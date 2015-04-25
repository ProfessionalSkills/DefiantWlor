//-----------------------------------------------------
// FILE: Player.h
//-----------------------------------------------------

#ifndef _PLAYER_H_
#define _PLAYER_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include <string>
#include <sstream>
#include <fstream>

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
#include "Wall.h"				// Static structure

#include "Barracks.h"			// Production structure
#include "ComCentre.h"			// Production structure
#include "Hellipad.h"			// Production structure
#include "SpaceCentre.h"		// Production structure


typedef std::unordered_multimap<EGameStructureTypes, CStructure*> GS_MultiMap;
typedef std::unordered_multimap<EGameAgentVariations, CGameAgent*> GA_MultiMap;


//-----------------------------------------------------
// BASE CLASS FOR PLAYERS
//-----------------------------------------------------
class CRTSPlayer
{
protected:
	// DATA
	//---------------------------
	CRandomiser* mpRandomiser;
	std::string mName;
	EFactions mPlayerFaction;

	int mNumMinerals;
	int mMineralBaseAddition;
	int mMineralBonus = 10;
	int mPopLimit;
	int mCurPop;

	int mNumSpaceFighter;
	int mNumMothership;
	int mNumTransport;

	int mNumArtillery;
	int mNumInfantry;
	int mNumTank;
	int mNumFighter; 
	int mNumBomber;
	CGrid* mpPlayerGrid;

	const float MINERAL_UPDATE_TIME;
	float mTimeToMineralUpdate;


	// LISTS
	//---------------------------
	GS_MultiMap mpStructuresMap;
	GA_MultiMap mpUnitsMap;

	GS_MultiMap::iterator miterStructuresMap;
	GA_MultiMap::iterator miterUnitsMap;



	std::vector<CGameAgent*> mpSpaceUnitsList;
	std::vector<CGameAgent*>::iterator mpiterSpaceUnits;

	std::vector<CMinerals*> mpMineralsList;
	std::vector<CMinerals*>::iterator miterMineralsList;
	

	// FLEET DATA
	//---------------------------
	CFleet* mpFleet;
	bool mWonLastSpaceBattle;


	// MISC
	//---------------------------
	IModel* mpPillars[8];


public:
	// PILLAR MESH
	//---------------------------
	static IMesh* mspMshPillar;


	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CRTSPlayer(EFactions playerFaction, int startingResources);
	virtual ~CRTSPlayer();


	// ACCESSORS
	//---------------------------
	inline std::string GetPlayerName()
	{
		return mName;
	}

	inline bool PutUnitsOnShips(CGameAgent* tmp)
	{
		for (int i = 0; i < mpSpaceUnitsList.size(); i++)
		{
			//space uni that it will attempt to to store a unit on
			CSpaceUnit* mpTemp = (CSpaceUnit*)(mpSpaceUnitsList[i]);

			if (mpTemp->StoreUnits(tmp))
			{
				tmp->SetState(OBJ_INSPACE);
				tmp->UnloadIModel();
				tmp->RemoveBoundingSphere();
				return true;
			}
		}
		return false;
	}

	inline bool GetWonLastSpaceBattle()
	{
		return mWonLastSpaceBattle;
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
		return mPopLimit;
	}

	inline CFleet* GetFleet()
	{
		return mpFleet;
	}

	inline std::vector<CGameAgent*>* GetSpaceUnitList()
	{
		return &mpSpaceUnitsList;
	}

	inline GA_MultiMap* GetWorldUnitList()
	{
		return &mpUnitsMap;
	}

	inline GS_MultiMap* GetStructuresList()
	{
		return &mpStructuresMap;
	}

	inline int GetAmountOfStructures()
	{
		return mpStructuresMap.size();
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

	// Function to determine whether or not the player's command centre is still standing
	bool IsAlive();

	// Get a random ground unit from the player
	CGameAgent* GetRandomAgent();
	CStructure* GetRandomStructure();


	// MUTATORS
	//---------------------------
	inline void SetPlayerName(std::string name)
	{
		mName = name;
	}

	inline void SetWonLastSpaceBattle(bool victory)
	{
		mWonLastSpaceBattle = victory;
	}

	inline void LosePop(int popLost)
	{
		mCurPop -= popLost;
		if (mCurPop < 0)mCurPop = 0;
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

	inline void IncreasePopValue(int value)
	{
		mCurPop += value;
	}

	//Debugging function to add ships
	inline void AddShips()
	{
		if (mPlayerFaction == FAC_EARTH_DEFENSE_FORCE)
		{
			CSpaceFighter* Temp;
			for (int i = 0; i < 25; i++)
			{
				Temp = new CSpaceFighter();
				mCurPop += Temp->GetPopValue();
				Temp->SetFaction(mPlayerFaction);
				mpSpaceUnitsList.push_back(Temp);
				mNumSpaceFighter++;
			}
			CTransport* temp;
			for (int i = 0; i < 5; i++)
			{
				temp = new CTransport();
				mCurPop += temp->GetPopValue();
				temp->SetFaction(mPlayerFaction);
				mpSpaceUnitsList.push_back(temp);
				mNumTransport++;
			}
			mCurPop += 10;
			mpSpaceUnitsList.push_back(new CMothership());
			mpSpaceUnitsList.back()->SetFaction(mPlayerFaction);
			mNumMothership++;
		}
		// Add less ships for AI
		else
		{
			CSpaceFighter* Temp;
			for (int i = 0; i < 10; i++)
			{
				Temp = new CSpaceFighter();
				mCurPop += Temp->GetPopValue();
				Temp->SetFaction(mPlayerFaction);
				mpSpaceUnitsList.push_back(Temp);
				mNumSpaceFighter++;
			}
			mCurPop += 10;
			mpSpaceUnitsList.push_back(new CMothership());
			mpSpaceUnitsList.back()->SetFaction(mPlayerFaction);
			mNumMothership++;
		}
	}

	bool MineralTransaction(int amount);

	void SetNumSpaceFighter(int NumShips);
	void SetNumTransport(int NumShips);
	void SetNumMothership(int NumShips);


	// METHODS
	//---------------------------
	EErrorTypes PurchaseStructure(CStructure* pStructure, CGrid* pGrid, CTile* pTile);
	void CheckGameObjectSelection(CStructure*& pStructure, CGameAgent*& pGameAgent,
		CMinerals*& pMinerals, DX::XMFLOAT3 origin, DX::XMFLOAT3 direction, float& curDist, bool toCheckMinerals);
	bool CheckDragSelection(DX::BoundingBox& box, std::vector<CGameAgent*>& selectedAgents);
	bool QueueUnit(CStructure* structure, CGameAgent* unit);
	void LaunchAttack();
	virtual void Update();
	void CreateResourcePiles();
	void CreatePillars();
	void RemovePillars();
	void LoadPlayerGridModels();
	void UnloadPlayerGridModels();
	bool PopLimitReached(int popValue);

	// Function to create the walls surrounding the player's building area
	void ConstructWalls();

	// Save all player data
	void SavePlayerData(std::ofstream& outFile);
	void LoadPlayerData(std::ifstream& inFile);
};


#endif /* _PLAYER_H_ */
