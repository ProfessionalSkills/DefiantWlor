//-----------------------------------------------------
// FILE: WorldState.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "WorldState.h"
#include "GameStateControl.h"
#include "GameSaverLoader.h"


IMesh* CWorldState::mspMshDrag = nullptr;
IMesh* CWorldState::mspMshUnitShadow = nullptr;


//-----------------------------------------------------
// WORLD STATE CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CWorldState::CWorldState() : CGameState()
{

}

CWorldState::~CWorldState()
{
	
}

//-----------------------------------------------------
// WORLD STATE CLASS METHODS
//-----------------------------------------------------
void CWorldState::UpdateMatrices()
{
	// Store the current camera's model matrix
	mpCamCurrent->GetCamera()->GetMatrix(&mCamWorldMatrix.m[0][0]);

	// XMMATRIX variables calculations
	DX::XMMATRIX world = DX::XMLoadFloat4x4(&mCamWorldMatrix);
	DX::XMMATRIX view = DX::XMMatrixInverse(NULL, DX::XMLoadFloat4x4(&mCamWorldMatrix));
	DX::XMMATRIX proj = DX::XMMatrixPerspectiveFovLH(DX::XM_PI / 3.4f, (float)mClientRect.right / (float)mClientRect.bottom, NEAR_CLIP, FAR_CLIP);
	DX::XMMATRIX viewProj = view * proj;
	DX::XMMATRIX invViewProj = DX::XMMatrixInverse(NULL, viewProj);

	DX::XMStoreFloat4x4(&mCamInvViewProj, invViewProj);
}

void CWorldState::UpdateHeldStructure()
{
	// Check if a structure is being held
	if (mpPlacingStructure)
	{		
		mpPlacingStructure->SetWorldPos(mpCurTile->GetWorldPos());

		// Determine current grid
		switch (mMouseState)
		{
		case MS_EARTH_GRID:
			// When placing structure, check if any used tiles
			if (mpEarthGrid->TurnOnTiles(mpCurTile, mpPlacingStructure->GetBLPosition(), mpPlacingStructure->GetTRPosition(), mpPlacingStructure->GetGridSpawnLocation())
				|| mpPlacingStructure->GetBuildCost() > mpHumanPlayer->GetMineralAmount())
			{
				mpPlacingStructure->SetBadTexture();
			}
			else
			{
				mpPlacingStructure->SetGoodTexture();
			}
			break;

		case MS_MARS_GRID:
			// Cannot build on Mars
			mpPlacingStructure->SetBadTexture();
			break;
		}
	}
}

void CWorldState::CalculateMouseGridPos()
{
	// Convert mouse co-ordinates to have a -1 to 1 range
	DX::XMFLOAT2 mousePoint(((2.0f * (float)mpMouseScreenPos->mPosX) / (float)mClientRect.right) - 1.0f,
		(((2.0f * (float)mpMouseScreenPos->mPosY) / (float)mClientRect.bottom) - 1.0f) * -1.0f);

	// Mouse vectors
	DX::XMFLOAT3 nearVec(mousePoint.x, mousePoint.y, 0.0f);
	DX::XMFLOAT3 farVec(mousePoint.x, mousePoint.y, 1.0f);

	// Determine ray points
	DX::XMVECTOR rayOrigin = DX::XMVector3TransformCoord(DX::XMLoadFloat3(&nearVec), DX::XMLoadFloat4x4(&mCamInvViewProj));
	DX::XMVECTOR rayEnd = DX::XMVector3TransformCoord(DX::XMLoadFloat3(&farVec), DX::XMLoadFloat4x4(&mCamInvViewProj));
	
	DX::XMFLOAT3 end;
	DX::XMFLOAT3 dir;

	DX::XMStoreFloat3(&end, rayEnd);
	DX::XMStoreFloat3(&mMouseOrigin, rayOrigin);

	dir.x = end.x - mMouseOrigin.x;
	dir.y = end.y - mMouseOrigin.y;
	dir.z = end.z - mMouseOrigin.z;

	// Normalise to determine final direction vector
	DX::XMVECTOR normalised = DX::XMVector3Normalize(DX::XMLoadFloat3(&dir));
	DX::XMStoreFloat3(&mMouseDirection, normalised);


	DX::XMVECTOR intersecVec = DX::XMPlaneIntersectLine(DX::XMLoadFloat3(&mYPlane), rayOrigin, rayEnd);
	DX::XMStoreFloat3(&mMouseWorldPos, intersecVec);		// Store the mouse's world position at y = 0

	// Use the mouse's world position to determine the grid position
	mMouseGridPos.mPosX = (int)((mMouseWorldPos.x - mCurGridPos.x) / GRID_TILE_SIZE);
	mMouseGridPos.mPosY = (int)((mMouseWorldPos.z - mCurGridPos.z) / GRID_TILE_SIZE);
}

void CWorldState::DrawFontData()
{
	// Draw Mouse world co-ordinates to screen
	strStream << "X: " << mMouseWorldPos.x << "  Z: " << mMouseWorldPos.z;
	mFntDebug->Draw(strStream.str(), 1580, 615, kWhite, kRight, kTop);
	strStream.str("");

	// Draw mouse grid co-ordinates
	strStream << "X: " << mMouseGridPos.mPosX << "  Y: " << mMouseGridPos.mPosY;
	mFntDebug->Draw(strStream.str(), 1580, 630, kWhite, kRight, kTop);
	strStream.str("");

	// Draw mouse grid co-ordinates
	strStream << "Current pop " << mpHumanPlayer->GetCurrentPop() << "/" << mpHumanPlayer->GetCurrentPopLimit();
	mFntDebug->Draw(strStream.str(), 1580, 600, kWhite, kRight, kTop);
	strStream.str("");


	//////////////temp
	//Current Tactics
	strStream << "Current Tactic: " << mpHumanPlayer->GetFleet()->GetTacticsName();
	mFntDebug->Draw(strStream.str(), 1225, 0, kWhite, kLeft, kTop);
	strStream.str("");

	// Minerals
	strStream << "Minerals: " << mpHumanPlayer->GetMineralAmount();
	mFntDebug->Draw(strStream.str(), 1225, 15, kWhite, kLeft, kTop);
	strStream.str("");


	// Draw mouse state - and get current grid data
	strStream << "GRID: ";
	switch (mMouseState)
	{
	case MS_EARTH_GRID:
		mpCurTile = mpEarthGrid->GetTileData(mMouseGridPos);
		strStream << "Earth";
		break;
	case MS_MARS_GRID:
		strStream << "Minerals: " << mpAIPlayer->GetMineralAmount();
		mFntDebug->Draw(strStream.str(), 1225, 30, kWhite, kLeft, kTop);
		strStream.str("");

		mpCurTile = mpMarsGrid->GetTileData(mMouseGridPos);
		strStream << "Mars";
		break;
	case MS_NO_AREA:
	case MS_EARTH_EDGE:
	case MS_MARS_EDGE:
		mpCurTile = mpNullTile;
		strStream << "None";
		break;
	case MS_UI:
		mpCurTile = mpNullTile;
		strStream << "UI";
		break;
	}

	if (mpCurTile)
	{
		strStream << "  USED: " << mpCurTile->IsTileUsed();
		mFntDebug->Draw(strStream.str(), 1580, 645, kWhite, kRight, kTop);
		strStream.str("");
	}
}

EMouseStates CWorldState::UpdateMouseState()
{
	// Check if on UI
	if (mpMouseScreenPos->mPosY > 750)
	{
		return MS_UI;
	}
	
	// Check whether it is within earth boundary
	if (mMouseWorldPos.x > mpEarthGrid->GetGridStartPos().x && mMouseWorldPos.x < mpEarthGrid->GetGridEndPos().x
		&& mMouseWorldPos.z > mpEarthGrid->GetGridStartPos().z && mMouseWorldPos.z < mpEarthGrid->GetGridEndPos().z)
	{
		mCurGridPos = mpEarthGrid->GetGridStartPos();		// Bottom left position of grid
		return MS_EARTH_GRID;
	}
	
	// Check if in mars boundary
	if (mMouseWorldPos.x > mpMarsGrid->GetGridStartPos().x && mMouseWorldPos.x < mpMarsGrid->GetGridEndPos().x
		&& mMouseWorldPos.z > mpMarsGrid->GetGridStartPos().z && mMouseWorldPos.z < mpMarsGrid->GetGridEndPos().z)
	{
		mCurGridPos = mpMarsGrid->GetGridStartPos();		// Bottom left position of grid
		return MS_MARS_GRID;
	}

	// Check if in earth boundary, but outside of the actual grid area
	if (mMouseWorldPos.x > mpEarthGrid->GetGridStartPos().x - 1000.0f && mMouseWorldPos.x < mpEarthGrid->GetGridEndPos().x + 1000.0f
		&& mMouseWorldPos.z > mpEarthGrid->GetGridStartPos().z - 1000.0f && mMouseWorldPos.z < mpEarthGrid->GetGridEndPos().z + 1000.0f)
	{
		mCurGridPos = mpEarthGrid->GetGridStartPos();		// Bottom left position of grid
		return MS_EARTH_EDGE;
	}

	// Check if in mars boundary, but outside of the actual grid area
	if (mMouseWorldPos.x > mpMarsGrid->GetGridStartPos().x - 1000.0f && mMouseWorldPos.x < mpMarsGrid->GetGridEndPos().x + 1000.0f
		&& mMouseWorldPos.z > mpMarsGrid->GetGridStartPos().z - 1000.0f && mMouseWorldPos.z < mpMarsGrid->GetGridEndPos().z + 1000.0f)
	{
		mCurGridPos = mpMarsGrid->GetGridStartPos();		// Bottom left position of grid
		return MS_MARS_EDGE;
	}

	// Must be outside of all grid and edge areas
	return MS_NO_AREA;
}

