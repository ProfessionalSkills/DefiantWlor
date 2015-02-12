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
	else
	{

	}
}
