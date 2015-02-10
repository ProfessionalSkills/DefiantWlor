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
// MENU STATE CLASS METHODS
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
	DX::XMFLOAT3 gridPos = mpEarthGrid->GetGridStartPos();		// Bottom left position of grid
	mpMouseGridPos->mPosX = (mMouseWorldPos.x - gridPos.x) / GRID_TILE_SIZE;
	mpMouseGridPos->mPosY = (mMouseWorldPos.z - gridPos.z) / GRID_TILE_SIZE;
}

void CWorldState::DrawFontData()
{
	// Draw Mouse world co-ordinates to screen
	strStream << "X: " << mMouseWorldPos.x << "  Z: " << mMouseWorldPos.z;
	mFntDebug->Draw(strStream.str(), 5, 5, kWhite, kLeft, kTop);
	strStream.str("");

	// Draw mouse grid co-ordinates
	strStream << "X: " << mpMouseGridPos->mPosX << "  Y: " << mpMouseGridPos->mPosY;
	mFntDebug->Draw(strStream.str(), 5, 15, kWhite, kLeft, kTop);
	strStream.str("");

	// Draw mouse state
	strStream << "GRID: ";
	switch (mMouseState)
	{
	case MS_EARTH_GRID:
		strStream << "Earth";
		break;
	case MS_MARS_GRID:
		strStream << "Mars";
		break;
	case MS_OUT_OF_GRID: 
		strStream << "None";
		break;
	}

	mFntDebug->Draw(strStream.str(), 5, 25, kWhite, kLeft, kTop);
	strStream.str("");
}

EMouseStates CWorldState::UpdateMouseState()
{
	// Update position of the mouse
	CalculateMouseGridPos();

	// Check whether it is within earth boundary
	if (mMouseWorldPos.x > mpEarthGrid->GetGridStartPos().x && mMouseWorldPos.x < mpEarthGrid->GetGridEndPos().x
		&& mMouseWorldPos.z > mpEarthGrid->GetGridStartPos().z && mMouseWorldPos.z < mpEarthGrid->GetGridEndPos().z)
	{
		return MS_EARTH_GRID;
	}
	else
	{
		return MS_OUT_OF_GRID;
	}
}


//-----------------------------------------------------
// MENU STATE CLASS OVERRIDE METHODS
//-----------------------------------------------------
void CWorldState::StateSetup()
{
	// INITIALISE ADDITIONAL VARIABLES
	//------------------------------
	mpMouseScreenPos = new SPointData();
	mpMouseGridPos = new SPointData();
	mWindowClip = { 0 };
	GetClipCursor(&mBaseClip);
	bool success = GetWindowRect((HWND)gpEngine->GetWindow(), &mWindowClip);

	// Shrink the rectangle to not include side bars and window bar
	mWindowClip.top += 30;
	mWindowClip.left += 8;
	mWindowClip.right -= 8;
	mWindowClip.bottom -= 8;

	// Set the cursor's limits
	ClipCursor(&mWindowClip);


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


	// INITIALISE CAMERAS
	//-----------------------------
	mpCamEarth = gpEngine->CreateCamera(kManual, 0.0f, 120.0f, 0.0f);
	mpCamEarth->RotateX(50.0f);
	mpCamEarth->SetNearClip(NEAR_CLIP);
	mpCamEarth->SetFarClip(FAR_CLIP);

	mpCamCurrent = mpCamEarth;


	// INITIALISE SKYBOX
	//-----------------------------
	mpMshSkybox = gpEngine->LoadMesh("SkyboxWorld.x");
	mpMdlSkybox = mpMshSkybox->CreateModel(0.0f, 0.0f, 0.0f);
	mpMdlSkybox->Scale(0.5f);


	// INITIALISE WORLDS
	//-----------------------------
	mpEarthGrid = new CGrid(DX::XMFLOAT3(0.0f, 0.1f, 0.0f));


	// INITIALISE FONTS
	//-----------------------------
	mFntDebug = gpEngine->LoadFont("Font2.bmp", 15U);


	// INITIALISE MUSIC
	//-----------------------------
	string mMusicFile = "Perpetual Tension.wav"; //Sets the music file
	ALfloat mSourcePos[3] = { mpCamEarth->GetX(), mpCamEarth->GetY(), mpCamEarth->GetZ() }; //Music source to camera position
	ALfloat mSourceVel[3] = { 0.0f, 0.0f, 0.0f }; //No veloctiy of source
	mMusic = new CSound(mMusicFile, mSourcePos, mSourceVel); //Initialise music
	mMusic->PlaySound(); //Play music on loop


	buildTest = gpEngine->LoadMesh("Building09.x");
	mdlBuildTest = buildTest->CreateModel();
}

void CWorldState::StateUpdate(const float inDelta)
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
		mpCamEarth->MoveX(-CAM_MOVE_SPEED * inDelta);
	}
	if (mpMouseScreenPos->mPosX > WINDOW_WIDTH - EDGE_THRESHOLD)
	{
		// Mouse on right side of screen
		mpCamEarth->MoveX(CAM_MOVE_SPEED * inDelta);
	}
	if (mpMouseScreenPos->mPosY < EDGE_THRESHOLD)
	{
		// Mouse on top side of screen
		mpCamEarth->MoveZ(CAM_MOVE_SPEED * inDelta);
	}
	if (mpMouseScreenPos->mPosY > WINDOW_HEIGHT - EDGE_THRESHOLD)
	{
		// Mouse on bottom side of screen
		mpCamEarth->MoveZ(-CAM_MOVE_SPEED * inDelta);
	}


	// METHODS
	//---------------------------
	UpdateMatrices();
	mMouseState = UpdateMouseState();
	DrawFontData();

	mdlBuildTest->SetPosition(mMouseWorldPos.x, mMouseWorldPos.y, mMouseWorldPos.z);



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
	SafeDelete(mpMouseScreenPos);
	SafeDelete(mpMouseGridPos);

	mpMshSkybox->RemoveModel(mpMdlSkybox);

	gpEngine->RemoveMesh(mpMshSkybox);
	gpEngine->RemoveCamera(mpCamEarth);

	// Unclip cursor
	ClipCursor(&mBaseClip);
}