void CWorldState::CheckKeyPresses()
{
	// CHECK FOR SCROLLING
	//------------------------------
	gMouseWheelDelta = gpEngine->GetMouseWheelMovement() * -1.0f;
	if (gMouseWheelDelta != 0)
	{
		float moveAmount = gFrameTime * CAM_SCROLL_SPEED * gMouseWheelDelta;
		mpCamCurrent->AdjustRho(moveAmount);
		gMouseWheelDelta = 0;
	}
	

	// CLICKING
	//------------------------------
	if (gpEngine->KeyHit(Mouse_RButton))
	{
		mRMouseClicked = true;
	}

	// Left Click = place currently selected building
	if (mLMouseClicked)
	{
		// Check if placing a structure
		if (mpPlacingStructure)
		{
			switch (mMouseState)
			{
				case MS_EARTH_GRID:
				{
					// Place the structure - check if successful
					EErrorTypes error = mpHumanPlayer->PurchaseStructure(mpPlacingStructure, mpEarthGrid, mpCurTile);
					if (error == ERR_NONE)
					{
						// Safe to point at nothing due to structure pointer passed on to Player's data
						mpPlacingStructure = nullptr;
						mpEarthGrid->ResetTilesModels();
						mLMouseClicked = false;
					}
					else
					{
						// Alert player of error
						switch (error)
						{
						case ERR_NO_MINERALS:
							gpNewsTicker->AddNewElement("Insufficient minerals for this structure!", true);
							break;
						case ERR_NO_SPACE:
							gpNewsTicker->AddNewElement("You cannot build here! Something might be in the way.", true);
							break;
						}
					}
					break;
				}
				case MS_MARS_GRID:
					// Cannot build on mars - return error
					gpNewsTicker->AddNewElement("You can only build on your own planet!", true);
					break;
			}
		}
		else
		{
			// track the distance between each selection function call to determine the absolute closest unit/structure
			float tmpDistance = 9999.0f;
			
			// Not placing a structure - find out where they are clicking
			CStructure* pNewSelectedStructure = nullptr;
			CGameAgent* pNewSelectedAgent = nullptr;
			CMinerals* pNewSelectedMineral = nullptr;

			switch (mMouseState)
			{
			case MS_NO_AREA:
			case MS_EARTH_EDGE:
			case MS_EARTH_GRID:
			case MS_MARS_EDGE:
			case MS_MARS_GRID:
				mpHumanPlayer->CheckGameObjectSelection(pNewSelectedStructure, pNewSelectedAgent,
					pNewSelectedMineral, mMouseOrigin, mMouseDirection, tmpDistance, false);
				OnStructureSelectChange(pNewSelectedStructure);
				OnUnitSelectChange(pNewSelectedAgent, false);
				mpUnitSelectionList.clear();
				break;

			case MS_UI:
				OnStructureSelectChange(pNewSelectedStructure);
				OnUnitSelectChange(pNewSelectedAgent, false);
				break;
			}

			if (pNewSelectedStructure || pNewSelectedAgent)
			{
				mLMouseClicked = false;
			}
		}
	}

	if (mRMouseClicked)
	{
		if (mpCurSelectedAgent)
		{
			CStructure* pTargetStructure = nullptr;
			CGameAgent* pTargetGameAgent = nullptr;
			CMinerals* pTargetMinerals = nullptr;

			// If the current unit is a worker, ensure it is no longer targeting any minerals
			if (mpCurSelectedAgent->GetAgentData()->mAgentType == GAV_WORKER)
			{
				// Cast into a worker pointer to access methods
				CWorker* pWorker = static_cast<CWorker*>(mpCurSelectedAgent);
				pTargetMinerals = pWorker->GetMineral();
				if (pTargetMinerals) pTargetMinerals->SetUsage(false);
				pTargetMinerals = nullptr;
				pWorker->SetMineral(nullptr);
			}

			// track the distance between each selection function call to determine the absolute closest unit/structure
			float tmpDistance = 9999.0f;

			// Check the position of the mouse
			switch (mMouseState)
			{
			case MS_NO_AREA:
			case MS_EARTH_EDGE:
			case MS_EARTH_GRID:
			case MS_MARS_EDGE:
			case MS_MARS_GRID:
				mpHumanPlayer->CheckGameObjectSelection(pTargetStructure, pTargetGameAgent, pTargetMinerals, mMouseOrigin, mMouseDirection, tmpDistance, true);
				mpAIPlayer->CheckGameObjectSelection(pTargetStructure, pTargetGameAgent, pTargetMinerals, mMouseOrigin, mMouseDirection, tmpDistance, true);
				mpPlayerManager->CheckRebelSelection(pTargetStructure, pTargetGameAgent, pTargetMinerals, mMouseOrigin, mMouseDirection, tmpDistance);
				break;

			case MS_UI:
				break;
			}

			if (pTargetStructure != nullptr)
			{
				if (pTargetStructure->GetFaction() == FAC_EARTH_DEFENSE_FORCE)
				{
					mpCurSelectedAgent->SetAttackTarget(pTargetStructure);
				}
			}
			else if (pTargetGameAgent != nullptr)
			{
				if (pTargetGameAgent->GetFaction() != FAC_EARTH_DEFENSE_FORCE)
				{
					mpCurSelectedAgent->SetAttackTarget(pTargetGameAgent);
				}
			}
			else if (pTargetMinerals)
			{
				// Check if the currently selected unit is a worker
				if (mpCurSelectedAgent->GetAgentData()->mAgentType == GAV_WORKER)
				{
					// Check if the mineral is in use
					if (pTargetMinerals->IsBeingUsed())
					{
						// Alert the user that the minerals are already being harvested
						gpNewsTicker->AddNewElement("Minerals already being harvested by another worker!", true);
					}
					else
					{
						// Cast into a worker pointer to access methods
						CWorker* pWorker = static_cast<CWorker*>(mpCurSelectedAgent);
						// Send the worker unit to the resource pile position
						pWorker->SetPathTarget(pTargetMinerals->GetWorldPos());
						// Set the target mineral to be used and store in worker
						pWorker->SetMineral(pTargetMinerals);
						pTargetMinerals->SetUsage(true);
						// Let the user know the worker is going to go mine
						gpNewsTicker->AddNewElement("Worker unit going to mine.", false);
					}
				}
				else
				{
					// Show an error message saying only worker units can interact with resource piles
					gpNewsTicker->AddNewElement("Only worker units can interact with mineral deposits!", true);
				}
			}
			else if (mpCurTile == mpNullTile || !mpCurTile)
			{
				mpCurSelectedAgent->SetPathTarget(mMouseWorldPos);
			}
			else if (!mpCurTile->IsTileUsed())
			{
				mpCurSelectedAgent->SetPathTarget(mMouseWorldPos);
			}

			mRMouseClicked = false;
		}
		else if (mpUnitSelectionList.size() > 0)
		{
			// track the distance between each selection function call to determine the absolute closest unit/structure
			float tmpDistance = 9999.0f;
			
			// Update all the units in the list to the current path position if the tile is not in use
			CStructure* pTargetStructure = nullptr;
			CGameAgent* pTargetGameAgent = nullptr;
			CMinerals* pTargetMinerals = nullptr;

			// Check the position of the mouse
			switch (mMouseState)
			{
			case MS_NO_AREA:
			case MS_EARTH_EDGE:
			case MS_EARTH_GRID:
				mpHumanPlayer->CheckGameObjectSelection(pTargetStructure, pTargetGameAgent, pTargetMinerals, mMouseOrigin, mMouseDirection, tmpDistance, true);
				break;

			case MS_MARS_EDGE:
			case MS_MARS_GRID:
				mpAIPlayer->CheckGameObjectSelection(pTargetStructure, pTargetGameAgent, pTargetMinerals, mMouseOrigin, mMouseDirection, tmpDistance, true);
				break;

			case MS_UI:
				break;
			}

			if (pTargetStructure != nullptr)
			{
				if (pTargetStructure->GetFaction() == FAC_EARTH_DEFENSE_FORCE)
				{
					for (miterUnitSelectionList = mpUnitSelectionList.begin(); miterUnitSelectionList != mpUnitSelectionList.end(); miterUnitSelectionList++)
					{
						(*miterUnitSelectionList)->SetAttackTarget(pTargetStructure);
					}
				}
			}
			else if (pTargetGameAgent != nullptr)
			{
				if (pTargetGameAgent->GetFaction() != FAC_EARTH_DEFENSE_FORCE)
				{
					for (miterUnitSelectionList = mpUnitSelectionList.begin(); miterUnitSelectionList != mpUnitSelectionList.end(); miterUnitSelectionList++)
					{
						(*miterUnitSelectionList)->SetAttackTarget(pTargetGameAgent);
					}
				}
			}
			else if (pTargetMinerals)
			{
				// Check to see if there is just one unit in the selection & check that single unit is a worker unit
				if (mpUnitSelectionList.size() == 1 && mpUnitSelectionList.front()->GetAgentData()->mAgentType == GAV_WORKER)
				{
					// Set the minerals target to null
					// Cast into a worker pointer to access methods
					CWorker* pWorker = static_cast<CWorker*>(mpUnitSelectionList.front());
					CMinerals* pMinerals = pWorker->GetMineral();
					if (pMinerals) pMinerals->SetUsage(false);
					pMinerals = nullptr;
					pWorker->SetMineral(nullptr);
						
					// Send the worker to go mine
					// Check if the mineral is in use
					if (pTargetMinerals->IsBeingUsed())
					{
						// Alert the user that the minerals are already being harvested
						gpNewsTicker->AddNewElement("Minerals already being harvested by another worker!", true);
					}
					else
					{
						// Send the worker unit to the resource pile position
						pWorker->SetPathTarget(pTargetMinerals->GetWorldPos());
						// Set the target mineral to be used and store in worker
						pWorker->SetMineral(pTargetMinerals);
						pTargetMinerals->SetUsage(true);
						// Let the user know the worker is going to go mine
						gpNewsTicker->AddNewElement("Worker unit going to mine.", false);
					}
				}
				else
				{
					// Show an error as you can only send one worker unit to a resource
					gpNewsTicker->AddNewElement("A mineral deposit can only have a single unit!", true);
				}
			}
			else if (mpCurTile == mpNullTile || !mpCurTile)
			{
				for (miterUnitSelectionList = mpUnitSelectionList.begin(); miterUnitSelectionList != mpUnitSelectionList.end(); miterUnitSelectionList++)
				{
					(*miterUnitSelectionList)->SetPathTarget(mMouseWorldPos);
				}
				mRMouseClicked = false;
			}
			else if (!mpCurTile->IsTileUsed())
			{
				for (miterUnitSelectionList = mpUnitSelectionList.begin(); miterUnitSelectionList != mpUnitSelectionList.end(); miterUnitSelectionList++)
				{
					(*miterUnitSelectionList)->SetPathTarget(mMouseWorldPos);
				}
				mRMouseClicked = false;
			}

			mRMouseClicked = false;
		}
	}

	// If the flag for the lest mouse button is still raised
	if (mLMouseClicked)
	{
		mLMouseClicked = false;
		OnUnitSelectChange(nullptr, false);
	}

	// Check if a building is currently selected
	if (!mpCurSelectedStructure && !mpCurSelectedAgent)
	{
		// BUILDING PLACEMENT
		//------------------------------
		CStructure* pStructure;

		// C = no building is selected
		if (mRMouseClicked)
		{
			pStructure = nullptr;
			OnPlacingStructureChange(pStructure);
			mRMouseClicked = false;
		}

		// 1 = Barracks
		if (gpEngine->KeyHit(Key_1))
		{
			pStructure = new CBarracks();
			OnPlacingStructureChange(pStructure);
		}

		// 2 = Hellipad
		if (gpEngine->KeyHit(Key_2))
		{
			pStructure = new CHellipad();
			OnPlacingStructureChange(pStructure);
		}

		// 3 = Space Centre
		if (gpEngine->KeyHit(Key_3))
		{
			pStructure = new CSpaceCentre();
			OnPlacingStructureChange(pStructure);
		}
	}


	// UNIT CONSTRUCTION
	//------------------------------
	// C = deselect current building
	if (gpEngine->KeyHit(Key_C))
	{
		OnPlacingStructureChange(nullptr);
	}


	// CAMERA CHANGE
	//------------------------------
	if (gpEngine->KeyHit(Key_Return))
	{
		if (mpCamCurrent == mpCamEarth)
		{
			mpCamCurrent = mpCamMars;
		}
		else
		{
			mpCamCurrent = mpCamEarth;
		}

		// Ensure global pointer holds correct camera
		gpCurWorldCamera = mpCamCurrent->GetCamera();

		// Ensure no buildings can be brought over
		OnPlacingStructureChange(nullptr);
		OnUnitSelectChange(nullptr, false);
		OnStructureSelectChange(nullptr);
	}


	// SHORTCUT FUNCTIONS FOR TACTICS
	//--------------------------------
	//temporary, just used to test the transfering of ships to the fleet
	if (gpEngine->KeyHit(Key_L))
	{
		mpHumanPlayer->LaunchAttack();
		mpAIPlayer->LaunchAttack();
		gCurState = GS_SPACE;
	}

	//-- DEBUG CONTROLS --//
	//used to make the ai fleet win in testing
	if (gpEngine->KeyHit(Key_J))
	{
		mpAIPlayer->GetFleet()->SetTactic(Targeted);
	}
	//add units to both players
	if (gpEngine->KeyHit(Key_P))
	{
		mpHumanPlayer->AddShips();
		mpAIPlayer->AddShips();
	}

	mRMouseClicked = false;
}

void CWorldState::DisplaySelectedBuildingInfo()
{
	int newProgressAmount = 0;
	int newHealthAmount = -5;
	
	// Check if the building has been destoryed
	if (mpCurSelectedStructure)
	{
		if (mpCurSelectedStructure->GetHealth() <= 0.0f)
		{
			// Select nothing
			OnStructureSelectChange(nullptr);
		}
	}

	// If an object is selected, display its info
	if (mpCurSelectedStructure)
	{
		// BUILDING DESTRUCTION
		//------------------------------
		if (gpEngine->KeyHit(Key_Delete))
		{
			DeleteSelection();
			return;
		}
		
		mpCurSelectedStructure->DisplayInfo(mFntDebug);
		
		// If there is a queue, calculate the progress
		if (mpCurSelectedStructure->GetQueueSize() != 0)
		{
			// There is a queue - calculate progress of first unit in queue
			float timeLeft = mpCurSelectedStructure->GetQueue()->front()->GetCurProductionTimeLeft();
			float totalTime = mpCurSelectedStructure->GetQueue()->front()->GetProductionTime();

			// Calculate new progress using integer devision
			newProgressAmount = static_cast<int>(((totalTime - timeLeft) / totalTime) * 10.0f);
		}
		else
		{
			// There is no queue - ensure newProgressAmount is 0
			newProgressAmount = 0;
		}

		// Calculate health/construction progress percentage
		if (mpCurSelectedStructure->GetState() == OBJ_CONSTRUCTING)
		{
			// When constructing, health is based upon construction percentage
			// Calculate construction percentage
			float timeLeft = mpCurSelectedStructure->GetBuildTimeLeft();
			float maxTime = mpCurSelectedStructure->GetBuildTime();
			float timeTaken = maxTime - timeLeft;
			int percentage = (int)((timeTaken / maxTime) * 100.0f);

			// Check if the completion percentage is a multiple of 5
			if (percentage % 5 == 0)
			{
				// Percentage is a multiple of 5 - use its value as the new health bar amount
				newHealthAmount = percentage;
			}
			else
			{
				// maintain previous value
				newHealthAmount = mPrevHealth;
			}

			// Draw amount to screen
			strStream << percentage << "%";
			mFntDebug->Draw(strStream.str(), 990, 825, 0xff002930, kRight, kTop);
			strStream.str(""); 
		}
		else
		{
			// Calculate construction percentage
			float healthLeft = mpCurSelectedStructure->GetHealth();
			float maxHealth = mpCurSelectedStructure->GetMaxHealth();
			int percentage = (int)((healthLeft / maxHealth) * 100.0f);

			// Check if the completion percentage is a multiple of 5
			if (percentage % 5 == 0)
			{
				// Percentage is a multiple of 5 - use its value as the new health bar amount
				newHealthAmount = percentage;
			}
			else
			{
				// maintain previous value
				newHealthAmount = mPrevHealth;
			}

			// Draw amount to screen
			strStream << healthLeft << " / " << maxHealth;
			mFntDebug->Draw(strStream.str(), 990, 825, 0xff002930, kRight, kTop);
			strStream.str("");
		}
	}
	else
	{
		// Nothing is selected - ensure newProgressAmount is 0
		newProgressAmount = 0;

		// If there is also no unit or resource selected, set new health amount to default
		if (mpCurSelectedAgent || mpUnitSelectionList.size() != 0)
		{
			// If there is a selection, maintain current value
			newHealthAmount = mPrevHealth;
		}
		else
		{
			newHealthAmount = -5;
		}
	}

	// Check for a change in progress amount
	if (newProgressAmount != mQueuePrevProg)
	{
		// there has been a change - update the stored variable
		mQueuePrevProg = newProgressAmount;
		// Call change event function
		OnStructureQueueProgressChange();
	}

	// Check new health amount
	if (newHealthAmount != mPrevHealth)
	{
		mPrevHealth = newHealthAmount;
		OnItemHealthChange();
	}
}

