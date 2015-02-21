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

	// Check if queue is at maximum
	if (mpProductionQueue.size() >= MAX_QUEUE_SIZE)
	{
		// *** TO DO *** return error code rather than boolean
		return false;
	}

	// Increment iterator to index
	miterRespectiveAgents = mRespectiveAgentsList.begin();
	std::advance(miterRespectiveAgents, agentIndex);

	// Create instance based on created type
	switch ((*miterRespectiveAgents)->GetAgentData()->mAgentType)
	{
	case GAV_ARTILLERY:
		mpProductionQueue.push(new CArtillery());
		break;

	case GAV_BOMBER:
		mpProductionQueue.push(new CBomber());
		break;

	case GAV_FIGHTER:
		mpProductionQueue.push(new CFighter());
		break;

	case GAV_INFANTRY:
		mpProductionQueue.push(new CInfantry());
		break;

	case GAV_MOTHERSHIP:
		mpProductionQueue.push(new CMothership());
		break;

	case GAV_SPACE_FIGHTER:
		mpProductionQueue.push(new CSpaceFighter());
		break;

	case GAV_TANK:
		mpProductionQueue.push(new CTank());
		break;

	case GAV_TRANSPORT:
		mpProductionQueue.push(new CTransport());
		break;

	case GAV_WORKER:
		mpProductionQueue.push(new CWorker());
		break;
	}

	// Success
	return true;
}

bool CProductionStructure::RemoveFromQueue()
{
	CGameAgent* tmp = mpProductionQueue.front();
	tmp->Spawn(mpGrid, GetBuildLocation());
	
	//delete tmp;
	mpProductionQueue.pop();
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

void CProductionStructure::UpdateKeyPresses()
{
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
}


//-----------------------------------------------------
// PRODUCTION STRUCTURE CLASS OVERRIDE METHODS
//-----------------------------------------------------
void CProductionStructure::UnloadIModel()
{

}

void CProductionStructure::SetBuiltModel()
{

}

bool CProductionStructure::Update()
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
			// Object still alive
			return true;

			break;

		case OBJ_BUILT:
			// Update the modle being built at front of queue
			if (mpProductionQueue.size() != 0)
			{
				if (mpProductionQueue.front()->Construct())
				{
					// Unit fully constructed - for now remove from queue
					RemoveFromQueue();
				}
			}
			// Object still alive
			return true;

			break;

		case OBJ_DAMAGED:
			// Object still alive
			return true;

			break;

		case OBJ_WARNING:
			// Object still alive
			return true;

			break;

		case OBJ_DEAD:

			// ONCE THE PARTICLES FOR THE DESTRUCTION OF BUILDING HAS FINISHED
			// only then call the destory! - Particles not yet implemented
			Destroy();

			// Object no longer alive
			return false;

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
			mStrDisplay << counter << ": " << (*miterRespectiveAgents)->GetAgentData()->mAgentName;
			font->Draw(mStrDisplay.str(), 240, drawHeight, kWhite, kLeft, kTop);
			mStrDisplay.str("");

			drawHeight += 10;
			counter++;
		}

		// Display queue data
		mStrDisplay << "Queue Next: ";
		if (mpProductionQueue.size() == 0)
		{
			mStrDisplay << "None";
		}
		else
		{
			mStrDisplay << mpProductionQueue.front()->GetAgentData()->mAgentName;
		}
		font->Draw(mStrDisplay.str(), 480, 805, kWhite, kLeft, kTop);
		mStrDisplay.str("");

		mStrDisplay << "Queue Size: " << mpProductionQueue.size();
		font->Draw(mStrDisplay.str(), 480, 815, kWhite, kLeft, kTop);
		mStrDisplay.str("");

		// Display how long is left of the unit being constructed
		if (mpProductionQueue.size() != 0)
		{
			// Display the time left until build
			float timeLeft = mpProductionQueue.front()->GetCurProductionTimeLeft();
			mStrDisplay << "Build time left: " << static_cast<int>(timeLeft);
			font->Draw(mStrDisplay.str(), 725, 805, kWhite, kLeft, kTop);
			mStrDisplay.str("");

			// Show percentage completion
			float buildTime = mpProductionQueue.front()->GetProductionTime();
			mStrDisplay << "Percentage Complete: " << static_cast<int>(((buildTime - timeLeft) / buildTime) * 100.0f) << "%";
			font->Draw(mStrDisplay.str(), 725, 815, kWhite, kLeft, kTop);
			mStrDisplay.str("");
		}
	}

	if (mState != OBJ_CONSTRUCTING)
		UpdateKeyPresses();
}
