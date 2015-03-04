//-----------------------------------------------------
// FILE: ProductionStructure.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "ProductionStructure.h"
#include "Player.h"


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
bool CProductionStructure::AddToQueue(size_t agentIndex, CRTSPlayer* pPlayer)
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
		mpProductionQueue.push_back(new CArtillery());
		break;

	case GAV_BOMBER:
		mpProductionQueue.push_back(new CBomber());
		break;

	case GAV_FIGHTER:
		mpProductionQueue.push_back(new CFighter());
		break;

	case GAV_INFANTRY:
		mpProductionQueue.push_back(new CInfantry());
		break;

	case GAV_MOTHERSHIP:
		mpProductionQueue.push_back(new CMothership());
		break;

	case GAV_SPACE_FIGHTER:
		mpProductionQueue.push_back(new CSpaceFighter());
		break;

	case GAV_TANK:
		mpProductionQueue.push_back(new CTank());
		break;

	case GAV_TRANSPORT:
		mpProductionQueue.push_back(new CTransport());
		break;

	case GAV_WORKER:
		mpProductionQueue.push_back(new CWorker());
		break;
	}

	// Check if the player has enough funds
	if (pPlayer->GetMineralAmount() - mpProductionQueue.front()->GetBuildCost() < 0)
	{
		// Not enough funds - remove the object at the front of the queue
		RemoveFromQueue(0);
		return false;
	}

	// Enough funds - subtract them
	pPlayer->MineralTransaction(-mpProductionQueue.front()->GetBuildCost());

	// Success
	return true;
}

std::deque<CGameAgent*>* CProductionStructure::GetQueue()
{
	return &mpProductionQueue;
}

void CProductionStructure::RemoveFromQueue(size_t agentIndex)
{
	// Increment the iterator by the amount provided
	miterProdQ = mpProductionQueue.begin();
	std::advance(miterProdQ, agentIndex);

	// Remove the unit at the given location
	CGameAgent* tmp = (*miterProdQ);
	SafeDelete(tmp);
	mpProductionQueue.erase(miterProdQ);
}

bool CProductionStructure::SpawnFront()
{
	CGameAgent* tmp = mpProductionQueue.front();
	tmp->Spawn(mpGrid, GetGridSpawnLocation());

	//delete tmp;
	mpProductionQueue.pop_front();
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
void CProductionStructure::UnloadIModel()
{

}

void CProductionStructure::SetBuiltModel()
{

}

bool CProductionStructure::Update(CRTSPlayer* pPlayer)
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
					// Unit fully constructed - add to units list
					if (mStructureType == STR_SPACE_CENTRE)
					{
						// Space units list
						pPlayer->GetSpaceUnitList()->push_back(mpProductionQueue.front());
					}
					else
					{
						// World units list
						EGameAgentVariations agentVar = mpProductionQueue.front()->GetAgentData()->mAgentType;
						pPlayer->GetWorldUnitList()->insert(GA_MultiMap::value_type(agentVar, mpProductionQueue.front()));
					}
					// Spawn the unit
					SpawnFront();
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

		default:
			return true;

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
	}
}