void CWorldState::DisplaySelectedAgentInfo()
{
	int newHealthAmount = -5;
	
	// Check if the building has been destoryed
	if (mpCurSelectedAgent)
	{
		if (mpCurSelectedAgent->GetHealth() <= 0.0f)
		{
			// Select nothing
			OnUnitSelectChange(nullptr, false);
		}
	}

	// If an object is selected, display its info
	if (mpCurSelectedAgent)
	{
		// UNIT DESTRUCTION
		//------------------------------
		if (gpEngine->KeyHit(Key_Delete))
		{
			DeleteSelection();
			return;
		}

		mpCurSelectedAgent->DisplayInfo(mFntDebug);

		// Calculate construction percentage
		float healthLeft = mpCurSelectedAgent->GetHealth();
		float maxHealth = mpCurSelectedAgent->GetMaxHealth();
		int percentage = (int)((healthLeft / maxHealth) * 100.0f);

		// Check if the completion percentage is a multiple of 5
		if (percentage % 5 == 0)
		{
			// Percentage is a multiple of 5 - use its value as the new health bar amount
			newHealthAmount = percentage;
		}
		else
		{
			// maintain previous value
			newHealthAmount = mPrevHealth;
		}
	}
	// Check if there is a group of units selected
	else if (mpUnitSelectionList.size() != 0)
	{
		// UNIT DESTRUCTION
		//------------------------------
		if (gpEngine->KeyHit(Key_Delete))
		{
			DeleteSelection();
			return;
		}

		// Get the front unit
		CGameAgent* pFrontSelection = mpUnitSelectionList.front();

		// Display the info of the front unit
		pFrontSelection->DisplayInfo(mFntDebug);

		// Calculate construction percentage
		float healthLeft = pFrontSelection->GetHealth();
		float maxHealth = pFrontSelection->GetMaxHealth();
		int percentage = (int)((healthLeft / maxHealth) * 100.0f);

		// Check if the completion percentage is a multiple of 5
		if (percentage % 5 == 0)
		{
			// Percentage is a multiple of 5 - use its value as the new health bar amount
			newHealthAmount = percentage;
		}
		else
		{
			// maintain previous value
			newHealthAmount = mPrevHealth;
		}
	}
	else
	{
		// If there is also no building or resource selected, set new health amount to default
		if (mpCurSelectedStructure)
		{
			// If there is a selection, maintain current value
			newHealthAmount = mPrevHealth;
		}
		else
		{
			newHealthAmount = -5;
		}
	}

	// Check new health amount
	if (newHealthAmount != mPrevHealth)
	{
		mPrevHealth = newHealthAmount;
		OnItemHealthChange();
	}
}


