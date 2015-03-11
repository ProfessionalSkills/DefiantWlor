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
	// Check state of the production building
	if (mState == OBJ_CONSTRUCTING)
	{
		// Cannot queue units before the structure is ready
		return false;
	}

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

	miterRespectiveAgents = mRespectiveAgentsList.begin();
	std::advance(miterRespectiveAgents, agentIndex);

	if (pPlayer->PopLimitReached((*miterRespectiveAgents)->GetPopValue()))
	{
		return false;
	}
	else
	{
		pPlayer->IncreasePopValue((*miterRespectiveAgents)->GetPopValue());
	}
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
		RemoveFromQueue(mpProductionQueue.size() - 1, pPlayer);
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

void CProductionStructure::RemoveFromQueue(size_t agentIndex,CRTSPlayer* pPlayer)
{
	// Increment the iterator by the amount provided
	miterProdQ = mpProductionQueue.begin();
	std::advance(miterProdQ, agentIndex);
	pPlayer->IncreasePopValue(-(*miterRespectiveAgents)->GetPopValue());
	// Remove the unit at the given location
	CGameAgent* tmp = (*miterProdQ);
	SafeDelete(tmp);
	mpProductionQueue.erase(miterProdQ);
}

bool CProductionStructure::SpawnFront()
{
	CGameAgent* tmp = mpProductionQueue.front();
	tmp->Spawn(mpGrid, GetGridSpawnLocation());

	// Display data on news ticker if this concerns the player
	if (mFaction == FAC_EARTH_DEFENSE_FORCE)
	{
		mStrDisplay << "A new " << mpProductionQueue.front()->GetAgentData()->mAgentName
			<< " unit is ready for your orders.";
		gpNewsTicker->AddNewElement(mStrDisplay.str(), false);
		mStrDisplay.str("");
	}

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
				if (mStructureType != STR_COM_CENTRE && pPlayer->GetPlayerFaction() == FAC_EARTH_DEFENSE_FORCE) //If the structure is not the player's command centre and belongs to the human player
				{
					string mMusicFile = "drill.wav"; //Sets the music file
					DX::XMFLOAT3 mSourcePos = { mpObjModel->GetX(), mpObjModel->GetY(), mpObjModel->GetZ() };
					DX::XMFLOAT3 mSourceVel = { 0.0f, 0.0f, 0.0f };
					DX::XMFLOAT3 listenerPos = { mpObjModel->GetX(), 50.0f, mpObjModel->GetZ() };
					DX::XMFLOAT3 listenerVel = { 0.0f, 0.0f, 0.0f };
					mGenSound = new CSound(mMusicFile, mSourcePos, mSourceVel, false, 0.51f, listenerPos, listenerVel); //Initialise music
					mGenSound->PlaySound(); //construction sound

					// Display on ticker
					switch (GetStructureType())
					{
					case STR_BARRACKS:
						mStrDisplay << "The new Barracks";
						break;

					case STR_HELLIPAD:
						mStrDisplay << "The new Hellipad";
						break;

					case STR_SPACE_CENTRE:
						mStrDisplay << "The new Space Centre";
						break;
					}
					mStrDisplay << " has been constructed.";
					gpNewsTicker->AddNewElement(mStrDisplay.str(), false);
					mStrDisplay.str("");
				}
			
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

	font->Draw(mStrDisplay.str(), 470, 800, kWhite, kLeft, kTop);
	mStrDisplay.str("");
}
