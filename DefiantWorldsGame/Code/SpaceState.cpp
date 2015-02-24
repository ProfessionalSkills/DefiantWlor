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
CSpaceState::CSpaceState() : CGameState()
{
	mDisplacement = 20.0f;
	mTimeSinceUpdate = 0.0f;
	mTimeToUpdate = 0.1f;
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

	//temp, for testing only//
	mpHumanPlayer->LaunchAttack();
	mpAIPlayer->LaunchAttack();
	//----------------------------//

	mpPlayerOneFleet->LoadShipModels(-mDisplacement);
	mpPlayerTwoFleet->LoadShipModels(mDisplacement);

	// INITIALISE MUSIC
	//------------------------------
	string mMusicFile = "Space_Music.wav";
	ALfloat mSourcePos[3] = { mpCamMain->GetX(), mpCamMain->GetY(), mpCamMain->GetZ() };
	ALfloat mSourceVel[3] = { 0.0f, 0.0f, 0.0f };
	mMusic = new CSound(mMusicFile, mSourcePos, mSourceVel);
	mMusic->PlaySound();
}

void CSpaceState::StateUpdate()
{
	// Draw the scene
	gpEngine->DrawScene();

	//update time, used to slow down the speed of the fight
	mTimeSinceUpdate += gFrameTime;
	if (mTimeSinceUpdate >= mTimeToUpdate)
	{
		//fleets attack each other according to tactics
		mpPlayerOneFleet->Fight();
		mpPlayerTwoFleet->Fight();

		//finds and removes dead ships
		mpPlayerOneFleet->UpdateCondition();
		mpPlayerTwoFleet->UpdateCondition();

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
	//set pointers to null
	mpPlayerOneFleet = nullptr;
	mpPlayerTwoFleet = nullptr;

	mpPlayerOneFleet->SetEnemy(nullptr);
	mpPlayerTwoFleet->SetEnemy(nullptr);

	//stop sound
	mMusic->StopSound();

	//unload models
	mpMshSkybox->RemoveModel(mpMdlSkybox);

}