//-----------------------------------------------------
// WORLD STATE CLASS OVERRIDE METHODS
//-----------------------------------------------------
void CWorldState::StateSetup()
{
	// INITIALISE ADDITIONAL VARIABLES
	//------------------------------
	mpCurSelectedAgent = nullptr;
	mpCurSelectedStructure = nullptr;

	mQueuePrevSize = 0;

	mpMouseScreenPos = new SPointData();
	mWindowClip = { 0 };
	GetClipCursor(&mBaseClip);
	GetWindowRect((HWND)gpEngine->GetWindow(), &mWindowClip);
	GetClientRect((HWND)gpEngine->GetWindow(), &mClientRect);

	// Calculate how much of the window is made up of bars (i.e. window bar & side/bottom adjustment bars)
	// WIDTH - find difference in width between client rectangle and window rectangle
	int difference = mWindowClip.right - mClientRect.right;
	// Small bars are same size on either side of the window - so simply divide difference by 2
	int smallBarSize = difference / 2;

	// HEIGHT
	difference = mWindowClip.bottom - mClientRect.bottom;
	// Window bar at top is a different size to the smaller bar
	// Use the small bar size to find out the size of the large bar at the top
	int largeBarSize = difference - smallBarSize;

	// Shrink the rectangle to not include side bars and window bar
	mWindowClip.top += largeBarSize;
	mWindowClip.left += smallBarSize;
	mWindowClip.right -= smallBarSize;
	mWindowClip.bottom -= smallBarSize;

	// Set the cursor's limits
	ClipCursor(&mWindowClip);
	mLMouseClicked = false;
	mRMouseClicked = false;
	mLMouseHeld = false;
	mHoldCount = 0.0f;
	mClickCoolDown = 0.1f;

	mpDragBox = nullptr;
	mpMdlDragBox = nullptr;


	// Load scenery meshes
	mpTreeMeshes[0] = gpEngine->LoadMesh("Tree1.x");
	mpTreeMeshes[1] = gpEngine->LoadMesh("Tree2.x");

	mpRockMeshes[0] = gpEngine->LoadMesh("rock_001_LOD1.x");
	mpRockMeshes[1] = gpEngine->LoadMesh("rock_002_LOD1.x");
	mpRockMeshes[2] = gpEngine->LoadMesh("rock_003_LOD1.x");
	mpRockMeshes[3] = gpEngine->LoadMesh("rock_004_LOD1.x");
	mpRockMeshes[4] = gpEngine->LoadMesh("rock_005_LOD1.x");
	mpRockMeshes[5] = gpEngine->LoadMesh("rock_006_LOD1.x");
	mpRockMeshes[6] = gpEngine->LoadMesh("rock_007_LOD1.x");
	mpRockMeshes[7] = gpEngine->LoadMesh("rock_008_LOD1.x");
	mpRockMeshes[8] = gpEngine->LoadMesh("rock_009_LOD1.x");
	mpRockMeshes[9] = gpEngine->LoadMesh("rock_010_LOD1.x");
	mpRockMeshes[10] = gpEngine->LoadMesh("rock_011_LOD1.x");

	mpPlayerManager = CStateControl::GetInstance()->GetPlayerManager();


	// CREATE Y = 0 PLANE
	//-----------------------------
	DX::XMFLOAT3 point(-10.0f, 0.0f, 50.0f);
	DX::XMVECTOR point1 = DX::XMLoadFloat3(&point);
	point = DX::XMFLOAT3(48.0f, 0.0f, 12.0f);
	DX::XMVECTOR point2 = DX::XMLoadFloat3(&point);
	point = DX::XMFLOAT3(26.0f, 0.0f, -30.0f);
	DX::XMVECTOR point3 = DX::XMLoadFloat3(&point);

	// Calculate plane coefficient
	DX::XMVECTOR planeVec = DX::XMPlaneFromPoints(point1, point2, point3);
	DX::XMStoreFloat3(&mYPlane, planeVec);


	// INITIALISE SKYBOX
	//-----------------------------
	mpMshSkybox = gpEngine->LoadMesh("SkyboxWorld.x");
	mpMdlSkybox = mpMshSkybox->CreateModel(0.0f, -1.0f, 0.0f);
	mpMdlSkybox->Scale(4.0f);


	// INITIALISE NULL TILE
	//-----------------------------
	mpNullTile = new CTile();
	mpNullTile->SetWorldPos(DX::XMFLOAT3(-5000.0f, 0.0f, 0.0f));
	mpNullTile->SetTileUsage(true);


	// INITIALISE USER INTERFACE
	//-----------------------------
	mFntDebug = gpEngine->LoadFont("Calibri", 20U);
	mpBarBottom = gpEngine->CreateSprite("UIBarBottom.png", 150.0f, 800.0f, 0.9f);
	mpSprCursor = gpEngine->CreateSprite("BaseCursor.png", 5.0f, 50.0f, 0.0f);
	CAdvancedButton<CWorldState, void>* pNewButton = nullptr;

	pNewButton = new CAdvancedButton<CWorldState, void>("DefBarracksButton2.png", "SelBarracksButton2.png", SPointData(1050, 765),
		DX::XMFLOAT2(90.0f, 90.0f), *this, &CWorldState::CreateBarracks, TR_UP, true, 0.2f);
	mpButtonBarracks = pNewButton;
	mpGenericButtonList.push_back(pNewButton);

	pNewButton = new CAdvancedButton<CWorldState, void>("DefHellipadButton2.png", "SelHellipadButton2.png", SPointData(1145, 765),
		DX::XMFLOAT2(90.0f, 90.0f), *this, &CWorldState::CreateHellipad, TR_UP, true, 0.2f);
	mpButtonHellipad = pNewButton;
	mpGenericButtonList.push_back(pNewButton);

	pNewButton = new CAdvancedButton<CWorldState, void>("DefSpaceCentreButton2.png", "SelSpaceCentreButton2.png", SPointData(1240, 765),
		DX::XMFLOAT2(90.0f, 90.0f), *this, &CWorldState::CreateSpaceCentre, TR_UP, true, 0.2f);
	mpButtonSpaceCentre = pNewButton;
	mpGenericButtonList.push_back(pNewButton);

	pNewButton = new CAdvancedButton<CWorldState, void>("DefDeleteButton.png", "SelDeleteButton.png", SPointData(450, 765),
		DX::XMFLOAT2(90.0f, 90.0f), *this, &CWorldState::DeleteSelection, TR_UP, false, 0.2f);
	pNewButton->Hide();
	mpButtonDelete = pNewButton;
	mpGenericButtonList.push_back(pNewButton);

	mpButtonPutUnitIntoSpace = new CAdvancedButton<CWorldState, void>("DefBeamUpButton.png", "SelBeamUpButton.png",
		SPointData(260, 765), DX::XMFLOAT2(90.0f, 90.0f), *this, &CWorldState::PutUnitIntoSpace, TR_UP, false, 0.2f);
	mpGenericButtonList.push_back(mpButtonPutUnitIntoSpace);

	// Barracks units buttons
	mpBarracksButtons = new SStructureButtons<CWorldState>(3);
	mpBarracksButtons->mpButtons[0] = new CAdvancedButton<CWorldState, void, int>("DefInfantryButton2.png", "SelInfantryButton2.png", SPointData(1050, 765),
		DX::XMFLOAT2(90.0f, 90.0f), *this, &CWorldState::QueueUnit, TR_UP, false, 0.2f);
	mpBarracksButtons->mpButtons[1] = new CAdvancedButton<CWorldState, void, int>("DefArtilleryButton2.png", "SelArtilleryButton2.png", SPointData(1145, 765),
		DX::XMFLOAT2(90.0f, 90.0f), *this, &CWorldState::QueueUnit, TR_UP, false, 0.2f);
	mpBarracksButtons->mpButtons[2] = new CAdvancedButton<CWorldState, void, int>("DefTankButton2.png", "SelTankButton2.png", SPointData(1240, 765),
		DX::XMFLOAT2(90.0f, 90.0f), *this, &CWorldState::QueueUnit, TR_UP, false, 0.2f);
	mpBarracksButtons->Hide();

	for (int i = 0; i < mpBarracksButtons->mNumButtons; i++)
	{
		mpUnitsButtonList.push_back(mpBarracksButtons->mpButtons[i]);
	}
	
	// hellipad units buttons
	mpHellipadButtons = new SStructureButtons<CWorldState>(2);
	mpHellipadButtons->mpButtons[0] = new CAdvancedButton<CWorldState, void, int>("DefFighterButton2.png", "SelFighterButton2.png", SPointData(1050, 765),
		DX::XMFLOAT2(90.0f, 90.0f), *this, &CWorldState::QueueUnit, TR_UP, false, 0.2f);
	mpHellipadButtons->mpButtons[1] = new CAdvancedButton<CWorldState, void, int>("DefBomberButton2.png", "SelBomberButton2.png", SPointData(1145, 765),
		DX::XMFLOAT2(90.0f, 90.0f), *this, &CWorldState::QueueUnit, TR_UP, false, 0.2f);

	for (int i = 0; i < mpHellipadButtons->mNumButtons; i++)
	{
		mpUnitsButtonList.push_back(mpHellipadButtons->mpButtons[i]);
	}

	// Space centre units buttons
	mpSpaceCentreButtons = new SStructureButtons<CWorldState>(3);
	mpSpaceCentreButtons->mpButtons[0] = new CAdvancedButton<CWorldState, void, int>("DefSpaceFighterButton2.png", "SelSpaceFighterButton2.png", SPointData(1050, 765),
		DX::XMFLOAT2(90.0f, 90.0f), *this, &CWorldState::QueueUnit, TR_UP, false, 0.2f);
	mpSpaceCentreButtons->mpButtons[1] = new CAdvancedButton<CWorldState, void, int>("DefTransportButton2.png", "SelTransportButton2.png", SPointData(1145, 765),
		DX::XMFLOAT2(90.0f, 90.0f), *this, &CWorldState::QueueUnit, TR_UP, false, 0.2f);
	mpSpaceCentreButtons->mpButtons[2] = new CAdvancedButton<CWorldState, void, int>("DefMothershipButton2.png", "SelMothershipButton2.png", SPointData(1240, 765),
		DX::XMFLOAT2(90.0f, 90.0f), *this, &CWorldState::QueueUnit, TR_UP, false, 0.2f);
	mpSpaceCentreButtons->Hide();

	for (int i = 0; i < mpSpaceCentreButtons->mNumButtons; i++)
	{
		mpUnitsButtonList.push_back(mpSpaceCentreButtons->mpButtons[i]);
	}

	// Command centre units buttons
	mpComCentreButtons = new SStructureButtons<CWorldState>(1);
	mpComCentreButtons->mpButtons[0] = new CAdvancedButton<CWorldState, void, int>("DefWorkerButton2.png", "SelWorkerButton2.png", SPointData(1050, 765),
		DX::XMFLOAT2(90.0f, 90.0f), *this, &CWorldState::QueueUnit, TR_UP, false, 0.2f);
	mpComCentreButtons->Hide();

	for (int i = 0; i < mpComCentreButtons->mNumButtons; i++)
	{
		mpUnitsButtonList.push_back(mpComCentreButtons->mpButtons[i]);
	}


	// Queue buttons
	mpQueueButtons = new SStructureButtons<CWorldState>(5);
	mpQueueButtons->mpButtons[0] = new CAdvancedButton<CWorldState, void, int>(SPointData(5, 5), DX::XMFLOAT2(90.0f, 90.0f),
		*this, &CWorldState::UnqueueUnit, TR_DOWN, true, 0.2f);
	mpQueueButtons->mpButtons[1] = new CAdvancedButton<CWorldState, void, int>(SPointData(100, 5), DX::XMFLOAT2(90.0f, 90.0f),
		*this, &CWorldState::UnqueueUnit, TR_DOWN, true, 0.2f);
	mpQueueButtons->mpButtons[2] = new CAdvancedButton<CWorldState, void, int>(SPointData(195, 5), DX::XMFLOAT2(90.0f, 90.0f),
		*this, &CWorldState::UnqueueUnit, TR_DOWN, true, 0.2f);
	mpQueueButtons->mpButtons[3] = new CAdvancedButton<CWorldState, void, int>(SPointData(290, 5), DX::XMFLOAT2(90.0f, 90.0f),
		*this, &CWorldState::UnqueueUnit, TR_DOWN, true, 0.2f);
	mpQueueButtons->mpButtons[4] = new CAdvancedButton<CWorldState, void, int>(SPointData(385, 5), DX::XMFLOAT2(90.0f, 90.0f),
		*this, &CWorldState::UnqueueUnit, TR_DOWN, true, 0.2f);

	for (int i = 0; i < mpQueueButtons->mNumButtons; i++)
	{
		mpUnitsButtonList.push_back(mpQueueButtons->mpButtons[i]);
	}

	mpSprQProg = nullptr;
	mQueuePrevProg = 0;


	// Space Buttons
	mpSpaceAtaackButtons = new CAdvancedButton<CWorldState, void>("AttackButton.png", "AttackButtonMO.png", SPointData(165, 765),
		DX::XMFLOAT2(90.0f, 90.0f), *this, &CWorldState::LaunchAttack);
	mpGenericButtonList.push_back(mpSpaceAtaackButtons);

	// Health bar variables
	mpSprHealth = nullptr;
	mPrevHealth = 0;


	// EARTH
	mpEarthGrid = new CGrid(DX::XMFLOAT3(-2000.0f, 0.4f, 0.0f));

	DX::XMFLOAT3 gridCentre = mpEarthGrid->GetGridCentrePos();
	DX::XMFLOAT3 gridBottomLeft = mpEarthGrid->GetGridStartPos();
	DX::XMFLOAT3 gridTopRight = mpEarthGrid->GetGridEndPos();
	mpMdlSkybox->SetPosition(gridCentre.x, -1.0f, gridCentre.z);

	mpMshGridArea = gpEngine->LoadMesh("Grid.x");
	mpMdlEarthGridArea = mpMshGridArea->CreateModel(gridCentre.x, 0.3f, gridCentre.z);
	mpMdlEarthGridArea->ScaleX((GRID_SIZE_X * GRID_TILE_SIZE) / 2.0f);
	mpMdlEarthGridArea->ScaleZ((GRID_SIZE_Y * GRID_TILE_SIZE) / 2.0f);

	mpMshGrassArea = gpEngine->LoadMesh("Grass.x");
	mpMdlEarthGrassArea = mpMshGrassArea->CreateModel(gridCentre.x, 0.1f, gridCentre.z);
	mpMdlEarthGrassArea->ScaleX(GRID_SIZE_X * GRID_TILE_SIZE * 4.0f);
	mpMdlEarthGrassArea->ScaleZ(GRID_SIZE_Y * GRID_TILE_SIZE * 4.0f);


	// MARS
	float marsXStart = (float)(GRID_SIZE_X * GRID_TILE_SIZE) + 4000.0f;
	mpMarsGrid = new CGrid(DX::XMFLOAT3(marsXStart, 0.4f, 0.0f));

	gridCentre = mpMarsGrid->GetGridCentrePos();
	gridBottomLeft = mpMarsGrid->GetGridStartPos();
	gridTopRight = mpMarsGrid->GetGridEndPos();

	mpMdlMarsGridArea = mpMshGridArea->CreateModel(gridCentre.x, 0.3f, gridCentre.z);
	mpMdlMarsGridArea->ScaleX((GRID_SIZE_X * GRID_TILE_SIZE) / 2.0f);
	mpMdlMarsGridArea->ScaleZ((GRID_SIZE_Y * GRID_TILE_SIZE) / 2.0f);

	mpMdlMarsGrassArea = mpMshGrassArea->CreateModel(gridCentre.x, 0.1f, gridCentre.z);
	mpMdlMarsGrassArea->ScaleX(GRID_SIZE_X * GRID_TILE_SIZE * 4.0f);
	mpMdlMarsGrassArea->ScaleZ(GRID_SIZE_Y * GRID_TILE_SIZE * 4.0f);
	mpMdlMarsGrassArea->SetSkin("sand.jpg");


	// LOAD IN OR CREATE PLAYERS
	//-----------------------------
	// Check if a game is being loaded by file
	if (CStateControl::GetInstance()->GetSettingsManager()->IsGameBeingLoaded())
	{
		// Create an instance of the loading and saving manager
		CGameSaverLoader* pLoader = new CGameSaverLoader();

		// Load the game from the provided file
		std::string fileToLoad;
		CStateControl::GetInstance()->GetSettingsManager()->GetLoadFileName(fileToLoad);

		// Create an input file stream - already know it exists through verification so no need to check again
		std::ifstream inFile(fileToLoad);

		// Load settings through settings manager
		CStateControl::GetInstance()->GetSettingsManager()->LoadSettings(inFile);

		// Create players based on the settings just loaded
		CStateControl::GetInstance()->GetPlayerManager()->CreatePlayers(FAC_EARTH_DEFENSE_FORCE, 1, CStateControl::GetInstance()->GetSettingsManager()->GetStartingResourcesAmount(),
			CStateControl::GetInstance()->GetSettingsManager()->GetAIDifficulty());

		// Get the players from the player manager & store relevant grids
		mpHumanPlayer = mpPlayerManager->GetHumanPlayer();
		mpAIPlayer = mpPlayerManager->GetAIPlayer(0);
		mpHumanPlayer->StorePlayerGridState(mpEarthGrid);
		mpAIPlayer->StorePlayerGridState(mpMarsGrid);

		// Load the game (required data on load file stored in settings)
		pLoader->LoadGame(inFile);

		// Initialise news ticker
		gpNewsTicker = new CNewsTicker();

		// Set game being loaded to false (as it has already loaded)
		CStateControl::GetInstance()->GetSettingsManager()->SetIfLoadingGame(false);
	}
	// if players have already been initialised, this is not necessary
	else if (!mpPlayerManager->ArePlayersInitialised())
	{
		// INITIALISE PLAYERS
		//-----------------------------
		mpPlayerManager->CreatePlayers(FAC_EARTH_DEFENSE_FORCE, 1, CStateControl::GetInstance()->GetSettingsManager()->GetStartingResourcesAmount(),
			CStateControl::GetInstance()->GetSettingsManager()->GetAIDifficulty());
		mpHumanPlayer = mpPlayerManager->GetHumanPlayer();
		mpAIPlayer = mpPlayerManager->GetAIPlayer(0);
		

		// INITIALISE WORLDS
		//-----------------------------
		// Set grids for each player for easy referal
		mpHumanPlayer->StorePlayerGridState(mpEarthGrid);
		mpAIPlayer->StorePlayerGridState(mpMarsGrid);

		// Initialise news ticker
		gpNewsTicker = new CNewsTicker();		
		
		// EARTH
		mpPlacingStructure = nullptr;
		CStructure* pTemp = new CComCentre();
		mpCurTile = mpEarthGrid->GetTileData(SPointData((int)(GRID_SIZE_X / 2.0f), (int)(GRID_SIZE_Y / 2.0f)));
		OnPlacingStructureChange(pTemp);
		mpHumanPlayer->PurchaseStructure(mpPlacingStructure, mpEarthGrid, mpCurTile);

		// MARS
		mpPlacingStructure = nullptr;
		pTemp = new CComCentre();
		mpCurTile = mpMarsGrid->GetTileData(SPointData((int)(GRID_SIZE_X / 2.0f), (int)(GRID_SIZE_Y / 2.0f)));
		OnPlacingStructureChange(pTemp);
		mpAIPlayer->PurchaseStructure(mpPlacingStructure, mpMarsGrid, mpCurTile);
		mpPlacingStructure = nullptr;

		// Create the resource piles for each player
		mpHumanPlayer->CreateResourcePiles();
		mpAIPlayer->CreateResourcePiles();

		// Create the walls for the players
		mpHumanPlayer->ConstructWalls();
		mpAIPlayer->ConstructWalls();

		// Ensure no tiles are glowing red
		mpEarthGrid->ResetTilesModels();
		mpMarsGrid->ResetTilesModels();
	}
	else
	{
		// RE-CONSTRUCT BUILDINGS
		//-----------------------------
		mpHumanPlayer->LoadPlayerGridModels();
		mpAIPlayer->LoadPlayerGridModels();

		// Re-assign previous grid data
		SafeDelete(mpEarthGrid);
		SafeDelete(mpMarsGrid);
		mpEarthGrid = mpHumanPlayer->GetPlayerGrid();
		mpMarsGrid = mpAIPlayer->GetPlayerGrid();

		// Create the walls for the players
		mpHumanPlayer->ConstructWalls();
		mpAIPlayer->ConstructWalls();
	}

	// Load trees around earth
	// Generate a random number of trees
	gridCentre = mpEarthGrid->GetGridCentrePos();
    gridBottomLeft = mpEarthGrid->GetGridStartPos();
	gridTopRight = mpEarthGrid->GetGridEndPos();

	CRandomiser* pRandomiser = new CRandomiser();
	int numTrees = pRandomiser->GetRandomInt(180, 220);

	for (int i = 0; i < numTrees; i++)
	{
		float x = 0.0f;
		float z = 0.0f;
		float rotation = pRandomiser->GetRandomFloat(0.0f, 180.0f);
		float scale = pRandomiser->GetRandomFloat(3.0f, 5.0f);

		// Get random x and z position
		do
		{
			x = pRandomiser->GetRandomFloat(-2400.0, -1000.0);
			z = pRandomiser->GetRandomFloat(-1000.0, 1000.0);
		} while (x > (gridBottomLeft.x - 30.0f) && x < (gridTopRight.x + 30.0f) && z >(gridBottomLeft.z - 30.0f) && z < (gridTopRight.z + 30.0f));

		// Create a random tree or bush
		int itemNum = pRandomiser->GetRandomInt(0, 1);
		mpMdlTreeList.push_back(mpTreeMeshes[itemNum]->CreateModel(x, 0.0f, z));
		mpMdlTreeList.back()->Scale(scale);
		mpMdlTreeList.back()->RotateY(rotation);
	}

	// Load rocks around mars
	gridCentre = mpMarsGrid->GetGridCentrePos();
	gridBottomLeft = mpMarsGrid->GetGridStartPos();
	gridTopRight = mpMarsGrid->GetGridEndPos();

	int numRocks = pRandomiser->GetRandomInt(180, 220);

	for (int i = 0; i < numTrees; i++)
	{
		float x = 0.0f;
		float z = 0.0f;
		float rotation = pRandomiser->GetRandomFloat(0.0f, 180.0f);
		float scale = pRandomiser->GetRandomFloat(2.5f, 4.5f);

		// Get random x and z position
		do
		{
			x = pRandomiser->GetRandomFloat(3200.0, 5800.0);
			z = pRandomiser->GetRandomFloat(-1000.0, 1000.0);
		} while (x > (gridBottomLeft.x - 30.0f) && x < (gridTopRight.x + 30.0f) && z >(gridBottomLeft.z - 30.0f) && z < (gridTopRight.z + 30.0f));

		// Create a random tree or bush
		int itemNum = pRandomiser->GetRandomInt(0, 10);
		mpMdlRockList.push_back(mpRockMeshes[itemNum]->CreateModel(x, 0.0f, z));
		mpMdlRockList.back()->Scale(scale);
		mpMdlRockList.back()->RotateY(rotation);
	}


	// INITIALISE CAMERAS
	//-----------------------------
	mpCamEarth = new CSphericalCamera(mpEarthGrid->GetGridCentrePos(), 350.0f, DX::XMConvertToRadians(-90.0f), DX::XMConvertToRadians(50.0f));
	mpCamMars = new CSphericalCamera(mpMarsGrid->GetGridCentrePos(), 350.0f, DX::XMConvertToRadians(-90.0f), DX::XMConvertToRadians(50.0f));
	mpCamEarth->Update();
	mpCamMars->Update();

	mpCamCurrent = mpCamEarth;

	// Ensure global pointer holds correct camera
	gpCurWorldCamera = mpCamCurrent->GetCamera();

	// Camera limitations
	float threshold = 50.0f;
	mMinEarthPos = mpEarthGrid->GetGridStartPos();
	mMinEarthPos.x -= threshold;
	mMinEarthPos.z -= threshold;
	mMaxEarthPos = mpEarthGrid->GetGridEndPos();
	mMaxEarthPos.x += threshold;
	mMaxEarthPos.z += threshold;

	mMinMarsPos = mpMarsGrid->GetGridStartPos();
	mMinMarsPos.x -= threshold;
	mMinMarsPos.z -= threshold;
	mMaxMarsPos = mpMarsGrid->GetGridEndPos();
	mMaxMarsPos.x += threshold;
	mMaxMarsPos.z += threshold;

	mCurCamPrevPos = DX::XMFLOAT3(0.0f, mpCamEarth->GetCamera()->GetY(), 0.0f);


	// Set camera to victor
	if (mpHumanPlayer->GetWonLastSpaceBattle())
	{
		mpCamCurrent = mpCamMars;
		mpHumanPlayer->SetWonLastSpaceBattle(false);
	}


	// FINALISE BUTTONS
	//-----------------------------
	OnStructureSelectChange(nullptr);
	OnUnitSelectChange(nullptr, false);


	// INITIALISE MUSIC
	//-----------------------------
	string mMusicFile = "Perpetual Tension.wav"; //Sets the music file
	DX::XMFLOAT3 mSourcePos = { mpCamEarth->GetCamera()->GetX(), mpCamEarth->GetCamera()->GetY(), mpCamEarth->GetCamera()->GetZ() };
	DX::XMFLOAT3 mSourceVel = { 0.0f, 0.0f, 0.0f };
	DX::XMFLOAT3 listenerPos = { mpCamEarth->GetCamera()->GetX(), mpCamEarth->GetCamera()->GetY(), mpCamEarth->GetCamera()->GetZ() };
	DX::XMFLOAT3 listenerVel = { 0.0f, 0.0f, 0.0f };
	float volume = CStateControl::GetInstance()->GetSettingsManager()->GetMusicVolume();
	mMusic = new CSound(mMusicFile, mSourcePos, mSourceVel, true, volume, listenerPos, listenerVel); //Initialise music
	mMusic->PlaySound(); //Play music on loop
}

