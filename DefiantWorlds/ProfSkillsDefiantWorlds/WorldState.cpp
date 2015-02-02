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
CWorldState::CWorldState() : IGameState()
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
	mpMousePos = new S2DPointData();
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
	mpCamEarth = gpEngine->CreateCamera(kManual, 0.0f, 500.0f, 0.0f);
	mpCamEarth->RotateX(90.0f);
	mpCamEarth->SetFarClip(30000.0f);


	// INITIALISE SKYBOX
	//-----------------------------
	mpMshSkybox = gpEngine->LoadMesh("SkyboxWorld.x");
	mpMdlSkybox = mpMshSkybox->CreateModel(0.0f, 0.0f, 0.0f);
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
