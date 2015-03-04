//-----------------------------------------------------
// FILE: GameAgent.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "GameAgent.h"


//-----------------------------------------------------
// GAME AGENT CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CGameAgent::CGameAgent()
{

}

CGameAgent::~CGameAgent()
{

}

//-----------------------------------------------------
// GAME AGENT CLASS ACCESSORS
//-----------------------------------------------------
eFleetPos CGameAgent::GetPosType()
{
	return mFleetPosition;
}

float CGameAgent::GetUnitSpacing()
{
	return 4.0f;
}

//-----------------------------------------------------
// GAME AGENT CLASS MUTATORS
//-----------------------------------------------------
bool CGameAgent::TakeDamage(float amount)
{
	mHealth -= amount;
	return false;
}

bool CGameAgent::Heal(float amount)
{
	mHealth += amount;
	return false;
}

bool CGameAgent::RayCollision(DX::XMFLOAT3 origin, DX::XMFLOAT3 direction, float& distance)
{
	return mBoundingSphere.GetCollisionDistance(DX::XMLoadFloat3(&origin),
		DX::XMLoadFloat3(&direction), distance);
}


void CGameAgent::DisplayInfo(IFont* font)
{
	mStrDisplay << "Structure: ";

	// Output selected building
	switch (mAgentInfo.mAgentType)
	{
	case GAV_ARTILLERY:
		mStrDisplay << "Artilery";
		break;

	case GAV_BOMBER:
		mStrDisplay << "Bomber";
		break;

	case GAV_FIGHTER:
		mStrDisplay << "Fighter";
		break;

	case GAV_INFANTRY:
		mStrDisplay << "Infantry";
		break;

	case GAV_MOTHERSHIP:
		mStrDisplay << "Mothership";
		break;

	case GAV_SPACE_FIGHTER:
		mStrDisplay << "Space Fighter";
		break;

	case GAV_TANK:
		mStrDisplay << "Tank";
		break;

	case GAV_TRANSPORT:
		mStrDisplay << "Transport";
		break;
	
	case GAV_WORKER:
		mStrDisplay << "Worker";
		break;
	}

	font->Draw(mStrDisplay.str(), 5, 805, kWhite, kLeft, kTop);
	mStrDisplay.str("");

	// Display additional data so long as the building is not dead
	if (mState != OBJ_DEAD)
	{
		// Display the status of the building
		mStrDisplay << "Building Status: ";
		switch (mState)
		{
		case OBJ_BUILT:
			mStrDisplay << "New";
			font->Draw(mStrDisplay.str(), 5, 815, kGreen, kLeft, kTop);
			break;

		case OBJ_DAMAGED:
			mStrDisplay << "Damaged";
			font->Draw(mStrDisplay.str(), 5, 815, 0xFD20, kLeft, kTop);
			break;

		case OBJ_WARNING:
			mStrDisplay << "WARNING!";
			font->Draw(mStrDisplay.str(), 5, 815, kRed, kLeft, kTop);
			break;
		}
		mStrDisplay.str("");

		size_t drawHeight = 805;
		int    counter = 1;
		
		font->Draw(mStrDisplay.str(), 480, 805, kWhite, kLeft, kTop);
		mStrDisplay.str("");

		mStrDisplay << "Health Remaining: " << mHealth;
		font->Draw(mStrDisplay.str(), 480, 815, kWhite, kLeft, kTop);
		mStrDisplay.str("");
	}

}

//-----------------------------------------------------
// GAME AGENT CLASS METHODS
//-----------------------------------------------------
bool CGameAgent::Construct()
{
	// Reduce construction time left
	mCurProductionTimeLeft -= gFrameTime;

	if (mCurProductionTimeLeft >= 0)
	{
		return false;
	}
	
	return true;
}