void CWorldState::StateUpdate()
{
	// SCENE DRAW
	//------------------------------
	gpEngine->DrawScene(mpCamCurrent->GetCamera());
	

	// MOUSE TRACKING
	//------------------------------
	// Remember previous mouse position
	mpMousePrevScreenPos.mPosX = mpMouseScreenPos->mPosX;
	mpMousePrevScreenPos.mPosY = mpMouseScreenPos->mPosY;

	// Update mouse position
	mpMouseScreenPos->mPosX = gpEngine->GetMouseX();
	mpMouseScreenPos->mPosY = gpEngine->GetMouseY();

	mpSprCursor->SetPosition(mpMouseScreenPos->mPosX, mpMouseScreenPos->mPosY);


	// IF GAME HAS ENDED
	//------------------------------
	if (mEnd)
	{
		// End menu updates
		// Check for click
		if (gpEngine->KeyHit(Mouse_LButton))
		{
			mLMouseClicked = true;
		}

		// Update pause menu visuals
		// Determine which player has lost
		if (mpHumanPlayer->IsAlive()) mpTitleFont->Draw("HEROIC VICTORY!", 1015, 90, kCyan, kCentre, kTop);
		else if (mpAIPlayer->IsAlive()) mpTitleFont->Draw("HUMILIATING DEFEAT!", 1015, 90, kCyan, kCentre, kTop);

		mpButtonFont->Draw("QUIT TO MAIN MENU", 1015, 505, kWhite, kCentre, kTop);

		// Update buttons
		for (miterPauseButtons = mpPauseButtonList.begin(); miterPauseButtons != mpPauseButtonList.end(); miterPauseButtons++)
		{
			CAdvancedButton<CWorldState, void>* pButton = (*miterPauseButtons);
			// Check if the mouse is colliding with the object
			if (pButton->GetBoundingBox().IsColliding(DX::XMFLOAT3(mpMouseScreenPos->mPosX, 0.0f, mpMouseScreenPos->mPosY)))
			{
				pButton->SetMouseOver(true);
			}
			else
			{
				pButton->SetMouseOver(false);
			}

			// Check for click 
			if (pButton->GetMouseOver())
			{
				// Check if the mouse is over the button
				if (mLMouseClicked)
				{
					// Execute the command attached to the button which has been clicked
					pButton->Execute();
					mLMouseClicked = false;
					return;
				}
			}

			// Update the button
			pButton->Update();
		}

		// Exit this function so everything underneath does not get executed
		mLMouseClicked = false;
		return;
	}


	// CHECK FOR PAUSE
	//------------------------------
	if (gpEngine->KeyHit(Key_Escape))
	{
		if (!mPaused)
		{
			OnPause();
		}
		else
		{
			OnUnPause();
		}
	}


	// IF PAUSED
	//------------------------------
	if (mPaused)
	{
		// Pause menu updates
		// Check for click
		if (gpEngine->KeyHit(Mouse_LButton))
		{
			mLMouseClicked = true;
		}

		// Check if the type box exists - will determine whether the user is typing or not
		if (mpTypeBox == nullptr)
		{
			// Update pause menu visuals
			mpTitleFont->Draw("GAME PAUSED", 1015, 90, kCyan, kCentre, kTop);
			mpButtonFont->Draw("CONTINUE GAME", 1015, 365, kWhite, kCentre, kTop);
			mpButtonFont->Draw("SAVE GAME", 1015, 435, kWhite, kCentre, kTop);
			mpButtonFont->Draw("QUIT TO MAIN MENU", 1015, 505, kWhite, kCentre, kTop);
		}
		else
		{
			// Update pause menu visuals
			mpTitleFont->Draw("SAVE GAME", 1015, 90, kCyan, kCentre, kTop);
			mpButtonFont->Draw("TYPE THE NAME OF THE SAVE FILE BELOW:", 780, 200, kWhite, kLeft, kTop);
			mpButtonFont->Draw("SAVE", 945, 295, kWhite, kCentre, kTop);
			mpButtonFont->Draw("CANCEL", 1085, 295, kWhite, kCentre, kTop);

			// Update the type box
			mpTypeBox->Update();
		}

		// Update buttons
		for (miterPauseButtons = mpPauseButtonList.begin(); miterPauseButtons != mpPauseButtonList.end(); miterPauseButtons++)
		{
			CAdvancedButton<CWorldState, void>* pButton = (*miterPauseButtons);
			// Check if the mouse is colliding with the object
			if (pButton->GetBoundingBox().IsColliding(DX::XMFLOAT3(mpMouseScreenPos->mPosX, 0.0f, mpMouseScreenPos->mPosY)))
			{
				pButton->SetMouseOver(true);
			}
			else
			{
				pButton->SetMouseOver(false);
			}

			// Check for click 
			if (pButton->GetMouseOver())
			{
				// Check if the mouse is over the button
				if (mLMouseClicked)
				{
					// Execute the command attached to the button which has been clicked
					pButton->Execute();
					mLMouseClicked = false;
					return;
				}
			}

			// Update the button
			pButton->Update();
		}

		// Exit this function so everything underneath does not get executed
		mLMouseClicked = false;
		return;
	}


	// EDGE SCROLLING
	//------------------------------
	// Matrix for identifying direction to scroll
	DX::XMFLOAT4X4 camMatrix;
	DX::XMFLOAT3 camNormalDirection;
	mpCamCurrent->GetCamera()->GetMatrix(&camMatrix.m[0][0]);

	// Check for side scrolling
	if (mpMouseScreenPos->mPosX < 10 || gpEngine->KeyHeld(Key_A))
	{
		// Mouse on left side of screen
		// Move in the negative local x direction but do not move on the Y axis
		camNormalDirection = { camMatrix.m[0][0], 0.0f, camMatrix.m[0][2] };
		DX::XMVECTOR vec = DX::XMLoadFloat3(&camNormalDirection);
		vec = DX::XMVector4Normalize(vec);
		DX::XMStoreFloat3(&camNormalDirection, vec);
		camNormalDirection.x *= -CAM_MOVE_SPEED * gFrameTime;
		camNormalDirection.z *= -CAM_MOVE_SPEED * gFrameTime;
		mpCamCurrent->AdjustPivotPoint(camNormalDirection);
	}

	if (mpMouseScreenPos->mPosX > WINDOW_WIDTH - 30 || gpEngine->KeyHeld(Key_D))
	{
		// Mouse on right side of screen
		// Move in the local x direction but do not move on the Y axis
		camNormalDirection = { camMatrix.m[0][0], 0.0f, camMatrix.m[0][2] };
		DX::XMVECTOR vec = DX::XMLoadFloat3(&camNormalDirection);
		vec = DX::XMVector4Normalize(vec);
		DX::XMStoreFloat3(&camNormalDirection, vec);
		camNormalDirection.x *= CAM_MOVE_SPEED * gFrameTime;
		camNormalDirection.z *= CAM_MOVE_SPEED * gFrameTime;
		mpCamCurrent->AdjustPivotPoint(camNormalDirection);
	}

	if (mpMouseScreenPos->mPosY < 10 || gpEngine->KeyHeld(Key_W))
	{
		// Mouse on top side of screen
		// Move in the local z direction but do not move on the Y axis
		camNormalDirection = { camMatrix.m[2][0], 0.0f, camMatrix.m[2][2] };
		DX::XMVECTOR vec = DX::XMLoadFloat3(&camNormalDirection);
		vec = DX::XMVector4Normalize(vec);
		DX::XMStoreFloat3(&camNormalDirection, vec);
		camNormalDirection.x *= CAM_MOVE_SPEED * gFrameTime;
		camNormalDirection.z *= CAM_MOVE_SPEED * gFrameTime;
		mpCamCurrent->AdjustPivotPoint(camNormalDirection);
	}

	if (mpMouseScreenPos->mPosY > WINDOW_HEIGHT - 50 || gpEngine->KeyHeld(Key_S))
	{
		// Mouse on bottom side of screen
		// Move in the negative local z direction but do not move on the Y axis
		camNormalDirection = { camMatrix.m[2][0], 0.0f, camMatrix.m[2][2] };
		DX::XMVECTOR vec = DX::XMLoadFloat3(&camNormalDirection);
		vec = DX::XMVector4Normalize(vec);
		DX::XMStoreFloat3(&camNormalDirection, vec);
		camNormalDirection.x *= -CAM_MOVE_SPEED * gFrameTime;
		camNormalDirection.z *= -CAM_MOVE_SPEED * gFrameTime;
		mpCamCurrent->AdjustPivotPoint(camNormalDirection);
	}

	// Camera rotation keys
	if (gpEngine->KeyHeld(Key_Right))
	{
		mpCamCurrent->AdjustPhi(DX::XMConvertToRadians(CAM_MOVE_SPEED * gFrameTime));
	}

	if (gpEngine->KeyHeld(Key_Left))
	{
		mpCamCurrent->AdjustPhi(DX::XMConvertToRadians(-CAM_MOVE_SPEED * gFrameTime));
	}

	if (gpEngine->KeyHeld(Key_Up))
	{
		mpCamCurrent->AdjustTheta(DX::XMConvertToRadians(-CAM_MOVE_SPEED * gFrameTime));
	}

	if (gpEngine->KeyHeld(Key_Down))
	{
		mpCamCurrent->AdjustTheta(DX::XMConvertToRadians(CAM_MOVE_SPEED * gFrameTime));
	}

	// Get camera's looking at position
	DX::XMFLOAT3 pivotPos;
	mpCamCurrent->GetPivotPoint(pivotPos);

	// Check camera is within boundaries
	if (mpCamCurrent == mpCamEarth)
	{
		if (pivotPos.x >= mMinEarthPos.x && pivotPos.x <= mMaxEarthPos.x &&
			pivotPos.z >= mMinEarthPos.z && pivotPos.z <= mMaxEarthPos.z)
		{
			// Remember current position as previous
			mCurCamPrevPos = pivotPos;
		}
		else
		{
			// Otherwise set the previous position as it is out of area
			mpCamCurrent->SetPivotPoint(mCurCamPrevPos);
		}
	}
	else
	{
		if (pivotPos.x >= mMinMarsPos.x && pivotPos.x <= mMaxMarsPos.x &&
			pivotPos.z >= mMinMarsPos.z && pivotPos.z <= mMaxMarsPos.z)
		{
			// Remember current position as previous
			mCurCamPrevPos = pivotPos;
		}
		else
		{
			// Otherwise set the previous position as it is out of area
			mpCamCurrent->SetPivotPoint(mCurCamPrevPos);
		}
	}


	// CHECK FOR MOUSE ROTATION
	//------------------------------
	if (gpEngine->KeyHeld(Mouse_MButton))
	{
		// Check if there has been a change in mouse's x position
		if (mpMousePrevScreenPos.mPosX != mpMouseScreenPos->mPosX)
		{
			// Determine if it is negative or positive
			float change = mpMouseScreenPos->mPosX - mpMousePrevScreenPos.mPosX;
			if (change < 0.0f)
			{
				mpCamCurrent->AdjustPhi(DX::XMConvertToRadians(-CAM_MOVE_SPEED * 6.0f * gFrameTime));
			}
			else
			{
				mpCamCurrent->AdjustPhi(DX::XMConvertToRadians(CAM_MOVE_SPEED * 6.0f * gFrameTime));
			}
		}

		// Check if there has been a change in mouse's y position
		if (mpMousePrevScreenPos.mPosY != mpMouseScreenPos->mPosY)
		{
			// Determine if it is negative or positive
			float change = mpMouseScreenPos->mPosY - mpMousePrevScreenPos.mPosY;
			if (change < 0.0f)
			{
				mpCamCurrent->AdjustTheta(DX::XMConvertToRadians(-CAM_MOVE_SPEED * 6.0f * gFrameTime));
			}
			else
			{
				mpCamCurrent->AdjustTheta(DX::XMConvertToRadians(CAM_MOVE_SPEED * 6.0f * gFrameTime));
			}
		}
	}

	// Update cameras
	mpCamCurrent->Update();


	// CHECK LIST OF UNITS
	//------------------------------
	if (mpUnitSelectionList.size())
	{
		// If there are units selected in a list, check to see if any of them have no health left
		// so that they can be removed from the list - avoids crashes
		for (miterUnitSelectionList = mpUnitSelectionList.begin(); miterUnitSelectionList != mpUnitSelectionList.end(); miterUnitSelectionList++)
		{
			if ((*miterUnitSelectionList)->GetHealth() <= 0.0f)
			{
				mpUnitSelectionList.erase(miterUnitSelectionList);
				break;
			}
		}
	}


	// METHODS
	//---------------------------
	UpdateMatrices();
	CalculateMouseGridPos();
	mMouseState = UpdateMouseState();
	DrawFontData();
	DisplaySelectedBuildingInfo();
	DisplaySelectedAgentInfo();

	// Update the current queue size ( if a structure is slected 0. Returns true if a change has occured
	if (CheckForQueueChange())
	{
		// Update the buttons for the queue
		OnStructureSelectChange(mpCurSelectedStructure);
	}

	// Update news ticker
	gpNewsTicker->UpdateTimers();


	// BUTTON UPDATES
	//---------------------------
	// Decrement left click cool down
	mClickCoolDown -= gFrameTime;

	if (gpEngine->KeyHeld(Mouse_LButton))
	{	
		if (!mLMouseHeld && mHoldCount > 0.1f)
		{
			// If it was clicked last frame & held threshold is reached, it's being held
			mLMouseHeld = true;
		}

		// Deselect everything if holding down is greater than a quarter of a second
		if (mHoldCount > 0.2f)
		{
			OnPlacingStructureChange(nullptr);
			OnUnitSelectChange(nullptr, false);
			OnStructureSelectChange(nullptr);
		}

		if (!mLMouseHeld && mClickCoolDown < 0.0f)
		{
			mLMouseClicked = true;
			mClickCoolDown = 0.15f;

			mDragStartPos = mMouseWorldPos;
			mDragStartPos.y = -200.0f;
		}

		// Increment held counter
		mHoldCount += gFrameTime;
	}
	else
	{
		// Check if the held button was previously active
		if (mLMouseHeld)
		{
			// Let go of mouse whilst holding - get end position
			mDragEndPos = mMouseWorldPos;
			mDragEndPos.y = 200.0f;

			// Create the drag box
			mpDragBox = new SBoundingCube(mDragStartPos, mDragEndPos);

			// use bounding box to check for unit selections against player's units
			mpHumanPlayer->CheckDragSelection(mpDragBox->mBox, mpUnitSelectionList);

			// Delete the dragging box
			if (mpDragBox)
			{
				mspMshDrag->RemoveModel(mpMdlDragBox);
				mpMdlDragBox = nullptr;
			}

			// Update buttons
			OnUnitSelectChange(nullptr, true);
		}
		
		// No button clicking
		mLMouseClicked = false;
		mLMouseHeld = false;
		mHoldCount = 0.0f;

		// Delete the box - no longer needed
		SafeDelete(mpDragBox);
	}

	// Check if held is still true
	if (mLMouseHeld)
	{
		if (mpMdlDragBox)
		{
			mspMshDrag->RemoveModel(mpMdlDragBox);
		}
		
		// Scale the model to the mouse's position
		float scaleX = mMouseWorldPos.x - mDragStartPos.x;
		float scaleZ = mMouseWorldPos.z - mDragStartPos.z;
		
		// Create new dragging box model
		mpMdlDragBox = mspMshDrag->CreateModel(mDragStartPos.x, 0.5f, mDragStartPos.z);
		mpMdlDragBox->ScaleX(scaleX);
		mpMdlDragBox->ScaleZ(scaleZ);
	}

	// Loop through generic buttons
	for (miterGenericButtons = mpGenericButtonList.begin(); miterGenericButtons != mpGenericButtonList.end(); miterGenericButtons++)
	{
		CAdvancedButton<CWorldState, void>* pButton = (*miterGenericButtons);
		// Check if the mouse is colliding with the object
		if (pButton->GetBoundingBox().IsColliding(DX::XMFLOAT3((float)mpMouseScreenPos->mPosX, 0.0f, (float)mpMouseScreenPos->mPosY)))
		{
			pButton->SetMouseOver(true);
		}
		else
		{
			pButton->SetMouseOver(false);
		}

		// Update the button
		pButton->Update();

		// Check for click 
		if (pButton->GetMouseOver())
		{
			// Check if the mouse is over the button
			if (mLMouseClicked)
			{
				pButton->Execute();
			}
		}
	}

	// Loop through building unit buttons
	for (miterUnitsButtons = mpUnitsButtonList.begin(); miterUnitsButtons != mpUnitsButtonList.end(); miterUnitsButtons++)
	{
		CAdvancedButton<CWorldState, void, int>* pButton = (*miterUnitsButtons);
		// Check if the mouse is colliding with the object
		if (pButton->GetBoundingBox().IsColliding(DX::XMFLOAT3((float)mpMouseScreenPos->mPosX, 0.0f, (float)mpMouseScreenPos->mPosY)))
		{
			pButton->SetMouseOver(true);
		}
		else
		{
			pButton->SetMouseOver(false);
		}

		// Update the button
		pButton->Update();

		// Check for click 
		if (pButton->GetMouseOver())
		{
			// Check if the mouse is over the button
			if (mLMouseClicked)
			{
				int index = mpBarracksButtons->GetMouseOverIndex();
				if (index != -1)
				{
					pButton->Execute(index);
					break;
				}

				index = mpHellipadButtons->GetMouseOverIndex();
				if (index != -1)
				{
					pButton->Execute(index);
					break;
				}

				index = mpComCentreButtons->GetMouseOverIndex();
				if (index != -1)
				{
					pButton->Execute(index);
					break;
				}

				index = mpSpaceCentreButtons->GetMouseOverIndex();
				if (index != -1)
				{
					pButton->Execute(index);
					break;
				}

				index = mpQueueButtons->GetMouseOverIndex();
				if (index != -1)
				{
					pButton->Execute(index);
					break;
				}
			}
		}
	}

	// If after checking for button presses and there is still the left mouse button unsolved, remove any selected units
	if (mLMouseClicked)
	{
		OnUnitSelectChange(nullptr, false);
	}

	// Loop through key presses
	CheckKeyPresses();


	// MODEL UPDATES
	//---------------------------
	if (mMouseGridPos.mPosX != mMousePrevGridPos.mPosX || mMouseGridPos.mPosY != mMousePrevGridPos.mPosY)
	{
		UpdateHeldStructure();
		mMousePrevGridPos = mMouseGridPos;
	}

	/*if (mpCurSelectedAgent)
	{
		if (mpCurSelectedAgent->GetAttackTarget() == nullptr)
		{
			mpCurSelectedAgent->SetAutoTimer(mpCurSelectedAgent->GetAutoTimer() + gFrameTime);
			if (mpCurSelectedAgent->GetAutoTimer() >= mpCurSelectedAgent->GetAttackTimer())
			{
				float shortestDistance = 9999.0f;
				CGameObject* newTarget;
				GA_MultiMap* enemyUnits = mpHumanPlayer->GetWorldUnitList();
				GS_MultiMap* enemyStructures = mpHumanPlayer->GetStructuresList();
				GA_MultiMap::iterator miterEnemyUnits;
				GS_MultiMap::iterator miterEnemyStructures;

				for (miterEnemyUnits = enemyUnits->begin(); miterEnemyUnits != enemyUnits->end(); miterEnemyUnits++)
				{
					if (miterEnemyUnits->second != mpCurSelectedAgent)
					{
						float dist = distance((miterEnemyUnits->second->GetWorldXPos() - mpCurSelectedAgent->GetWorldXPos()), 0.0f, (miterEnemyUnits->second->GetWorldXPos() - mpCurSelectedAgent->GetWorldXPos()));
						if (dist < shortestDistance)
						{
							shortestDistance = dist;
							newTarget = miterEnemyUnits->second;
						}
					}
					
				}
				for (miterEnemyStructures = enemyStructures->begin(); miterEnemyStructures != enemyStructures->end(); miterEnemyStructures++)
				{
					float dist = distance((miterEnemyStructures->second->GetWorldXPos() - mpCurSelectedAgent->GetWorldXPos()), 0.0f, (miterEnemyStructures->second->GetWorldXPos() - mpCurSelectedAgent->GetWorldXPos()));
					if ( dist < shortestDistance)
					{
						shortestDistance = dist;
						newTarget = miterEnemyStructures->second;
					}
				}
				mpCurSelectedAgent->SetAttackTarget(newTarget);
				mpCurSelectedAgent->SetAutoTimer(0.0f);
				mpCurSelectedAgent->SetAttackTimer(gpRandomiser->GetRandomFloat(5.0f, 10.0f));
				
			}
		}
	}*/

	// UPDATE PLAYERS
	//------------------------------
	int updateResult = mpPlayerManager->UpdatePlayers();
	if (updateResult == 1 || updateResult == 2)
	{
		OnEnd();
	}
}

