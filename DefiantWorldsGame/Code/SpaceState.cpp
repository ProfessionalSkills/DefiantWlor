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
CSpaceState::CSpaceState() :mTimeToUpdate(0.1f), mCamRotSpeed(0.7), CGameState() 

{
	mDisplacement = 20.0f;
	mTimeSinceUpdate = 0.0f;
	
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
	mpCamMain = gpEngine->CreateCamera(kManual, 0.0f, 0.0f, -50.0f);

	// INITIALISE SKYBOX
	//------------------------------
	mpMshSkybox = gpEngine->LoadMesh("SkyboxSpace.x");
	mpMdlSkybox = mpMshSkybox->CreateModel(0.0f, -1000.0f, 0.0f);

	// LOAD MODELS
	//------------------------------
	mpPlayerOneFleet->LoadShipModels(-mDisplacement);
	mpPlayerTwoFleet->LoadShipModels(mDisplacement);

	// INITIALISE MUSIC
	//------------------------------
	string mMusicFile = "Space_Music.wav";
	ALfloat mSourcePos[3] = { mpCamMain->GetX(), mpCamMain->GetY(), mpCamMain->GetZ() };
	ALfloat mSourceVel[3] = { 0.0f, 0.0f, 0.0f };
	mMusic = new CSound(mMusicFile, mSourcePos, mSourceVel);
	mMusic->PlaySound();

	//doesn't play sound yet, maybe has problems with small sound files?
	mMusicFile = "SpaceLaser.wav";
	ALfloat mSourceVel2[3] = { 1.0f, 0.0f, 0.0f };
	mFiringSound = new CSound(mMusicFile, mSourcePos, mSourceVel2);
	mFiringSound->PlaySound();

	mpMdlSkybox->AttachToParent(mpCamMain);
}

void CSpaceState::StateUpdate()
{
	// Draw the scene
	gpEngine->DrawScene();

	mpCamMain->RotateLocalZ(mCamRotSpeed*gFrameTime);
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

		mpPlayerTwoFleet->Fight();

		//finds and removes dead ships
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
	mFiringSound->StopSound();

	//unload models
	mpMshSkybox->RemoveModel(mpMdlSkybox);

	mpPlayerOneFleet->ReturnFleet(mpHumanPlayer);
	mpPlayerTwoFleet->ReturnFleet(mpAIPlayer);

	//set pointers to null
	mpPlayerOneFleet = nullptr;
	mpPlayerTwoFleet = nullptr;
}
