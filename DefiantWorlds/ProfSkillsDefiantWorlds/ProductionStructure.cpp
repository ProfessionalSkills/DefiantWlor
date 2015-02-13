//-----------------------------------------------------
// FILE: ProductionStructure.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "ProductionStructure.h"


//-----------------------------------------------------
// PRODUCTION STRUCTURE CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CProductionStructure::CProductionStructure()
{

}

CProductionStructure::~CProductionStructure()
{

}

//-----------------------------------------------------
// PRODUCTION STRUCTURE CLASS ACCESSORS
//-----------------------------------------------------
std::string CProductionStructure::GetAgentName(EGameAgentsTypes agentType)
{
	switch (agentType)
	{
	case GA_ARTILLERY:
		return "Artillery";
		break;

	case GA_BOMBER:
		return "Bomber";
		break;

	case GA_FIGHTER:
		return "Fighter";
		break;

	case GA_INFANTRY:
		return "Infantry";
		break;

	case GA_MOTHERSHIP:
		return"Mothership";
		break;

	case GA_SPACE_FIGHTER:
		return "Space Fighter";
		break;

	case GA_TANK:
		return "Tank";
		break;

	case GA_TRANSPORT:
		return "Transport Ship";
		break;

	case GA_WORKER:
		return "Worker";
		break;
	}

	return "UNKOWN";
}


//-----------------------------------------------------
// PRODUCTION STRUCTURE CLASS METHODS
//-----------------------------------------------------
bool CProductionStructure::AddToQueue(CGameAgent* agent)
{
	return false;
}

bool CProductionStructure::RemoveFromQueue()
{
	return false;
}

bool CProductionStructure::UpdateProduction()
{
	return false;
}

CGameAgent* CProductionStructure::CreateAgent()
{
	return nullptr;
}


//-----------------------------------------------------
// PRODUCTION STRUCTURE CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CProductionStructure::Destroy()
{
	return false;
}

void CProductionStructure::UnloadIModel()
{

}

void CProductionStructure::SetBuiltModel()
{

}

void CProductionStructure::Update()
{
	// Determine state of the structure
	switch (mState)
	{
		case OBJ_CONSTRUCTING:
			// Update the timer left until construction is finished
			mCurBuildTimeLeft -= gFrameTime;

			// Check if the timer has reached 0
			if (mCurBuildTimeLeft < 0.0f)
			{
				// Set the building to the correct model type
				SetBuiltModel();

				// Change status of the building to 'built'
				mState = OBJ_BUILT;
			}

			break;

		case OBJ_BUILT:

			break;

		case OBJ_DAMAGED:

			break;

		case OBJ_WARNING:

			break;

		case OBJ_DEAD:
			// Call Destroy function until destruction animation is finished. Then deallocate the object
			// and ensure area it was in can be built upon i.e. grid tiles no longer in use

			break;
	}
}

void CProductionStructure::DisplayInfo(IFont* font)
{
	mStrDisplay << "Structure: ";
	
	// Output selected building
	switch (mStructureType)
	{
	case STR_BARRACKS:
		mStrDisplay << "Barracks";
		break;

	case STR_COM_CENTRE:
		mStrDisplay << "Command Centre";
		break;

	case STR_HELLIPAD:
		mStrDisplay << "Hellipad";
		break;

	case STR_SPACE_CENTRE:
		mStrDisplay << "Space Centre";
		break;
	}

	font->Draw(mStrDisplay.str(), 5, 805, kWhite, kLeft, kTop);
	mStrDisplay.str("");

	// Check to see what state the building is in
	if (mState == OBJ_CONSTRUCTING)
	{
		// Display the time left until build
		mStrDisplay << "Build time left: " << static_cast<int>(mCurBuildTimeLeft);
		font->Draw(mStrDisplay.str(), 5, 815, kWhite, kLeft, kTop);
		mStrDisplay.str("");

		// Show percentage completion
		mStrDisplay << "Percentage Complete: " << static_cast<int>(((mBuildTime - mCurBuildTimeLeft) / mBuildTime) * 100.0f) << "%";
		font->Draw(mStrDisplay.str(), 5, 825, kWhite, kLeft, kTop);
		mStrDisplay.str("");
	}
	// Display additional data so long as the building is not dead
	else if (mState != OBJ_DEAD)
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
		// Display key presses for possible unit queuing
		for (miterRespectiveAgents = mRespectiveAgentsList.begin(); miterRespectiveAgents != mRespectiveAgentsList.end(); miterRespectiveAgents++)
		{
			mStrDisplay << counter << ": " << GetAgentName(*miterRespectiveAgents);
			font->Draw(mStrDisplay.str(), 240, drawHeight, kWhite, kLeft, kTop);
			mStrDisplay.str("");

			drawHeight += 10;
			counter++;
		}

		// Display if a unit is queued and what unit
	}
}
