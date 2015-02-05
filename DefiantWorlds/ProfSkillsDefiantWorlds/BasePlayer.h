//-----------------------------------------------------
// FILE: BasePlayer.h
//-----------------------------------------------------

#ifndef _BASE_PLAYER_H_
#define _BASE_PLAYER_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "SpaceFighter.h"
#include "Mothership.h"
#include "Transport.h"
#include "Worker.h"
#include "Infantry.h"
#include "Tank.h"
#include "Artillery.h"


//-----------------------------------------------------
// CONSTANTS
//-----------------------------------------------------
const int MAX_UNITS = 50;
const int MAX_SPACE_UNITS = 10;


//-----------------------------------------------------
// INTERFACE CLASS FOR PLAYERS
//-----------------------------------------------------
class CPlayer
{
private:
	// DATA
	//---------------------------
	std::string mName;
	int         mNumMinerals;
	int         mCurPopLimit;
	int         mCurPop;
	bool        mIsCommsDestroyed;
	

	// LISTS
	//---------------------------
	// std::vector<CStructure*> mStructureList;
	std::vector<CGameAgent*> mWorldUnitsList;
	std::vector<CSpaceUnit*> mSpaceUnitsList;


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
	//bool PurchaseStructure(CStructure* structure, DX::XMFLOAT2 pos);
	//bool QueueUnit(CStructure* structure, CGameAgent* unit);


	// VIRTUAL METHODS
	//---------------------------
	virtual void Update(float delta) = 0;
};


#endif /* _BASE_PLAYER_H_ */
