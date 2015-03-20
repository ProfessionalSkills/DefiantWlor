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
	mPopCost = 0;
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
	return mUnitSpacing;
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

bool CGameAgent::DragBoxCollision(DX::BoundingBox& box)
{
	return mBoundingSphere.CollidingWithBox(box);
}


void CGameAgent::DisplayInfo(IFont* font)
{
	// Output selected agent
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

	font->Draw(mStrDisplay.str(), 470, 800, kWhite, kLeft, kTop);
	mStrDisplay.str("");

	// Display health
	mStrDisplay << (int)mHealth << " \\ " << (int)mMaxHealth;
	font->Draw(mStrDisplay.str(), 1130, 800, kWhite, kRight, kTop);
	mStrDisplay.str("");
}

//-----------------------------------------------------
// GAME AGENT CLASS METHODS
//-----------------------------------------------------
void CGameAgent::SetState(EObjectStates newState)
{
	mState = newState;
}

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
