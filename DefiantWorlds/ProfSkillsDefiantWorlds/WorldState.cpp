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
void CWorldState::UpdateMatrices()
{
	// Store the current camera's model matrix
	mpCamCurrent->GetMatrix(&mCamWorldMatrix.m[0][0]);

	// XMMATRIX variables calculations
	DX::XMMATRIX world = DX::XMLoadFloat4x4(&mCamWorldMatrix);
	DX::XMMATRIX view = DX::XMMatrixInverse(NULL, DX::XMLoadFloat4x4(&mCamWorldMatrix));
	DX::XMMATRIX proj = DX::XMMatrixPerspectiveFovLH(DX::XM_PI / 3.4f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, NEAR_CLIP, FAR_CLIP);
	DX::XMMATRIX viewProj = view * proj;
	DX::XMMATRIX invViewProj = DX::XMMatrixInverse(NULL, viewProj);

	DX::XMStoreFloat4x4(&mCamInvViewProj, invViewProj);
}

void CWorldState::CalculateMouseGridPos()
{
	// Convert mouse co-ordinates to have a -1 to 1 range
	DX::XMFLOAT2 mousePoint(((2.0f * (float)mpMouseScreenPos->mPosX) / (float)WINDOW_WIDTH) - 1.0f,
		(((2.0f * (float)mpMouseScreenPos->mPosY) / (float)WINDOW_HEIGHT) - 1.0f) * -1.0f);

	// Mouse vectors
	DX::XMFLOAT3 nearVec(mousePoint.x, mousePoint.y, 0.0f);
	DX::XMFLOAT3 farVec(mousePoint.x, mousePoint.y, 1.0f);

	// Determine ray points
	DX::XMVECTOR rayOrigin = DX::XMVector3TransformCoord(DX::XMLoadFloat3(&nearVec), DX::XMLoadFloat4x4(&mCamInvViewProj));
	DX::XMVECTOR rayEnd = DX::XMVector3TransformCoord(DX::XMLoadFloat3(&farVec), DX::XMLoadFloat4x4(&mCamInvViewProj));
	//DX::XMVECTOR rayDir = DX::XMVector3Normalize(XMMinusVectors(rayEnd, rayOrigin));
	//DX::XMStoreFloat3(&mCamRayDir, rayDir);
	DX::XMStoreFloat3(&mCamRayOrigin, rayOrigin);
	DX::XMStoreFloat3(&mCamRayEnd, rayEnd);

	// Get distance of line produced
	DX::XMFLOAT3 distVec(mCamRayEnd.x - mCamRayOrigin.x, mCamRayEnd.y - mCamRayOrigin.y, mCamRayEnd.z - mCamRayOrigin.z);
	float dist = sqrtf((distVec.x * distVec.x) + (distVec.y * distVec.y) + (distVec.z * distVec.z));

	// Based on height of camera, get the ratio of line
	float camHeight = mCamRayOrigin.y;
	float ratio = camHeight / dist;

	// Find point that divides segment into ratio (1-ratio):ratio
	// This will calculate the world position of mouse at y = 0
	mMouseWorldPos.x = ratio * mCamRayEnd.x + (1.0f - ratio) * mCamRayOrigin.x;
	mMouseWorldPos.y = ratio * mCamRayEnd.y + (1.0f - ratio) * mCamRayOrigin.y;
	mMouseWorldPos.z = ratio * mCamRayEnd.z + (1.0f - ratio) * mCamRayOrigin.z;
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


	// INITIALISE CAMERAS
	//-----------------------------
	mpCamEarth = gpEngine->CreateCamera(kManual, 0.0f, 10.0f, 0.0f);
	mpCamEarth->RotateX(90.0f);
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
	test = gpEngine->LoadMesh("Planet.x");
	mpEarthGrid = new CGrid(DX::XMFLOAT3(0.0f, 0.0f, 0.0f), test);
	testModel = test->CreateModel();
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
	CalculateMouseGridPos();

	testModel->SetPosition(mMouseWorldPos.x, mMouseWorldPos.y, mMouseWorldPos.z);


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
