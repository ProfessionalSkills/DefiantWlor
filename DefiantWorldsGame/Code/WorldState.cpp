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

	DX::XMVECTOR intersecVec = DX::XMPlaneIntersectLine(DX::XMLoadFloat3(&mYPlane), rayOrigin, rayEnd);
	DX::XMStoreFloat3(&mMouseWorldPos, intersecVec);		// Store the mouse's world position at y = 0

	// Use the mouse's world position to determine the grid position
	mMouseGridPos.mPosX = (mMouseWorldPos.x - mCurGridPos.x) / GRID_TILE_SIZE;
	mMouseGridPos.mPosY = (mMouseWorldPos.z - mCurGridPos.z) / GRID_TILE_SIZE;
}

void CWorldState::DrawFontData()
{
	// Draw Mouse world co-ordinates to screen
	strStream << "X: " << mMouseWorldPos.x << "  Z: " << mMouseWorldPos.z;
	mFntDebug->Draw(strStream.str(), 5, 5, kWhite, kLeft, kTop);
	strStream.str("");

	// Draw mouse grid co-ordinates
	strStream << "X: " << mMouseGridPos.mPosX << "  Y: " << mMouseGridPos.mPosY;
	mFntDebug->Draw(strStream.str(), 5, 15, kWhite, kLeft, kTop);
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
		mpCurTile = mpMarsGrid->GetTileData(mMouseGridPos);
		strStream << "Mars";
		break;
	case MS_OUT_OF_GRID: 
		mpCurTile = mpNullTile;
		strStream << "None";
		break;
	case MS_UI:
		mpCurTile = mpNullTile;
		strStream << "None";
		break;
	}

	if (mpCurTile)
	{
		strStream << "  USED: " << mpCurTile->IsTileUsed();
		mFntDebug->Draw(strStream.str(), 5, 25, kWhite, kLeft, kTop);
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
	gMouseWheelDelta = gpEngine->GetMouseWheelMovement() * -1;
	if (gMouseWheelDelta != 0)
	{
		float moveAmount = gFrameTime * CAM_SCROLL_SPEED * gMouseWheelDelta;
		mpCamCurrent->SetY(Clampf(CAM_MIN_HEIGHT, CAM_MAX_HEIGHT, mpCamCurrent->GetY() + moveAmount));
		gMouseWheelDelta = 0;
	}
	

	// CLICKING
	//------------------------------
	// Left Click = place currently selected building
	if (gpEngine->KeyHit(Mouse_LButton))
	{
		// Assume nothing is clicked on - reset all pointers (except PlacingStructure)
		mpCurSelectedStructure = nullptr;
		mMouseClicked = true;

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
						mMouseClicked = false;
					}
					break;

				case MS_MARS_GRID:
					// Place the structure - check if successful
					if (mpAIPlayer->PurchaseStructure(mpPlacingStructure, mpMarsGrid, mpCurTile))
					{
						// Safe to point at nothing due to structure pointer passed on to Player's data
						mpPlacingStructure = nullptr;
						mpMarsGrid->ResetTilesModels();
						mMouseClicked = false;
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
				// Check if it's a building
				mpCurSelectedStructure = mpHumanPlayer->CheckStructureSelection(mMouseWorldPos);
				break;

			case MS_MARS_GRID:
				// Check if it's a building
				mpCurSelectedStructure = mpAIPlayer->CheckStructureSelection(mMouseWorldPos);
				break;
			}

			if (mpCurSelectedStructure)
			{
				mMouseClicked = false;
			}
		}
	}


	// Check if a building is currently selected
	if (!mpCurSelectedStructure)
	{
		// BUILDING PLACEMENT
		//------------------------------
		CStructure* pStructure;

		// C = no building is selected
		if (gpEngine->KeyHit(Key_C))
		{
			pStructure = nullptr;
			OnPlacingStructureChange(pStructure);
		}

		// 0 = CommsCentre
		if (gpEngine->KeyHit(Key_0))
		{
			pStructure = new CComCentre();
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
	/*
	//test to return fleet
	if (gpEngine->KeyHit(Key_T))
	{
		CFleet* test = mpHumanPlayer->GetFleet();

		mpHumanPlayer->SetSpaceUnitList(test->ReturnFleet(mpHumanPlayer->GetSpaceUnitList()));
	}

	//temp functions to test space setup
	if (gpEngine->KeyHit(Key_Y))
	{
		CFleet* mpPlayerOneFleet = mpHumanPlayer->GetFleet();
		CFleet* mpPlayerTwoFleet = mpAIPlayer->GetFleet();

		mpPlayerOneFleet->SetEnemy(mpPlayerTwoFleet);
		mpPlayerTwoFleet->SetEnemy(mpPlayerOneFleet);
	}

	//temp function to test space fighting
	if (gpEngine->KeyHit(Key_U))
	{
		CFleet* mpPlayerOneFleet = mpHumanPlayer->GetFleet();
		CFleet* mpPlayerTwoFleet = mpAIPlayer->GetFleet();
		for (int i = 0; i < 1000; i++)
		{
			//fleets attack each other according to tactics
			mpPlayerOneFleet->Fight();
			mpPlayerTwoFleet->Fight();

			//finds and removes dead ships
			mpPlayerOneFleet->UpdateCondition();
			mpPlayerTwoFleet->UpdateCondition();
		}
		//mpPlayerOneFleet = 0; //used to put a break point at the end of the function
	}

	*/

	// RETURN TO MENU TEST
	//------------------------------
	if (gpEngine->KeyHit(Key_M))
	{
		mMusic->StopSound();
		gCurState = GS_MAIN_MENU;
	}
}

void CWorldState::DisplaySelectedBuildingInfo()
{
	// If an object is selected, display its info
	if (mpCurSelectedStructure)
	{
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

	mpMouseScreenPos = new SPointData();
	mWindowClip = { 0 };
	GetClipCursor(&mBaseClip);
	bool success = GetWindowRect((HWND)gpEngine->GetWindow(), &mWindowClip);

	// Shrink the rectangle to not include side bars and window bar
	mWindowClip.top += 30;
	mWindowClip.left += 8;
	mWindowClip.right -= 8;
	mWindowClip.bottom -= 8;

	// Set the cursor's limits
	//ClipCursor(&mWindowClip);
	mMouseClicked = false;


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


	// INITIALISE WORLDS
	//-----------------------------
	// EARTH
	mpEarthGrid = new CGrid(DX::XMFLOAT3(0.0f, 0.3f, 0.0f));
	mpNullTile = new CTile();
	mpNullTile->SetWorldPos(DX::XMFLOAT3(-2000.0f, 0.0f, 0.0f));

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


	// INITIALISE USER INTERFACE
	//-----------------------------
	mFntDebug = gpEngine->LoadFont("Font2.bmp", 15U);
	mpMainUI = gpEngine->CreateSprite("WorldUI.png", 0.0f, 0.0f, 0.9f);
	CButton* pNewButton = nullptr;

	pNewButton = new CButton("DefBarracksButton.png", "SelBarracksButton.png", SPointData(1219, 695),
		SAABoundingBox(772.0f, 1322.0f, 695.0f, 1219.0f), "Barracks");
	mpButtonList.push_back(pNewButton);

	pNewButton = new CButton("DefHellipadButton.png", "SelHellipadButton.png", SPointData(1219, 782),
		SAABoundingBox(879.0f, 1322.0f, 782.0f, 1219.0f), "Hellipad");
	mpButtonList.push_back(pNewButton);

	pNewButton = new CButton("DefSpaceCentreButton.png", "SelSpaceCentreButton.png", SPointData(1332, 695),
		SAABoundingBox(772.0f, 1429.0f, 695.0f, 1332.0f), "Space Centre");
	mpButtonList.push_back(pNewButton);


	// INITIALISE MUSIC
	//-----------------------------
	string mMusicFile = "Perpetual Tension.wav"; //Sets the music file
	ALfloat mSourcePos[3] = { mpCamEarth->GetX(), mpCamEarth->GetY(), mpCamEarth->GetZ() }; //Music source to camera position
	ALfloat mSourceVel[3] = { 0.0f, 0.0f, 0.0f }; //No veloctiy of source
	mMusic = new CSound(mMusicFile, mSourcePos, mSourceVel); //Initialise music
	mMusic->PlaySound(); //Play music on loop


	// CONSTRUCT COMMAND CENTRES
	//-----------------------------
	// if players have already been initialised, this is not necessary
	if (!mpPlayerManager->ArePlayersInitialised())
	{
		// EARTH
		mpPlacingStructure = nullptr;
		CStructure* pTemp = new CComCentre();
		mpCurTile = mpEarthGrid->GetTileData(SPointData(GRID_SIZE_X / 2.0f, GRID_SIZE_Y / 2.0f));
		OnPlacingStructureChange(pTemp);

		if (mpHumanPlayer->PurchaseStructure(mpPlacingStructure, mpEarthGrid, mpCurTile))
		{
			mpPlacingStructure = nullptr;
			mpEarthGrid->ResetTilesModels();
		}

		// MARS
		mpPlacingStructure = nullptr;
		pTemp = new CComCentre();
		mpCurTile = mpMarsGrid->GetTileData(SPointData(GRID_SIZE_X / 2.0f, GRID_SIZE_Y / 2.0f));
		OnPlacingStructureChange(pTemp);

		if (mpAIPlayer->PurchaseStructure(mpPlacingStructure, mpMarsGrid, mpCurTile))
		{
			mpPlacingStructure = nullptr;
			mpEarthGrid->ResetTilesModels();
		}
		mpPlacingStructure = nullptr;

		// Set players to initialised
		mpPlayerManager->PlayersInitialised();
	}
	else
	{
		// CONSTRUCT BUILDINGS
		//-----------------------------
		mpHumanPlayer->LoadStructureModels();
		mpAIPlayer->LoadStructureModels();
	}
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
	CheckKeyPresses();
	DrawFontData();
	DisplaySelectedBuildingInfo();


	// BUTTON UPDATES
	//---------------------------
	for (miterButtons = mpButtonList.begin(); miterButtons != mpButtonList.end(); miterButtons++)
	{
		// Check if the mouse is colliding with the object
		if ((*miterButtons)->GetBoundingBox().IsColliding(DX::XMFLOAT3(mpMouseScreenPos->mPosX, 0.0f, mpMouseScreenPos->mPosY)))
		{
			(*miterButtons)->SetMouseOver(true);
		}
		else
		{
			(*miterButtons)->SetMouseOver(false);
		}

		// Check for click 
		if ((*miterButtons)->GetMouseOver())
		{
			// Check if the mouse is over the button
			if (mMouseClicked)
			{
				// Raise click flag
				std::string purpose = *(*miterButtons)->GetPurpose();

				if (purpose == "Space Centre")
				{
					CStructure* pStructure = new CSpaceCentre();
					OnPlacingStructureChange(pStructure);
				}
				else if (purpose == "Barracks")
				{
					CStructure* pStructure = new CBarracks();
					OnPlacingStructureChange(pStructure);
				}
				else if (purpose == "Hellipad")
				{
					CStructure* pStructure = new CHellipad();
					OnPlacingStructureChange(pStructure);
				}

				mMouseClicked = false;
			}
		}

		// Update the button
		(*miterButtons)->Update();
	}


	// MODEL UPDATES
	//---------------------------
	if (mMouseGridPos.mPosX != mMousePrevGridPos.mPosX || mMouseGridPos.mPosY != mMousePrevGridPos.mPosY)
	{
		UpdateHeldStructure();
		mMousePrevGridPos = mMouseGridPos;
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
	// Unclip cursor
	ClipCursor(&mBaseClip);
	
	SafeDelete(mpMouseScreenPos);
	SafeDelete(mpEarthGrid);
	SafeDelete(mpMarsGrid);

	mpMshSkybox->RemoveModel(mpMdlSkybox);

	//used to unload the structure models
	mpHumanPlayer->UnloadStructureModels();
	mpAIPlayer->UnloadStructureModels();
	mpHumanPlayer->UnloadUnitModels();

	gpEngine->RemoveSprite(mpMainUI);
	gpEngine->RemoveMesh(mpMshSkybox);
	gpEngine->RemoveCamera(mpCamEarth);

	mMusic->StopSound();

	// Loop through buttons and remove them
	while (!mpButtonList.empty())
	{
		CButton* pTmp = mpButtonList.back();
		SafeDelete(pTmp);
		mpButtonList.pop_back();
	}
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