void CWorldState::StateCleanup()
{
	// Unload fonts
	if (mpTitleFont) gpEngine->RemoveFont(mpTitleFont);
	if (mpButtonFont) gpEngine->RemoveFont(mpButtonFont);
	mpTitleFont = nullptr;
	mpButtonFont = nullptr;
	
	// DISPLAY LOADING SCREEN
	ISprite* pLoading = gpEngine->CreateSprite("Loading.png");
	gpEngine->DrawScene(gpCurWorldCamera);

	// Remove loading screen sprite
	gpEngine->RemoveSprite(pLoading);
	
	// Lower any raised flags
	mPaused = false;
	mEnd = false;
	
	// Unload pause menu
	OnUnPause();
	
	// Store grid state
	mpHumanPlayer->StorePlayerGridState(mpEarthGrid);
	mpAIPlayer->StorePlayerGridState(mpMarsGrid);

	// Unclip cursor
	ClipCursor(&mBaseClip);
	
	SafeDelete(mpMouseScreenPos);

	mpMshSkybox->RemoveModel(mpMdlSkybox);

	//used to unload the structure models
	mpHumanPlayer->UnloadPlayerGridModels();
	mpAIPlayer->UnloadPlayerGridModels();

	gpEngine->RemoveSprite(mpBarBottom);
	gpEngine->RemoveMesh(mpMshSkybox);
	
	SafeDelete(mpCamEarth);
	SafeDelete(mpCamMars);

	if (mpSprQProg)
	{
		gpEngine->RemoveSprite(mpSprQProg);
	}

	if (mpSprHealth)
	{
		gpEngine->RemoveSprite(mpSprHealth);
	}

	gpEngine->RemoveSprite(mpSprCursor);

	SafeDelete(mpDragBox);

	if (mpMdlDragBox)
	{
		mspMshDrag->RemoveModel(mpMdlDragBox);
		mpMdlDragBox = nullptr;
	}

	mMusic->StopSound();

	// Unload buttons
	while (!mpGenericButtonList.empty())
	{
		CAdvancedButton<CWorldState, void>* pButton = mpGenericButtonList.back();
		SafeDelete(pButton);
		mpGenericButtonList.pop_back();
	}

	while (!mpUnitsButtonList.empty())
	{
		CAdvancedButton<CWorldState, void, int>* pButton = mpUnitsButtonList.back();
		SafeDelete(pButton);
		mpUnitsButtonList.pop_back();
	}

	// Unload trees
	while (!mpMdlTreeList.empty())
	{
		IMesh* pMshTemp = mpMdlTreeList.back()->GetMesh();
		pMshTemp->RemoveModel(mpMdlTreeList.back());
		mpMdlTreeList.pop_back();
	}

	// Unload rocks
	while (!mpMdlRockList.empty())
	{
		IMesh* pMshTemp = mpMdlRockList.back()->GetMesh();
		pMshTemp->RemoveModel(mpMdlRockList.back());
		mpMdlRockList.pop_back();
	}

	// Unload grids
	mpMshGrassArea->RemoveModel(mpMdlEarthGrassArea);
	mpMshGrassArea->RemoveModel(mpMdlMarsGrassArea);

	mpMshGridArea->RemoveModel(mpMdlEarthGridArea);
	mpMshGridArea->RemoveModel(mpMdlMarsGridArea);
}

