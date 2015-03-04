//-----------------------------------------------------
// FILE: SpaceState.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "SpaceState.h"
#include "GameStateControl.h"


//-----------------------------------------------------
// SPACE STATE CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CSpaceState::CSpaceState() :mTimeToUpdate(0.1f), mCamRotSpeed(0.7),mCamZAdjust(-10.0f),mBaseCamZ(-50.0f), mDisplacement(35),CGameState()
{
	mTimeSinceUpdate = 0.0f;
	mCamZ = 0.0f;
}

CSpaceState::~CSpaceState()
{

}


//-----------------------------------------------------
// MENU STATE CLASS OVERRIDE METHODS
//-----------------------------------------------------
void CSpaceState::StateSetup()
{
	// PLAYERS
	//-----------------------------
	mpPlayerManager = CStateControl::GetInstance()->GetPlayerManager();
	mpHumanPlayer = mpPlayerManager->GetHumanPlayer();
	mpAIPlayer = mpPlayerManager->GetAIPlayer(0);
	
	// FLEET SETUP
	//------------------------------
	mpPlayerOneFleet = mpHumanPlayer->GetFleet();
	mpPlayerTwoFleet = mpAIPlayer->GetFleet();

	mpPlayerOneFleet->SetEnemy(mpPlayerTwoFleet);
	mpPlayerTwoFleet->SetEnemy(mpPlayerOneFleet);

	// INITIALISE CAMERAS
	//------------------------------
	//figures out which fleet has the most ships, then adjusts the camera to fit in all of the ships in that fleet
	if (mpPlayerOneFleet->GetRows() > mpPlayerTwoFleet->GetRows())
	{
		mCamZ = mBaseCamZ + (mpPlayerOneFleet->GetRows()*mCamZAdjust);
	}
	else
	{
		mCamZ = mBaseCamZ + (mpPlayerTwoFleet->GetRows()*mCamZAdjust);
	}

	mpCamMain = gpEngine->CreateCamera(kManual, 0.0f, 0.0f, mCamZ);

	// INITIALISE SKYBOX
	//------------------------------
	mpMshSkybox = gpEngine->LoadMesh("SkyboxSpace.x");
	mpMdlSkybox = mpMshSkybox->CreateModel(0.0f, -1000.0f, 0.0f);

	// LOAD MODELS
	//------------------------------
	mpPlayerOneFleet->LoadShipModels(-mDisplacement);
	mpPlayerTwoFleet->LoadShipModels(mDisplacement);
	mpMdlSkybox->AttachToParent(mpCamMain);

	// INITIALISE MUSIC
	//------------------------------
	string mMusicFile = "Space_Music.wav";
	ALfloat mSourcePos[3] = { mpCamMain->GetX(), mpCamMain->GetY(), mpCamMain->GetZ() };
	ALfloat mSourceVel[3] = { 0.0f, 0.0f, 0.0f };
	ALfloat listenerPos[3] = { mpCamMain->GetX(), mpCamMain->GetY(), mpCamMain->GetZ() };
	ALfloat listenerVel[3] = { 0.0f, 0.0f, 0.0f };
	mMusic = new CSound(mMusicFile, mSourcePos, mSourceVel, true, 1.0f, listenerPos, listenerVel);
	mMusic->PlaySound();
} 

void CSpaceState::StateUpdate()
{
	// Draw the scene
	gpEngine->DrawScene();
	if (gpEngine->KeyHit(Key_R))
	{
		gCurState = GS_WORLD;
	}

	if (gpEngine->KeyHit(Key_M))
	{
		gCurState = GS_MAIN_MENU;
	}

	//update time, used to slow down the speed of the fight
	mTimeSinceUpdate += gFrameTime;
	if (mTimeSinceUpdate >= mTimeToUpdate)
	{
		//fleets attack each other according to tactics
		mpPlayerOneFleet->Fight();
		mpPlayerTwoFleet->UpdateCondition();
		
		//finds and removes dead ships
		mpPlayerTwoFleet->Fight();
		mpPlayerOneFleet->UpdateCondition();
		
		//reset timer
		mTimeSinceUpdate = 0.0f;
	}
}

void CSpaceState::StateLoad()
{

}

void CSpaceState::StateSave()
{

}

void CSpaceState::StateCleanup()
{
	//stop sound
	mMusic->StopSound();

	//unload models
	mpMshSkybox->RemoveModel(mpMdlSkybox);

	mpPlayerOneFleet->ReturnFleet(mpHumanPlayer);
	mpPlayerTwoFleet->ReturnFleet(mpAIPlayer);

	//set pointers to null
	mpPlayerOneFleet = nullptr;
	mpPlayerTwoFleet = nullptr;
}
