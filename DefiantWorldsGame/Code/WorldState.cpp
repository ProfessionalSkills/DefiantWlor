//-----------------------------------------------------
// FILE: WorldState.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "WorldState.h"
#include "GameStateControl.h"


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
	mpCamCurrent->GetMatrix(&mCamWorldMatrix.m[0][0]);

	// XMMATRIX variables calculations
	DX::XMMATRIX world = DX::XMLoadFloat4x4(&mCamWorldMatrix);
	DX::XMMATRIX view = DX::XMMatrixInverse(NULL, DX::XMLoadFloat4x4(&mCamWorldMatrix));
	DX::XMMATRIX proj = DX::XMMatrixPerspectiveFovLH(DX::XM_PI / 3.4f, 1584.0f / 862.0f, NEAR_CLIP, FAR_CLIP);
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
			if (mpEarthGrid->TurnOnTiles(mpCurTile, mpPlacingStructure->GetBLPosition(), mpPlacingStructure->GetTRPosition(), mpPlacingStructure->GetGridSpawnLocation()))
			{
				mpPlacingStructure->SetBadTexture();
			}
			else
			{
				mpPlacingStructure->SetGoodTexture();
			}

			// If there are not enough materials, show the bad texture
			if (mpPlacingStructure->GetBuildCost() > mpHumanPlayer->GetMineralAmount())
			{
				mpPlacingStructure->SetBadTexture();
			}
			else
			{
				mpPlacingStructure->SetGoodTexture();
			}
			break;

		case MS_MARS_GRID:
			// When placing structure, check if any used tiles
			if (mpMarsGrid->TurnOnTiles(mpCurTile, mpPlacingStructure->GetBLPosition(), mpPlacingStructure->GetTRPosition(), mpPlacingStructure->GetGridSpawnLocation()))
			{
				mpPlacingStructure->SetBadTexture();
			}
			else
			{
				mpPlacingStructure->SetGoodTexture();
			}
			break;
		}
	}
}

void CWorldState::CalculateMouseGridPos()
{
	// Convert mouse co-ordinates to have a -1 to 1 range
	DX::XMFLOAT2 mousePoint(((2.0f * (float)mpMouseScreenPos->mPosX) / 1584.0f) - 1.0f,
		(((2.0f * (float)mpMouseScreenPos->mPosY) / 862.0f) - 1.0f) * -1.0f);

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
	mFntDebug->Draw(strStream.str(), 5, 615, kWhite, kLeft, kTop);
	strStream.str("");

	// Draw mouse grid co-ordinates
	strStream << "X: " << mMouseGridPos.mPosX << "  Y: " << mMouseGridPos.mPosY;
	mFntDebug->Draw(strStream.str(), 5, 630, kWhite, kLeft, kTop);
	strStream.str("");

	// Draw mouse grid co-ordinates
	strStream << "Cuurent pop " << mpHumanPlayer->GetCurrentPop() << "/" << mpHumanPlayer->GetCurrentPopLimit();
	mFntDebug->Draw(strStream.str(), 500, 630, kWhite, kLeft, kTop);
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
	case MS_OUT_OF_GRID: 
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
		mFntDebug->Draw(strStream.str(), 5, 645, kWhite, kLeft, kTop);
		strStream.str("");
	}
}

EMouseStates CWorldState::UpdateMouseState()
{
	// Check if on UI
	if (mpMouseScreenPos->mPosY > 685)
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

	return MS_OUT_OF_GRID;
}