bool CWorldState::CheckForQueueChange()
{
	if (mpCurSelectedStructure)
	{
		int newQueueSize = mpCurSelectedStructure->GetQueueSize();
		// Check for changes
		if (newQueueSize != mQueuePrevSize)
		{
			// A change has occured - store new variable
			mQueuePrevSize = newQueueSize;
			return true;
		}
	}

	// Nothing selected or changed so queue size cannot change
	return false;
}


//-----------------------------------------------------
// WORLD STATE CLASS EVENT HANDLERS
//-----------------------------------------------------
void CWorldState::OnPlacingStructureChange(CStructure* selStructure)
{
	// Remove currently selected structure - if it exists
	if (mpPlacingStructure)
	{
		mpPlacingStructure->UnloadIModel();
		SafeDelete(mpPlacingStructure);
	}

	// Store pointer to new structure
	mpPlacingStructure = selStructure;
	
	UpdateHeldStructure();
}

void CWorldState::OnStructureSelectChange(CStructure* pSelStructure)
{
	// If there is a different building selected, do the same as above
	if (pSelStructure != mpCurSelectedStructure && mpCurSelectedStructure)
	{
		mpCurSelectedStructure->SetDeselectedTexture();
	}
	
	// Check if something is slelected
	if (pSelStructure)
	{
		// Update buttons if the new selected building is not the same as the previous one
		if (mpCurSelectedStructure != pSelStructure)
		{
			// Hide basic buttons
			mpButtonBarracks->Hide();
			mpButtonHellipad->Hide();
			mpButtonSpaceCentre->Hide();
			mpButtonPutUnitIntoSpace->Hide();

			// Show specific structure buttons
			mpButtonDelete->Show();
			// Identify type of building in order to display the correct buttons for it
			switch (pSelStructure->GetStructureType())
			{
			case STR_BARRACKS:
				// Hide other structure buttons
				mpHellipadButtons->Hide();
				mpComCentreButtons->Hide();
				mpSpaceCentreButtons->Hide();
				// Show this structures buttons
				mpBarracksButtons->Show();
				break;
			case STR_COM_CENTRE:
				// Hide other structure buttons
				mpBarracksButtons->Hide();
				mpHellipadButtons->Hide();
				mpSpaceCentreButtons->Hide();
				// Show this structures buttons
				mpComCentreButtons->Show();
				break;
			case STR_HELLIPAD:
				// Hide other structure buttons
				mpBarracksButtons->Hide();
				mpComCentreButtons->Hide();
				mpSpaceCentreButtons->Hide();
				// Show this structures buttons
				mpHellipadButtons->Show();
				break;
			case STR_HOUSE:
				// Hide other structure buttons
				mpBarracksButtons->Hide();
				mpHellipadButtons->Hide();
				mpComCentreButtons->Hide();
				mpSpaceCentreButtons->Hide();
				// This structure has no buttons
				break;
			case STR_SPACE_CENTRE:
				// Hide other structure buttons
				mpBarracksButtons->Hide();
				mpHellipadButtons->Hide();
				mpComCentreButtons->Hide();
				// Show this structures buttons
				mpSpaceCentreButtons->Show();
				break;
			case STR_WALL:
				// Hide other structure buttons
				mpBarracksButtons->Hide();
				mpHellipadButtons->Hide();
				mpComCentreButtons->Hide();
				mpSpaceCentreButtons->Hide();
				// This structure has no buttons
				break;
			}
		}
		
		// Set its texture to be selected
		pSelStructure->SetSelectedTexture();
		
		// Unload the existing queue buttons
		mpQueueButtons->UnloadSprites();
		
		std::deque<CGameAgent*>* pQueue = nullptr;
		std::deque<CGameAgent*>::iterator iterQ;

		// Get the pointer to the queue
		pQueue = pSelStructure->GetQueue();

		// Loop through the queue to create the buttons for each of the units in the queue
		if (pQueue != nullptr)
		{
			int i = 0;
			for (iterQ = pQueue->begin(); iterQ != pQueue->end(); iterQ++)
			{
				switch ((*iterQ)->GetAgentData()->mAgentType)
				{
				case GAV_ARTILLERY:
					mpQueueButtons->mpButtons[i]->LoadButtons("DefArtilleryButton2.png", "SelArtilleryButton2.png");
					break;

				case GAV_BOMBER:
					mpQueueButtons->mpButtons[i]->LoadButtons("DefBomberButton2.png", "SelBomberButton2.png");
					break;

				case GAV_FIGHTER:
					mpQueueButtons->mpButtons[i]->LoadButtons("DefFighterButton2.png", "SelFighterButton2.png");
					break;

				case GAV_INFANTRY:
					mpQueueButtons->mpButtons[i]->LoadButtons("DefInfantryButton2.png", "SelInfantryButton2.png");
					break;

				case GAV_MOTHERSHIP:
					mpQueueButtons->mpButtons[i]->LoadButtons("DefMothershipButton2.png", "SelmothershipButton2.png");
					break;

				case GAV_SPACE_FIGHTER:
					mpQueueButtons->mpButtons[i]->LoadButtons("DefSpaceFighterButton2.png", "SelSpaceFighterButton2.png");
					break;

				case GAV_TANK:
					mpQueueButtons->mpButtons[i]->LoadButtons("DefTankButton2.png", "SelTankButton2.png");
					break;

				case GAV_TRANSPORT:
					mpQueueButtons->mpButtons[i]->LoadButtons("DefTransportButton2.png", "SelTransportButton2.png");
					break;

				case GAV_WORKER:
					mpQueueButtons->mpButtons[i]->LoadButtons("DefWorkerButton2.png", "SelWorkerButton2.png");
					break;
				}

				i++;
			}
		}

		// Determine whether or not selected building is under construction
		int percentage = 0;
		if (pSelStructure->GetState() == OBJ_CONSTRUCTING)
		{
			float timeLeft = pSelStructure->GetBuildTimeLeft();
			float maxTime = pSelStructure->GetBuildTime();
			float timeTaken = maxTime - timeLeft;
			percentage = (int)((timeTaken / maxTime) * 100.0f);
		}
		else
		{
			// Get health amount
			float healthLeft = pSelStructure->GetHealth();
			float maxHealth = pSelStructure->GetMaxHealth();
			percentage = (int)((healthLeft / maxHealth) * 100.0f);
		}

		// Get the remainder of dividing by 5 (health only shows these increments)
		int remainder = percentage % 5;
		mPrevHealth = percentage - remainder;

		// Update health bar
		OnItemHealthChange();
	}
	// If no unit is selected, display the other required buttons
	else if (!mpCurSelectedAgent)
	{
		// Nothing selected - unload sprites
		mpQueueButtons->UnloadSprites();

		// Hide structure specific buttons buttons
		mpButtonDelete->Hide();
		mpBarracksButtons->Hide();
		mpSpaceCentreButtons->Hide();
		mpHellipadButtons->Hide();
		mpComCentreButtons->Hide();
		mpButtonPutUnitIntoSpace->Hide();

		// Show base buttons
		mpButtonBarracks->Show();
		mpButtonHellipad->Show();
		mpButtonSpaceCentre->Show();
	}

	// Set the currently selected structure to the parameter passed in
	mpCurSelectedStructure = pSelStructure;
}

void CWorldState::OnUnitSelectChange(CGameAgent* pSelAgent, bool listSelection)
{
	// If there was a previously selected unit, set it's selected texture back
	if (mpCurSelectedAgent) mpCurSelectedAgent->SetDeselectedTexture();
	
	// Set the currently selected agent to the parameter passed in
	mpCurSelectedAgent = pSelAgent;

	// Check if something is selected
	if (mpCurSelectedAgent)
	{
		// Show the buttons specific to units
		mpButtonDelete->Show();
		mpButtonPutUnitIntoSpace->Show();

		// Hide base buttons
		mpButtonBarracks->Hide();
		mpButtonHellipad->Hide();
		mpButtonSpaceCentre->Hide();

		// Hide unit construction buttons
		mpBarracksButtons->Hide();
		mpHellipadButtons->Hide();
		mpSpaceCentreButtons->Hide();
		mpComCentreButtons->Hide();
		mpQueueButtons->UnloadSprites();

		// Get health amount
		float healthLeft = mpCurSelectedAgent->GetHealth();
		float maxHealth = mpCurSelectedAgent->GetMaxHealth();
		int percentage = (int)((healthLeft / maxHealth) * 100.0f);

		// Get the remainder of dividing by 5 (health only shows these increments)
		int remainder = percentage % 5;
		mPrevHealth = percentage - remainder;

		// Update health bar
		OnItemHealthChange();

		// Set the selected texture
		mpCurSelectedAgent->SetSelectedTexture();
	}
	else if (listSelection)
	{
		// Show the buttons specific to units
		mpButtonDelete->Show();
		mpButtonPutUnitIntoSpace->Show();

		// Hide base buttons
		mpButtonBarracks->Hide();
		mpButtonHellipad->Hide();
		mpButtonSpaceCentre->Hide();

		// Hide unit construction buttons
		mpBarracksButtons->Hide();
		mpHellipadButtons->Hide();
		mpSpaceCentreButtons->Hide();
		mpComCentreButtons->Hide();
		mpQueueButtons->UnloadSprites();

		// Get health amount of front unit
		float healthLeft = mpUnitSelectionList.front()->GetHealth();
		float maxHealth = mpUnitSelectionList.front()->GetMaxHealth();
		int percentage = (int)((healthLeft / maxHealth) * 100.0f);

		// Get the remainder of dividing by 5 (health only shows these increments)
		int remainder = percentage % 5;
		mPrevHealth = percentage - remainder;

		// Update health bar
		OnItemHealthChange();

		// Set the selected texture for each unit
		for (miterUnitSelectionList = mpUnitSelectionList.begin(); miterUnitSelectionList != mpUnitSelectionList.end(); miterUnitSelectionList++)
		{
			(*miterUnitSelectionList)->SetSelectedTexture();
		}
	}
	// If there are no multi-units selected
	else if (!listSelection)
	{
		// Check if there was previously a list
		if (mpUnitSelectionList.size())
		{
			// Set the deselected texture for each unit
			for (miterUnitSelectionList = mpUnitSelectionList.begin(); miterUnitSelectionList != mpUnitSelectionList.end(); miterUnitSelectionList++)
			{
				(*miterUnitSelectionList)->SetDeselectedTexture();
			}

			// Clear the selection
			mpUnitSelectionList.clear();
		}
	}
	// If no building is selected, show the other buttons
	else if (!mpCurSelectedStructure)
	{
		// Nothing selected - hide buttons no longer required
		mpButtonDelete->Hide();
		mpButtonPutUnitIntoSpace->Hide();

		// Show base buttons
		mpButtonBarracks->Show();
		mpButtonHellipad->Show();
		mpButtonSpaceCentre->Show();
	}
}

