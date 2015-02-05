//-----------------------------------------------------
// FILE: BasePlayer.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "BasePlayer.h"


//-----------------------------------------------------
// BASE PLAYER CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CPlayer::CPlayer()
{
	
}

CPlayer::~CPlayer()
{

}


//-----------------------------------------------------
// BASE PLAYER CLASS MUTATORS
//-----------------------------------------------------
bool CPlayer::MineralTransaction(int amount)
{
	if (mNumMinerals + amount < 0)
	{
		// Not enough funds for transaction
		return false;
	}
	else
	{
		// Enough funds
		mNumMinerals += amount;
		return true;
	}
}


//-----------------------------------------------------
// BASE PLAYER CLASS MUTATORS
//-----------------------------------------------------
//bool CPlayer::PurchaseStructure(CStructure* structure, DX::XMFLOAT2 pos)

//bool CPlayer::QueueUnit(CStructure* structure, CGameAgent* unit)
