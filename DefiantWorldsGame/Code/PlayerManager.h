//-----------------------------------------------------
// FILE: PlayerManager.h
//-----------------------------------------------------

#ifndef _PLAYER_MANAGER_H_
#define _PLAYER_MANAGER_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "AIPlayer.h"
#include "TransportBeam.h"


//-----------------------------------------------------
// PLAYER MANAGER CLASS
//-----------------------------------------------------
class CPlayerManager
{
public:
	// CONSTRUCTOR & DESTRUCTOR
	//---------------------------
	CPlayerManager();
	~CPlayerManager();


	// METHODS
	//---------------------------
	void CreatePlayers(EFactions humanFaction, int numAI, int startingResources, int difficulty);
	void RemovePlayers();
	int UpdatePlayers();
	void SavePlayers(std::ofstream& outFile);
	void LoadPlayers(std::ifstream& inFile);
	void InvadeEarth();
	void InvadeMars();

	// Check clicking/selection of rebel units
	void CheckRebelSelection(CStructure*& pStructure, CGameAgent*& pGameAgent, CMinerals*& pMinerals,
		DX::XMFLOAT3 origin, DX::XMFLOAT3 direction, float& curDist);

	// Get all walls still alive
	void GetWorldWalls(std::vector<CStaticStructure*>& pWalls);


	// MUTATORS
	//---------------------------
	inline void SetGameTime(float time)
	{
		mTimeSinceGameStart = time;
	}

	inline void AddToEarthAirspace(CGameAgent* pAgent)
	{
		mpEarthAirspaceList.push_back(pAgent);
	}

	inline void AddToMarsAirspace(CGameAgent* pAgent)
	{
		mpMarsAirspaceList.push_back(pAgent);
	}

	inline void ClearMarsAirspace()
	{
		mpMarsAirspaceList.clear();
	}

	inline void ClearEarthAirspace()
	{
		mpEarthAirspaceList.clear();
	}


	// ACCESSORS
	//---------------------------
	inline CRTSPlayer* GetHumanPlayer()
	{
		return mpHuman;
	}

	inline CRTSAIPlayer* GetAIPlayer(int index = 0)
	{
		return mpAI[index];
	}

	inline bool ArePlayersInitialised()
	{
		return mPlayerDataInitialised;
	}


private:
	// PLAYERS
	//---------------------------
	CRTSPlayer* mpHuman;
	CRTSAIPlayer* mpAI[MAX_AI_NUM];

	int mNumAI;

	bool mPlayerDataInitialised;

	// Airspace lists
	std::vector<CGameAgent*> mpEarthAirspaceList;
	std::vector<CGameAgent*> mpMarsAirspaceList;


	// REBEL PLAYER DATA
	//---------------------------
	float mTimeSinceGameStart = 0.0f;
	float mTimeToEarthInvasion = 0.0f;
	float mTimeToMarsInvasion = 0.0f;

	int mEarthUnits = 0;
	int mMarsUnits = 0;

	// Units list
	std::vector<CGameAgent*> mpRebelEarthList;
	std::vector<CGameAgent*> mpRebelMarsList;

	// Transport beams
	std::vector<CTransportBeam*> mpTransportBeams;
};


#endif /* _PLAYER_MANAGER_H_ */