void CWorldState::OnStructureQueueProgressChange()
{
	// Check if the new progress is back to 0
	if (mQueuePrevProg == 0)
	{
		// Delete current sprite and return
		if (mpSprQProg)
		{
			gpEngine->RemoveSprite(mpSprQProg);
			mpSprQProg = nullptr;
			return;
		}
	}
	
	// Concatenate progress file name with progress amount
	strStream << "ButtonProg" << mQueuePrevProg << ".png";

	// Remove current sprite (if there is one)
	if (mpSprQProg)
	{
		gpEngine->RemoveSprite(mpSprQProg);
		mpSprQProg = nullptr;
	}

	// Create new sprite & clear string stream
	mpSprQProg = gpEngine->CreateSprite(strStream.str(), 5.0f, 5.0f, 0.6f);
	strStream.str("");
}

void CWorldState::OnItemHealthChange()
{
	// Check if the new progress is back to 0
	if (mPrevHealth == -5)
	{
		// Delete current sprite and return
		if (mpSprHealth)
		{
			gpEngine->RemoveSprite(mpSprHealth);
			mpSprHealth = nullptr;
			return;
		}
	}

	// Concatenate progress file name with progress amount
	strStream << "HealthBar" << mPrevHealth << ".png";

	// Remove current sprite (if there is one)
	if (mpSprHealth)
	{
		gpEngine->RemoveSprite(mpSprHealth);
		mpSprHealth = nullptr;
	}

	// Create new sprite & clear string stream
	mpSprHealth = gpEngine->CreateSprite(strStream.str(), 553.0f, 800.0f, 0.5f);
	strStream.str("");
}


void CWorldState::OnPause()
{
	// Raise the paused flag & stop music
	mPaused = true;
	mMusic->StopSound();

	// Create the visuals for the pause menu
	mpButtonFont = gpEngine->LoadFont("font2.bmp", 15U);
	mpTitleFont = gpEngine->LoadFont("font2.bmp", 35U);
	mpSprBackground = gpEngine->CreateSprite("PauseMenuBG.png", 400.0f, 50.0f, 0.75f);

	// Load buttons
	CAdvancedButton<CWorldState, void>* pNewButton = new CAdvancedButton<CWorldState, void>("DefMenuButton.png", "SelMenuButton.png", SPointData(815, 350),
		DX::XMFLOAT2(400.0f, 50.0f), *this, &CWorldState::Continue);
	mpPauseButtonList.push_back(pNewButton);

	pNewButton = new CAdvancedButton<CWorldState, void>("DefMenuButton.png", "SelMenuButton.png", SPointData(815, 420),
		DX::XMFLOAT2(400.0f, 50.0f), *this, &CWorldState::OnChooseSave);
	mpPauseButtonList.push_back(pNewButton);

	pNewButton = new CAdvancedButton<CWorldState, void>("DefMenuButton.png", "SelMenuButton.png", SPointData(815, 490),
		DX::XMFLOAT2(400.0f, 50.0f), *this, &CWorldState::QuitGame);
	mpPauseButtonList.push_back(pNewButton);
}

void CWorldState::OnUnPause()
{
	// Lower the paused flag & start music
	mPaused = false;
	mMusic->PlaySound();

	// Unload pause menu visuals
	if (mpTitleFont) gpEngine->RemoveFont(mpTitleFont);
	mpTitleFont = nullptr;
	if (mpButtonFont) gpEngine->RemoveFont(mpButtonFont);
	mpButtonFont = nullptr;
	if (mpSprBackground) gpEngine->RemoveSprite(mpSprBackground);
	mpSprBackground = nullptr;

	while (!mpPauseButtonList.empty())
	{
		CAdvancedButton<CWorldState, void>* pTmp = mpPauseButtonList.back();
		SafeDelete(pTmp);
		mpPauseButtonList.pop_back();
	}

	if (mpTypeBox) SafeDelete(mpTypeBox);
}

void CWorldState::OnChooseSave()
{
	// Create a typebox
	mpTypeBox = new CTypeBox(SPointData{ 770, 220 }, DX::XMFLOAT2{ 500.0f, 40.0f });

	// Remove the buttons as they are no longer needed
	while (!mpPauseButtonList.empty())
	{
		CAdvancedButton<CWorldState, void>* pButton = mpPauseButtonList.back();
		SafeDelete(pButton);
		mpPauseButtonList.pop_back();
	}

	// Create the save related buttons
	CAdvancedButton<CWorldState, void>* pNewButton = new CAdvancedButton<CWorldState, void>("DefSmallButton.png", "SelSmallButton.png", SPointData(895, 280),
		DX::XMFLOAT2(100.0f, 50.0f), *this, &CWorldState::SaveGame);
	mpPauseButtonList.push_back(pNewButton);

	pNewButton = new CAdvancedButton<CWorldState, void>("DefSmallButton.png", "SelSmallButton.png", SPointData(1035, 280),
		DX::XMFLOAT2(100.0f, 50.0f), *this, &CWorldState::OnChooseCancel);
	mpPauseButtonList.push_back(pNewButton);
}

void CWorldState::OnChooseCancel()
{
	// Remove visual items no longer required
	if (mpTypeBox) SafeDelete(mpTypeBox);

	// Remove the save buttons as they are no longer needed
	while (!mpPauseButtonList.empty())
	{
		CAdvancedButton<CWorldState, void>* pButton = mpPauseButtonList.back();
		SafeDelete(pButton);
		mpPauseButtonList.pop_back();
	}

	// Replace with buttons
	CAdvancedButton<CWorldState, void>* pNewButton = new CAdvancedButton<CWorldState, void>("DefMenuButton.png", "SelMenuButton.png", SPointData(815, 350),
		DX::XMFLOAT2(400.0f, 50.0f), *this, &CWorldState::Continue);
	mpPauseButtonList.push_back(pNewButton);

	pNewButton = new CAdvancedButton<CWorldState, void>("DefMenuButton.png", "SelMenuButton.png", SPointData(815, 420),
		DX::XMFLOAT2(400.0f, 50.0f), *this, &CWorldState::OnChooseSave);
	mpPauseButtonList.push_back(pNewButton);

	pNewButton = new CAdvancedButton<CWorldState, void>("DefMenuButton.png", "SelMenuButton.png", SPointData(815, 490),
		DX::XMFLOAT2(400.0f, 50.0f), *this, &CWorldState::QuitGame);
	mpPauseButtonList.push_back(pNewButton);
}

void CWorldState::OnEnd()
{
	// Raise the paused flag & stop music
	mEnd = true;
	mMusic->StopSound();

	// Create the visuals for the pause menu
	mpButtonFont = gpEngine->LoadFont("font2.bmp", 15U);
	mpTitleFont = gpEngine->LoadFont("font2.bmp", 35U);
	mpSprBackground = gpEngine->CreateSprite("PauseMenuBG.png", 400.0f, 50.0f, 0.75f);
	
	// Only the quit button is required here.
	CAdvancedButton<CWorldState, void>* pNewButton = new CAdvancedButton<CWorldState, void>("DefMenuButton.png", "SelMenuButton.png", SPointData(815, 490),
		DX::XMFLOAT2(400.0f, 50.0f), *this, &CWorldState::QuitGame);
	mpPauseButtonList.push_back(pNewButton);
}


//-----------------------------------------------------
// WORLD STATE CLASS BUTTON EVENT FUNCTIONS
//-----------------------------------------------------
void CWorldState::QueueUnit(int index)
{
	if (!mpCurSelectedStructure) return;
	
	// If there was an error, send it to ticker
	switch (mpCurSelectedStructure->AddToQueue(index, mpHumanPlayer))
	{
	case ERR_INCORRECT_INDEX:
		gpNewsTicker->AddNewElement("This structure does not have the typed unit.", true);
		break;
	case ERR_MAX_QUEUE_SIZE:
		gpNewsTicker->AddNewElement("Structure at maximum queue size!", true);
		break;
	case ERR_NOT_READY:
		gpNewsTicker->AddNewElement("Structure still constructing!", true);
		break;
	case ERR_NO_MINERALS:
		gpNewsTicker->AddNewElement("Insufficient minerals for this unit.", true);
		break;
	case ERR_POP_LIMIT:
		gpNewsTicker->AddNewElement("You have reached the population limit. Build more houses!", true);
		break;
	}
	mLMouseClicked = false;
}

void CWorldState::UnqueueUnit(int index)
{
	if (!mpCurSelectedStructure) return;

	mpCurSelectedStructure->RemoveFromQueue(index, mpHumanPlayer);
	mLMouseClicked = false;
}

void CWorldState::CreateBarracks()
{
	CStructure* pStructure = new CBarracks();
	OnPlacingStructureChange(pStructure);
	mLMouseClicked = false;
}

void CWorldState::CreateHellipad()
{
	CStructure* pStructure = new CHellipad();
	OnPlacingStructureChange(pStructure);
	mLMouseClicked = false;
}

void CWorldState::CreateSpaceCentre()
{
	CStructure* pStructure = new CSpaceCentre();
	OnPlacingStructureChange(pStructure);
	mLMouseClicked = false;
}

void CWorldState::CreateHouse()
{
	mLMouseClicked = false;
}

void CWorldState::DeleteSelection()
{
	// Check for a selected structure
	if (mpCurSelectedStructure)
	{
		// Set object to be deleted
		mpCurSelectedStructure->SetHealth(-1.0f);
		// pointer set to null
		OnStructureSelectChange(nullptr);
		mLMouseClicked = false;
	}
	// Check for a selected agent
	if (mpCurSelectedAgent)
	{
		// Set object to be deleted
		mpCurSelectedAgent->SetHealth(-1.0f);

		// Pointer set to null
		OnUnitSelectChange(nullptr, false);
		mLMouseClicked = false;
	}
	// Check for a list of selected units
	if (mpUnitSelectionList.size() != 0)
	{
		// Loop through each one to delete it
		for (miterUnitSelectionList = mpUnitSelectionList.begin(); miterUnitSelectionList != mpUnitSelectionList.end(); miterUnitSelectionList++)
		{
			(*miterUnitSelectionList)->SetHealth(-1.0f);
		}

		OnUnitSelectChange(nullptr, false);
		mLMouseClicked = false;
	}
}

void CWorldState::PutUnitIntoSpace()
{
	// Check for a single selected unit
	if (mpCurSelectedAgent)
	{
		if (mpHumanPlayer->PutUnitsOnShips(mpCurSelectedAgent))
		{
			gpNewsTicker->AddNewElement("Unit Boarded a Transport Ship.", false);

			// Change states of the agent (such as movement & target)
			mpCurSelectedAgent->SetAttackTarget(nullptr);
			mpCurSelectedAgent->CancelPathTarget();

			OnUnitSelectChange(nullptr, false);
		}
		else
		{
			gpNewsTicker->AddNewElement("No Transport Ships Have Open Space For This Unit.", true);
		}
	}
	// Check for a selection of units
	else if (mpUnitSelectionList.size() > 0)
	{
		// Loop through each unit in the list placing it on the ship
		for (miterUnitSelectionList = mpUnitSelectionList.begin(); miterUnitSelectionList != mpUnitSelectionList.end(); miterUnitSelectionList++)
		{
			CGameAgent* pAgent = (*miterUnitSelectionList);

			// Change states of the agent (such as movement & target)
			pAgent->SetAttackTarget(nullptr);
			pAgent->CancelPathTarget();

			// Attempt to store unit on a ship
			mpHumanPlayer->PutUnitsOnShips(pAgent);
		}

		//Display message
		gpNewsTicker->AddNewElement("Unit selection sent to transport ships.", false);

		OnUnitSelectChange(nullptr, false);
	}

	mLMouseClicked = false;
}

void CWorldState::LaunchAttack()
{
	mpHumanPlayer->LaunchAttack();
	mpAIPlayer->LaunchAttack();
	gCurState = GS_SPACE;
}

void CWorldState::Continue()
{
	OnUnPause();
}

void CWorldState::SaveGame()
{
	// Use what the user has typed in as the name of the file
	std::string fileName = mpTypeBox->GetText();

	// Remove leading & trailling spaces
	fileName.erase(fileName.begin(), std::find_if(fileName.begin(), fileName.end(), std::bind1st(std::not_equal_to<char>(), ' ')));
	fileName.erase(std::find_if(fileName.rbegin(), fileName.rend(), std::bind1st(std::not_equal_to<char>(), ' ')).base(), fileName.end());

	// If file name is black
	if (fileName == "")
	{
		// Set the filename to be equal to the date and time
		time_t curRawTime;
		struct tm* pTimeInfo;

		time(&curRawTime);
		pTimeInfo = localtime(&curRawTime);
		fileName = asctime(pTimeInfo);

		// Remove the \n at the end of the name
		fileName.erase(fileName.end() - 1);
		
		// Remove all : in the fileName
		for (int i = 0; i < 2; i++)
		{
			fileName.replace(fileName.find(":"), 1, "-");
		}
	}

	CGameSaverLoader* pSaver = new CGameSaverLoader();
	pSaver->SaveGame(fileName);
	OnUnPause();
	gpNewsTicker->AddNewElement("The game has been saved!", false);
}

void CWorldState::QuitGame()
{
	gCurState = GS_MAIN_MENU;
}
