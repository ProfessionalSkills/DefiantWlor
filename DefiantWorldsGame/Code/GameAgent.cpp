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

bool CGameAgent::SphereCollision(SBoundingSphere& sphere)
{
	return sphere.CollidingWith(mBoundingSphere.mSphere);
}

bool CGameAgent::DragBoxCollision(DX::BoundingBox& box)
{
	return mBoundingSphere.CollidingWith(box);
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
void CGameAgent::CalculateBoundingSphere()
{
	DX::XMFLOAT3 centre = { mpObjModel->GetX(), mpObjModel->GetY(), mpObjModel->GetZ() };
	float radius = 5.0f;

	mBoundingSphere = SBoundingSphere(centre, radius);
}

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
void CGameAgent::Destroy()
{
	// Alert news ticker if structure belongs to the player
	if (mFaction == FAC_EARTH_DEFENSE_FORCE)
	{
		switch (GetAgentData()->mAgentType)
		{
		case GAV_ARTILLERY:
			mStrDisplay << "An Artillery";
			break;

		case GAV_BOMBER:
			mStrDisplay << "A Bomber";
			break;

		case GAV_FIGHTER:
			mStrDisplay << "A Fighter";
			break;

		case GAV_INFANTRY:
			mStrDisplay << "Infantry";
			break;

		case GAV_MOTHERSHIP:
			mStrDisplay << "A Mothership";
			break;

		case GAV_SPACE_FIGHTER:
			mStrDisplay << "A Space Fighter";
			break;

		case GAV_TANK:
			mStrDisplay << "A Tank";
			break;

		case GAV_TRANSPORT:
			mStrDisplay << "A Transport Ship";
			break;

		case GAV_WORKER:
			mStrDisplay << "A Worker";
			break;
		}
		mStrDisplay << " has been destroyed!";
		gpNewsTicker->AddNewElement(mStrDisplay.str(), true);
		mStrDisplay.str("");
	}

	// Remove the model
	mDestructionExplosion = new CExplosion(mpObjModel, 100);
	UnloadIModel();
}

void CGameAgent::SaveAgent(std::ofstream& outFile)
{
	// Save the data for this unit
	outFile << mAgentInfo.mAgentType << " " << mFaction << " " << mState << " " << mWorldPos.x << " "
		<< mWorldPos.y << " " << mWorldPos.z << " " << mHealth << std::endl;
}

void CGameAgent::LoadAgent(std::ifstream& inFile)
{
	// Ensure the model for the unit is not already loaded
	UnloadIModel();

	// Store the required data for the structure
	int faction;
	int state;
	int qSize;
	inFile >> faction >> state >> mWorldPos.x >> mWorldPos.y >> mWorldPos.z >> mHealth;

	// Convert required values to enums
	mFaction = static_cast<EFactions>(faction);
	mState = static_cast<EObjectStates>(state);

	// Load the model
	LoadIModel();

	// Calculate the bounding sphere
	CalculateBoundingSphere();
}