void CWorldState::CheckKeyPresses()
{
	// CHECK FOR SCROLLING
	//------------------------------
	gMouseWheelDelta = gpEngine->GetMouseWheelMovement() * -1.0f;
	if (gMouseWheelDelta != 0)
	{
		float moveAmount = gFrameTime * CAM_SCROLL_SPEED * gMouseWheelDelta;
		mpCamCurrent->SetY(Clampf(CAM_MIN_HEIGHT, CAM_MAX_HEIGHT, mpCamCurrent->GetY() + moveAmount));
		gMouseWheelDelta = 0;
	}
	

	// CLICKING
	//------------------------------
	// Left Click = place currently selected building
	if (mLMouseClicked)
	{
		// Assume nothing is clicked on - reset all pointers (except PlacingStructure)
		mpCurSelectedStructure = nullptr;

		// Check if placing a structure
		if (mpPlacingStructure)
		{
			switch (mMouseState)
			{
				case MS_EARTH_GRID:
					// Place the structure - check if successful
					if (mpHumanPlayer->PurchaseStructure(mpPlacingStructure, mpEarthGrid, mpCurTile))
					{
						// Safe to point at nothing due to structure pointer passed on to Player's data
						mpPlacingStructure = nullptr;
						mpEarthGrid->ResetTilesModels();
						mLMouseClicked = false;
					}
					break;

				case MS_MARS_GRID:
					// Place the structure - check if successful
					if (mpAIPlayer->PurchaseStructure(mpPlacingStructure, mpMarsGrid, mpCurTile))
					{
						// Safe to point at nothing due to structure pointer passed on to Player's data
						mpPlacingStructure = nullptr;
						mpMarsGrid->ResetTilesModels();
						mLMouseClicked = false;
					}
					break;
			}
		}
		else
		{
			// Not placing a structure - find out where they are clicking
			switch (mMouseState)
			{
			case MS_EARTH_GRID:
				mpHumanPlayer->CheckGameObjectSelection(mpCurSelectedStructure, mpCurSelectedAgent,
					mMouseOrigin, mMouseDirection);
				OnStructureSelectChange();
				
				break;

			case MS_MARS_GRID:
				mpAIPlayer->CheckGameObjectSelection(mpCurSelectedStructure, mpCurSelectedAgent,
					mMouseOrigin, mMouseDirection);
				OnStructureSelectChange();
				break;
			}

			if (mpCurSelectedStructure || mpCurSelectedAgent)
			{
				mLMouseClicked = false;
			}
		}
	}

	if (mRMouseClicked)
	{
		if (mpCurSelectedAgent)
		{
			if (!mpCurTile->IsTileUsed())
			{
				mpCurSelectedAgent->SetPathTarget(mMouseWorldPos);
				mRMouseClicked = false;
			}
		}
	}
	mLMouseClicked = false;
	mRMouseClicked = false;

	// Check if a building is currently selected
	if (!mpCurSelectedStructure && !mpCurSelectedAgent)
	{
		// BUILDING PLACEMENT
		//------------------------------
		CStructure* pStructure;

		// C = no building is selected
		if (gpEngine->KeyHit(Mouse_RButton))
		{
			pStructure = nullptr;
			OnPlacingStructureChange(pStructure);
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
	if (gpEngine->KeyHit(Mouse_RButton))
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

		// Ensure no buildings can be brought over
		OnPlacingStructureChange(nullptr);
		mpCurSelectedAgent = nullptr;
		mpCurSelectedStructure = nullptr;
	}


	// TEST FUNCTIONS FOR SPACE
	//------------------------------

	//temporary, just used to test the transfering of ships to the fleet
	if (gpEngine->KeyHit(Key_R))
	{
		mpHumanPlayer->LaunchAttack();
		mpAIPlayer->LaunchAttack();
		gCurState = GS_SPACE;
	}
	
	if (gpEngine->KeyHit(Key_T))
	{
		mpHumanPlayer->GetFleet()->SetTactic(None);
	}

	if (gpEngine->KeyHit(Key_Y))
	{
		mpHumanPlayer->GetFleet()->SetTactic(Targeted);
	}

	if (gpEngine->KeyHit(Key_U))
	{
		mpHumanPlayer->GetFleet()->SetTactic(Rapid);
	}

	// RETURN TO MENU
	//------------------------------
	if (gpEngine->KeyHit(Key_M))
	{
		mMusic->StopSound();
		gCurState = GS_MAIN_MENU;
	}
}

void CWorldState::DisplaySelectedBuildingInfo()
{
	int newProgressAmount = 0;
	int newHealthAmount = -5;
	
	// If an object is selected, display its info
	if (mpCurSelectedStructure)
	{
		// Show building & unit related buttons
		mpButtonDelete->Show();
		
		switch (mpCurSelectedStructure->GetStructureType())
		{
		case STR_BARRACKS:
			// Hide other buildings' buttons
			mpHellipadButtons->Hide();
			mpSpaceCentreButtons->Hide();
			mpComCentreButtons->Hide();

			// Show this building's buttons
			mpBarracksButtons->Show();
			break;

		case STR_COM_CENTRE:
			// Hide other buildings' buttons
			mpBarracksButtons->Hide();
			mpHellipadButtons->Hide();
			mpSpaceCentreButtons->Hide();

			// Show this building's buttons
			mpComCentreButtons->Show();
			break;

		case STR_HELLIPAD:
			// Hide other buildings' buttons
			mpBarracksButtons->Hide();
			mpSpaceCentreButtons->Hide();
			mpComCentreButtons->Hide();

			// Show this building's buttons
			mpHellipadButtons->Show();
			break;

		case STR_SPACE_CENTRE:
			// Hide other buildings' buttons
			mpBarracksButtons->Hide();
			mpHellipadButtons->Hide();
			mpComCentreButtons->Hide();

			// Show this building's buttons
			mpSpaceCentreButtons->Show();
			break;
		}


		// BUILDING DESTRUCTION
		//------------------------------
		if (gpEngine->KeyHit(Key_D))
		{
			// Set object to be deleted
			mpCurSelectedStructure->SetState(OBJ_DEAD);
			// pointer set to null
			mpCurSelectedStructure = nullptr;
			// Leave function so next function call is not executed
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
			mFntDebug->Draw(strStream.str(), 1130, 800, kWhite, kRight, kTop);
			strStream.str("");
		}
		else
		{
			newHealthAmount = 100;
		}

		// Hide building construction buttons
		mpButtonBarracks->Hide();
		mpButtonHellipad->Hide();
		mpButtonSpaceCentre->Hide();
	}
	else
	{
		// Hide building & unit related buttons
		mpButtonDelete->Hide();
		mpBarracksButtons->Hide();
		mpHellipadButtons->Hide();
		mpSpaceCentreButtons->Hide();
		mpComCentreButtons->Hide();

		// Show building construction buttons
		mpButtonBarracks->Show();
		mpButtonHellipad->Show();
		mpButtonSpaceCentre->Show();

		// Nothing is selected - ensure newProgressAmount is 0
		newProgressAmount = 0;

		// If there is also no unit or resource selected, set new health amount to default
		if (mpCurSelectedAgent)
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
	
	// If an object is selected, display its info
	if (mpCurSelectedAgent)
	{
		mpButtonDelete->Show();
		mpHellipadButtons->Hide();
		mpSpaceCentreButtons->Hide();
		mpComCentreButtons->Hide();
		mpBarracksButtons->Hide();

		// Show this building's buttons
		// BUILDING DESTRUCTION
		//------------------------------
		if (gpEngine->KeyHit(Key_D))
		{
			// Set object to be deleted
			mpCurSelectedAgent->SetAgentState(OBJ_DEAD);
			// pointer set to null
			mpCurSelectedAgent = nullptr;
			// Leave function so next function call is not executed
			return;
		}

		mpCurSelectedAgent->DisplayInfo(mFntDebug);

		// Hide building construction buttons
		mpButtonBarracks->Hide();
		mpButtonHellipad->Hide();
		mpButtonSpaceCentre->Hide();

		// Calculate new health - temporarily set to 100
		newHealthAmount = 100;
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

	// Shrink the rectangle to not include side bars and window bar
	mWindowClip.top += 30;
	mWindowClip.left += 8;
	mWindowClip.right -= 8;
	mWindowClip.bottom -= 8;

	// Set the cursor's limits
	//ClipCursor(&mWindowClip);
	mLMouseClicked = false;
	mRMouseClicked = false;


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


	// PLAYERS
	//-----------------------------
	mpPlayerManager = CStateControl::GetInstance()->GetPlayerManager();
	mpHumanPlayer = mpPlayerManager->GetHumanPlayer();
	mpAIPlayer = mpPlayerManager->GetAIPlayer(0);


	// INITIALISE NULL TILE
	//-----------------------------
	mpNullTile = new CTile();
	mpNullTile->SetWorldPos(DX::XMFLOAT3(-2000.0f, 0.0f, 0.0f));


	// INITIALISE USER INTERFACE
	//-----------------------------
	mFntDebug = gpEngine->LoadFont("Calibri", 20U);
	mpMainUI = gpEngine->CreateSprite("WorldUI.png", 0.0f, 0.0f, 0.9f);
	CAdvancedButton<CWorldState, void>* pNewButton = nullptr;

	pNewButton = new CAdvancedButton<CWorldState, void>("DefBarracksButton.png", "SelBarracksButton.png", SPointData(1219, 695),
		SAABoundingBox(772.0f, 1322.0f, 695.0f, 1219.0f), *this, &CWorldState::CreateBarracks);
	mpButtonBarracks = pNewButton;
	mpGenericButtonList.push_back(pNewButton);

	pNewButton = new CAdvancedButton<CWorldState, void>("DefHellipadButton.png", "SelHellipadButton.png", SPointData(1219, 782),
		SAABoundingBox(879.0f, 1322.0f, 782.0f, 1219.0f), *this, &CWorldState::CreateHellipad);
	mpButtonHellipad = pNewButton;
	mpGenericButtonList.push_back(pNewButton);

	pNewButton = new CAdvancedButton<CWorldState, void>("DefSpaceCentreButton.png", "SelSpaceCentreButton.png", SPointData(1342, 695),
		SAABoundingBox(772.0f, 1439.0f, 695.0f, 1342.0f), *this, &CWorldState::CreateSpaceCentre);
	mpButtonSpaceCentre = pNewButton;
	mpGenericButtonList.push_back(pNewButton);


	pNewButton = new CAdvancedButton<CWorldState, void>("DefDeleteButton.png", "SelDeleteButton.png", SPointData(1465, 782),
		SAABoundingBox(879.0f, 1542.0f, 782.0f, 1465.0f), *this, &CWorldState::DeleteStructure);
	pNewButton->Hide();
	mpButtonDelete = pNewButton;
	mpGenericButtonList.push_back(pNewButton);

	// Barracks units buttons
	mpBarracksButtons = new SStructureButtons<CWorldState>(3);
	mpBarracksButtons->mpButtons[0] = new CAdvancedButton<CWorldState, void, int>("DefInfantryButton.png", "SelInfantryButton.png", SPointData(1219, 695),
		SAABoundingBox(772.0f, 1322.0f, 695.0f, 1219.0f), *this, &CWorldState::QueueUnit);
	mpBarracksButtons->mpButtons[1] = new CAdvancedButton<CWorldState, void, int>("DefArtilleryButton.png", "SelArtilleryButton.png", SPointData(1219, 782),
		SAABoundingBox(879.0f, 1322.0f, 782.0f, 1219.0f), *this, &CWorldState::QueueUnit);
	mpBarracksButtons->mpButtons[2] = new CAdvancedButton<CWorldState, void, int>("DefTankButton.png", "SelTankButton.png", SPointData(1342, 695),
		SAABoundingBox(772.0f, 1439.0f, 695.0f, 1342.0f), *this, &CWorldState::QueueUnit);
	mpBarracksButtons->Hide();

	for (int i = 0; i < mpBarracksButtons->mNumButtons; i++)
	{
		mpUnitsButtonList.push_back(mpBarracksButtons->mpButtons[i]);
	}
	
	// hellipad units buttons
	mpHellipadButtons = new SStructureButtons<CWorldState>(2);
	mpHellipadButtons->mpButtons[0] = new CAdvancedButton<CWorldState, void, int>("DefFighterButton.png", "SelFighterButton.png", SPointData(1219, 695),
		SAABoundingBox(772.0f, 1322.0f, 695.0f, 1219.0f), *this, &CWorldState::QueueUnit);
	mpHellipadButtons->mpButtons[1] = new CAdvancedButton<CWorldState, void, int>("DefBomberButton.png", "SelBomberButton.png", SPointData(1219, 782),
		SAABoundingBox(879.0f, 1322.0f, 782.0f, 1219.0f), *this, &CWorldState::QueueUnit);

	for (int i = 0; i < mpHellipadButtons->mNumButtons; i++)
	{
		mpUnitsButtonList.push_back(mpHellipadButtons->mpButtons[i]);
	}

	// Space centre units buttons
	mpSpaceCentreButtons = new SStructureButtons<CWorldState>(3);
	mpSpaceCentreButtons->mpButtons[0] = new CAdvancedButton<CWorldState, void, int>("DefSpaceFighterButton.png", "SelSpaceFighterButton.png", SPointData(1219, 695),
		SAABoundingBox(772.0f, 1322.0f, 695.0f, 1219.0f), *this, &CWorldState::QueueUnit);
	mpSpaceCentreButtons->mpButtons[1] = new CAdvancedButton<CWorldState, void, int>("DefTransportButton.png", "SelTransportButton.png", SPointData(1219, 782),
		SAABoundingBox(879.0f, 1322.0f, 782.0f, 1219.0f), *this, &CWorldState::QueueUnit);
	mpSpaceCentreButtons->mpButtons[2] = new CAdvancedButton<CWorldState, void, int>("DefMothershipButton.png", "SelMothershipButton.png", SPointData(1342, 695),
		SAABoundingBox(772.0f, 1439.0f, 695.0f, 1342.0f), *this, &CWorldState::QueueUnit);
	mpSpaceCentreButtons->Hide();

	for (int i = 0; i < mpSpaceCentreButtons->mNumButtons; i++)
	{
		mpUnitsButtonList.push_back(mpSpaceCentreButtons->mpButtons[i]);
	}

	// Command centre units buttons
	mpComCentreButtons = new SStructureButtons<CWorldState>(1);
	mpComCentreButtons->mpButtons[0] = new CAdvancedButton<CWorldState, void, int>("DefWorkerButton.png", "SelWorkerButton.png", SPointData(1219, 695),
		SAABoundingBox(772.0f, 1322.0f, 695.0f, 1219.0f), *this, &CWorldState::QueueUnit);
	mpComCentreButtons->Hide();

	for (int i = 0; i < mpComCentreButtons->mNumButtons; i++)
	{
		mpUnitsButtonList.push_back(mpComCentreButtons->mpButtons[i]);
	}


	// Queue buttons
	mpQueueButtons = new SStructureButtons<CWorldState>(5);
	mpQueueButtons->mpButtons[0] = new CAdvancedButton<CWorldState, void, int>(SPointData(5, 5), SAABoundingBox(82.0f, 108.0f, 5.0f, 5.0f), 
		*this, &CWorldState::UnqueueUnit);
	mpQueueButtons->mpButtons[1] = new CAdvancedButton<CWorldState, void, int>(SPointData(118, 5), SAABoundingBox(82.0f, 221.0f, 5.0f, 118.0f),
		*this, &CWorldState::UnqueueUnit);
	mpQueueButtons->mpButtons[2] = new CAdvancedButton<CWorldState, void, int>(SPointData(231, 5), SAABoundingBox(82.0f, 334.0f, 5.0f, 231.0f),
		*this, &CWorldState::UnqueueUnit);
	mpQueueButtons->mpButtons[3] = new CAdvancedButton<CWorldState, void, int>(SPointData(344, 5), SAABoundingBox(82.0f, 447.0f, 5.0f, 344.0f),
		*this, &CWorldState::UnqueueUnit);
	mpQueueButtons->mpButtons[4] = new CAdvancedButton<CWorldState, void, int>(SPointData(457, 5), SAABoundingBox(82.0f, 560.0f, 5.0f, 457.0f),
		*this, &CWorldState::UnqueueUnit);

	for (int i = 0; i < mpQueueButtons->mNumButtons; i++)
	{
		mpUnitsButtonList.push_back(mpQueueButtons->mpButtons[i]);
	}

	mpSprQProg = nullptr;
	mQueuePrevProg = 0;


	// Space Buttons
	mpSpaceAtaackButtons = new CAdvancedButton<CWorldState, void>("AttackButton.png", "AttackButtonMO.png", SPointData(10, 695), SAABoundingBox(773.0f, 113.0f, 695.0f, 10.0f),
		*this, &CWorldState::LaunchAttack);
	mpGenericButtonList.push_back(mpSpaceAtaackButtons);

	mpSpaceTacNoneButton = new CAdvancedButton<CWorldState, void>("NoTactics.png", "NoTacticsMO.png", SPointData(138, 695), SAABoundingBox(773.0f, 246.0f, 695.0f, 138.0f),
		*this, &CWorldState::ChangeTacNone);
	mpGenericButtonList.push_back(mpSpaceTacNoneButton);

	mpSpaceTacTargetedButton = new CAdvancedButton<CWorldState, void>("TargetButton.png", "TargetButtonMO.png", SPointData(265, 695), SAABoundingBox(773.0f, 369.0f, 695.0f, 265.0f),
		*this, &CWorldState::ChangeTacTargated);
	mpGenericButtonList.push_back(mpSpaceTacTargetedButton);

	mpSpaceTacRapidButton = new CAdvancedButton<CWorldState, void>("DefRapidFireButton.png", "SelRapidFireButton.png", SPointData(10, 783), SAABoundingBox(860.0f, 113.0f, 783.0f, 10.0f),
		*this, &CWorldState::ChangeTacRapid);
	mpGenericButtonList.push_back(mpSpaceTacRapidButton);

	
	// Health bar variables
	mpSprHealth = nullptr;
	mPrevHealth = 0;


	// CONSTRUCT COMMAND CENTRES
	//-----------------------------
	// if players have already been initialised, this is not necessary
	if (!mpPlayerManager->ArePlayersInitialised())
	{
		// INITIALISE WORLDS
		//-----------------------------
		// EARTH
		mpEarthGrid = new CGrid(DX::XMFLOAT3(0.0f, 0.3f, 0.0f));
		
		DX::XMFLOAT3 gridCentre = mpEarthGrid->GetGridCentrePos();
		mpMdlSkybox->SetPosition(gridCentre.x, -1.0f, gridCentre.z);

		mpMshGridArea = gpEngine->LoadMesh("Grid.x");
		mpMdlEarthGridArea = mpMshGridArea->CreateModel(gridCentre.x, 0.2f, gridCentre.z);
		mpMdlEarthGridArea->ScaleX((GRID_SIZE_X * GRID_TILE_SIZE) / 2.0f);
		mpMdlEarthGridArea->ScaleZ((GRID_SIZE_Y * GRID_TILE_SIZE) / 2.0f);

		mpMshGrassArea = gpEngine->LoadMesh("Grass.x");
		mpMdlEarthGrassArea = mpMshGrassArea->CreateModel(gridCentre.x, 0.1f, gridCentre.z);
		mpMdlEarthGrassArea->ScaleX(GRID_SIZE_X * GRID_TILE_SIZE * 2.0f);
		mpMdlEarthGrassArea->ScaleZ(GRID_SIZE_Y * GRID_TILE_SIZE * 2.0f);

		// MARS
		float marsXStart = (float)(GRID_SIZE_X * GRID_TILE_SIZE) + 1500.0f;
		mpMarsGrid = new CGrid(DX::XMFLOAT3(marsXStart, 0.3f, 0.0f));

		gridCentre = mpMarsGrid->GetGridCentrePos();

		mpMdlMarsGridArea = mpMshGridArea->CreateModel(gridCentre.x, 0.2f, gridCentre.z);
		mpMdlMarsGridArea->ScaleX((GRID_SIZE_X * GRID_TILE_SIZE) / 2.0f);
		mpMdlMarsGridArea->ScaleZ((GRID_SIZE_Y * GRID_TILE_SIZE) / 2.0f);

		mpMdlMarsGrassArea = mpMshGrassArea->CreateModel(gridCentre.x, 0.1f, gridCentre.z);
		mpMdlMarsGrassArea->ScaleX(GRID_SIZE_X * GRID_TILE_SIZE * 2.0f);
		mpMdlMarsGrassArea->ScaleZ(GRID_SIZE_Y * GRID_TILE_SIZE * 2.0f);
		mpMdlMarsGrassArea->SetSkin("sand.jpg");
		
		
		// EARTH
		mpPlacingStructure = nullptr;
		CStructure* pTemp = new CComCentre();
		mpCurTile = mpEarthGrid->GetTileData(SPointData((int)(GRID_SIZE_X / 2.0f), (int)(GRID_SIZE_Y / 2.0f)));
		OnPlacingStructureChange(pTemp);

		if (mpHumanPlayer->PurchaseStructure(mpPlacingStructure, mpEarthGrid, mpCurTile))
		{
			mpPlacingStructure = nullptr;
			mpEarthGrid->ResetTilesModels();
		}

		// MARS
		mpPlacingStructure = nullptr;
		pTemp = new CComCentre();
		mpCurTile = mpMarsGrid->GetTileData(SPointData((int)(GRID_SIZE_X / 2.0f), (int)(GRID_SIZE_Y / 2.0f)));
		OnPlacingStructureChange(pTemp);

		if (mpAIPlayer->PurchaseStructure(mpPlacingStructure, mpMarsGrid, mpCurTile))
		{
			mpPlacingStructure = nullptr;
			mpEarthGrid->ResetTilesModels();
		}
		mpPlacingStructure = nullptr;

		// Set players to initialised
		mpPlayerManager->PlayersInitialised();

		// Set grids for each player for easy referal
		mpHumanPlayer->StorePlayerGridState(mpEarthGrid);
		mpAIPlayer->StorePlayerGridState(mpMarsGrid);
	}
	else
	{
		// RE-CONSTRUCT BUILDINGS
		//-----------------------------
		mpHumanPlayer->LoadStructureModels();
		mpAIPlayer->LoadStructureModels();

		mpHumanPlayer->LoadUnitModels();
		mpAIPlayer->LoadUnitModels();

		// Re-assign previous grid data
		mpEarthGrid = mpHumanPlayer->GetPlayerGrid();
		mpMarsGrid = mpAIPlayer->GetPlayerGrid();
	}


	// INITIALISE CAMERAS
	//-----------------------------
	mpCamEarth = gpEngine->CreateCamera(kManual, mpEarthGrid->GetGridCentrePos().x, 230.0f, (float)GRID_SIZE_Y);
	mpCamEarth->RotateX(50.0f);
	mpCamEarth->SetNearClip(NEAR_CLIP);
	mpCamEarth->SetFarClip(FAR_CLIP);

	mpCamMars = gpEngine->CreateCamera(kManual, mpMarsGrid->GetGridCentrePos().x, 230.0f, (float)GRID_SIZE_Y);
	mpCamMars->RotateX(50.0f);
	mpCamMars->SetNearClip(NEAR_CLIP);
	mpCamMars->SetFarClip(FAR_CLIP);

	mpCamCurrent = mpCamEarth;

	// INITIALISE MUSIC
	//-----------------------------
	string mMusicFile = "Perpetual Tension.wav"; //Sets the music file
	DX::XMFLOAT3 mSourcePos = { mpCamEarth->GetX(), mpCamEarth->GetY(), mpCamEarth->GetZ()};
	DX::XMFLOAT3 mSourceVel = { 0.0f, 0.0f, 0.0f };
	DX::XMFLOAT3 listenerPos = { mpCamEarth->GetX(), mpCamEarth->GetY(), mpCamEarth->GetZ()};
	DX::XMFLOAT3 listenerVel = { 0.0f, 0.0f, 0.0f };
	mMusic = new CSound(mMusicFile, mSourcePos, mSourceVel, true, 1.0f, listenerPos, listenerVel); //Initialise music
	mMusic->PlaySound(); //Play music on loop
}

void CWorldState::StateUpdate()
{
	// SCENE DRAW
	//------------------------------
	gpEngine->DrawScene(mpCamCurrent);
	

	// MOUSE TRACKING
	//------------------------------
	// Update mouse position
	mpMouseScreenPos->mPosX = gpEngine->GetMouseX();
	mpMouseScreenPos->mPosY = gpEngine->GetMouseY();

	// Check for side scrolling
	if (mpMouseScreenPos->mPosX < EDGE_THRESHOLD)
	{
		// Mouse on left side of screen
		mpCamCurrent->MoveX(-CAM_MOVE_SPEED * gFrameTime);
	}
	if (mpMouseScreenPos->mPosX > WINDOW_WIDTH - EDGE_THRESHOLD)
	{
		// Mouse on right side of screen
		mpCamCurrent->MoveX(CAM_MOVE_SPEED * gFrameTime);
	}
	if (mpMouseScreenPos->mPosY < EDGE_THRESHOLD)
	{
		// Mouse on top side of screen
		mpCamCurrent->MoveZ(CAM_MOVE_SPEED * gFrameTime);
	}
	if (mpMouseScreenPos->mPosY > WINDOW_HEIGHT - EDGE_THRESHOLD)
	{
		// Mouse on bottom side of screen
		mpCamCurrent->MoveZ(-CAM_MOVE_SPEED * gFrameTime);
	}


	// METHODS
	//---------------------------
	UpdateMatrices();
	CalculateMouseGridPos();
	mMouseState = UpdateMouseState();
	DrawFontData();
	DisplaySelectedBuildingInfo();
	if (mpCurSelectedAgent != nullptr)
	{
		DisplaySelectedAgentInfo();
	}

	// Update the current queue size ( if a structure is slected 0. Returns true if a change has occured
	if (CheckForQueueChange())
	{
		// Update the buttons for the queue
		OnStructureSelectChange();
	}


	// BUTTON UPDATES
	//---------------------------
	if (gpEngine->KeyHit(Mouse_LButton))
	{
		// Raise click flag
		mLMouseClicked = true;
	}

	if (gpEngine->KeyHit(Mouse_RButton))
	{
		mRMouseClicked = true;
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

	// Loop through key presses
	CheckKeyPresses();


	// MODEL UPDATES
	//---------------------------
	if (mMouseGridPos.mPosX != mMousePrevGridPos.mPosX || mMouseGridPos.mPosY != mMousePrevGridPos.mPosY)
	{
		UpdateHeldStructure();
		mMousePrevGridPos = mMouseGridPos;
	}

	for (auto it = mpHumanPlayer->GetWorldUnitList()->begin(); it != mpHumanPlayer->GetWorldUnitList()->end(); it++)
	{
		if (it->second->HasTarget())
		{
			//it->second->Update();
			//mpEarthGrid->GetTileData(mGr  it->second->GetPathTarget()->GetGridPos())->SetTileUsage(true);
		}
	}

	// UPDATE PLAYERS
	//------------------------------
	mpPlayerManager->UpdatePlayers();
	


	// STATE CHANGE TEST
	//------------------------------
	if (gpEngine->KeyHit(Key_Back))
	{
		gCurState = GS_MAIN_MENU;
	}
}

void CWorldState::StateLoad()
{

}

void CWorldState::StateSave()
{

}

void CWorldState::StateCleanup()
{
	// Store grid state
	mpHumanPlayer->StorePlayerGridState(mpEarthGrid);
	mpAIPlayer->StorePlayerGridState(mpMarsGrid);

	// Unclip cursor
	ClipCursor(&mBaseClip);
	
	SafeDelete(mpMouseScreenPos);

	mpMshSkybox->RemoveModel(mpMdlSkybox);

	//used to unload the structure models
	mpHumanPlayer->UnloadStructureModels();
	mpAIPlayer->UnloadStructureModels();

	mpHumanPlayer->UnloadUnitModels();
	mpAIPlayer->UnloadUnitModels();

	gpEngine->RemoveSprite(mpMainUI);
	gpEngine->RemoveMesh(mpMshSkybox);
	gpEngine->RemoveCamera(mpCamEarth);

	if (mpSprQProg)
	{
		gpEngine->RemoveSprite(mpSprQProg);
	}

	if (mpSprHealth)
	{
		gpEngine->RemoveSprite(mpSprHealth);
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

void CWorldState::OnStructureSelectChange()
{
	// Check if something is slelected
	if (mpCurSelectedStructure)
	{
		// Unload the existing queue buttons
		mpQueueButtons->UnloadSprites();
		
		std::deque<CGameAgent*>* pQueue = nullptr;
		std::deque<CGameAgent*>::iterator iterQ;

		// Get the pointer to the queue
		pQueue = mpCurSelectedStructure->GetQueue();

		// Loop through the queue to create the buttons for each of the units in the queue
		int i = 0;
		for (iterQ = pQueue->begin(); iterQ != pQueue->end(); iterQ++)
		{
			switch ((*iterQ)->GetAgentData()->mAgentType)
			{
			case GAV_ARTILLERY:
				mpQueueButtons->mpButtons[i]->LoadButtons("DefArtilleryButton.png", "SelArtilleryButton.png");
				break;

			case GAV_BOMBER:
				mpQueueButtons->mpButtons[i]->LoadButtons("DefBomberButton.png", "SelBomberButton.png");
				break;

			case GAV_FIGHTER:
				mpQueueButtons->mpButtons[i]->LoadButtons("DefFighterButton.png", "SelFighterButton.png");
				break;

			case GAV_INFANTRY:
				mpQueueButtons->mpButtons[i]->LoadButtons("DefInfantryButton.png", "SelInfantryButton.png");
				break;

			case GAV_MOTHERSHIP:
				mpQueueButtons->mpButtons[i]->LoadButtons("DefMothershipButton.png", "SelmothershipButton.png");
				break;

			case GAV_SPACE_FIGHTER:
				mpQueueButtons->mpButtons[i]->LoadButtons("DefSpaceFighterButton.png", "SelSpaceFighterButton.png");
				break;

			case GAV_TANK:
				mpQueueButtons->mpButtons[i]->LoadButtons("DefTankButton.png", "SelTankButton.png");
				break;

			case GAV_TRANSPORT:
				mpQueueButtons->mpButtons[i]->LoadButtons("DefTransportButton.png", "SelTransportButton.png");
				break;

			case GAV_WORKER:
				mpQueueButtons->mpButtons[i]->LoadButtons("DefWorkerButton.png", "SelWorkerButton.png");
				break;
			}

			i++;
		}
	}
	else
	{
		// Nothing selected - unload sprites
		mpQueueButtons->UnloadSprites();
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
	mpSprHealth = gpEngine->CreateSprite(strStream.str(), 450.0f, 780.0f, 0.5f);
	strStream.str("");
}


//-----------------------------------------------------
// WORLD STATE CLASS BUTTON EVENT FUNCTIONS
//-----------------------------------------------------
void CWorldState::QueueUnit(int index)
{
	if (!mpCurSelectedStructure) return;
	
	mpCurSelectedStructure->AddToQueue(index, mpHumanPlayer);
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

void CWorldState::DeleteStructure()
{
	if (!mpCurSelectedStructure) return;
	
	// Set object to be deleted
	mpCurSelectedStructure->SetState(OBJ_DEAD);
	// pointer set to null
	mpCurSelectedStructure = nullptr;
	// Leave function so next function call is not executed
	return;
	mLMouseClicked = false;
}

void CWorldState::LaunchAttack()
{
	mpHumanPlayer->LaunchAttack();
	mpAIPlayer->LaunchAttack();
	gCurState = GS_SPACE;
}


void CWorldState::ChangeTacNone()
{
	mpHumanPlayer->GetFleet()->SetTactic(None);
}

void CWorldState::ChangeTacRapid()
{
	mpHumanPlayer->GetFleet()->SetTactic(Rapid);
}

void CWorldState::ChangeTacTargated()
{
	mpHumanPlayer->GetFleet()->SetTactic(Targeted);
}
