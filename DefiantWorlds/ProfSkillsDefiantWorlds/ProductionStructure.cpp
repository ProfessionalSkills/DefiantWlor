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
// PRODUCTION STRUCTURE CLASS METHODS
//-----------------------------------------------------
bool CProductionStructure::AddToQueue(int agentIndex)
{
	// Get the size of the respective agents array and compare with index
	if (agentIndex >= mRespectiveAgentsList.size())
	{
		// *** TO DO *** return error code rather than boolean
		return false;
	}

	// Increment iterator to index
	miterRespectiveAgents = mRespectiveAgentsList.begin();
	std::advance(miterRespectiveAgents, agentIndex);

	// Determine which agent is to be created
	switch ((*miterRespectiveAgents).mAgentType)
	{
		case 
	}

	// Success
	return true;
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
			// UNIT CONSTRUCTION
			//------------------------------
			bool addedSuccess;

			// 1 = First unit
			if (gpEngine->KeyHit(Key_1))
			{
				addedSuccess = AddToQueue(0);
			}

			// 2 = Second unit
			if (gpEngine->KeyHit(Key_2))
			{
				addedSuccess = AddToQueue(1);
			}

			// 3 = Third unit
			if (gpEngine->KeyHit(Key_3))
			{
				addedSuccess = AddToQueue(2);
			}

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
			mStrDisplay << counter << ": " << (*miterRespectiveAgents).mAgentName;
			font->Draw(mStrDisplay.str(), 240, drawHeight, kWhite, kLeft, kTop);
			mStrDisplay.str("");

			drawHeight += 10;
			counter++;
		}

		// Display if a unit is queued and what unit
	}
}
