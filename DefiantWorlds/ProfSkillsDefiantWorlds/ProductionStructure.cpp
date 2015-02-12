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
