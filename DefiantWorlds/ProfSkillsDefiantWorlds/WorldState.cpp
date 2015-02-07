//-----------------------------------------------------
// FILE: WorldState.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "WorldState.h"


//-----------------------------------------------------
// SPACE STATE CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CWorldState::CWorldState() : CPlayState()
{

}

CWorldState::~CWorldState()
{
	
}


//-----------------------------------------------------
// MENU STATE CLASS OVERRIDE METHODS
//-----------------------------------------------------
void CWorldState::StateSetup()
{
	// INITIALISE ADDITIONAL VARIABLES
	//------------------------------
	mpMousePos = new SPointData();
	mWindowClip = { 0 };
	GetClipCursor(&mBaseClip);
	GetWindowRect((HWND)gpEngine->GetWindow(), &mWindowClip);

	// Shrink the rectangle to not include side bars and window bar
	mWindowClip.top += 30;
	mWindowClip.left += 8;
	mWindowClip.right -= 8;
	mWindowClip.bottom -= 8;

	// Set the cursor's limits
	ClipCursor(&mWindowClip);


	// INITIALISE CAMERAS
	//-----------------------------
	mpCamEarth = gpEngine->CreateCamera(kManual, 0.0f, 200.0f, 0.0f);
	mpCamEarth->RotateX(90.0f);
	mpCamEarth->SetFarClip(3000.0f);


	// INITIALISE SKYBOX
	//-----------------------------
	mpMshSkybox = gpEngine->LoadMesh("SkyboxWorld.x");
	mpMdlSkybox = mpMshSkybox->CreateModel(0.0f, 0.0f, 0.0f);
	mpMdlSkybox->Scale(0.5f);


	// INITIALISE WORLDS
	//-----------------------------
	test = gpEngine->LoadMesh("Planet.x");
	mpEarthGrid = new CGrid(DX::XMFLOAT3(0.0f, 0.0f, 0.0f), test);
}

void CWorldState::StateUpdate(const float inDelta)
{
	// SCENE DRAW
	//------------------------------
	gpEngine->DrawScene();
	

	// MOUSE TRACKING
	//------------------------------
	// Update mouse position
	mpMousePos->mPosX = gpEngine->GetMouseX();
	mpMousePos->mPosY = gpEngine->GetMouseY();

	// Check for side scrolling
	if (mpMousePos->mPosX < EDGE_THRESHOLD)
	{
		// Mouse on left side of screen
		mpCamEarth->MoveX(-CAM_MOVE_SPEED * inDelta);
	}
	if (mpMousePos->mPosX > WINDOW_WIDTH - EDGE_THRESHOLD)
	{
		// Mouse on right side of screen
		mpCamEarth->MoveX(CAM_MOVE_SPEED * inDelta);
	}
	if (mpMousePos->mPosY < EDGE_THRESHOLD)
	{
		// Mouse on top side of screen
		mpCamEarth->MoveZ(CAM_MOVE_SPEED * inDelta);
	}
	if (mpMousePos->mPosY > WINDOW_HEIGHT - EDGE_THRESHOLD)
	{
		// Mouse on bottom side of screen
		mpCamEarth->MoveZ(-CAM_MOVE_SPEED * inDelta);
	}


	// UPDATE PLAYERS
	//------------------------------
	mpAIPlayer->Update(inDelta);
	mpHumanPlayer->Update(inDelta);


	// STATE CHANGE TEST
	//------------------------------
	if (gpEngine->KeyHit(Key_Return))
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
	SafeDelete(mpMousePos);

	mpMshSkybox->RemoveModel(mpMdlSkybox);

	gpEngine->RemoveMesh(mpMshSkybox);
	gpEngine->RemoveCamera(mpCamEarth);

	// Unclip cursor
	ClipCursor(&mBaseClip);
}
