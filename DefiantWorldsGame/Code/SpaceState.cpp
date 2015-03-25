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
CSpaceState::CSpaceState() :mTimeToUpdate(1.0f), mTimeToUpdateEffects(0.1f), mCamRotSpeed(0.7), mCamZAdjust(-10.4f), mBaseCamZ(-188.0f),
mDisplacement(30), mNumCamStates(3),CGameState()
{
	mTimeSinceUpdate = 0.0f;
	mCamZ = 0.0f;
	mCamState = 0;
	mCamZMovement = 0.0f;
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
	mpCamMain->LookAt(0.0f, 0.0f, 0.0f);

	// INITIALISE SKYBOX
	//------------------------------
	mpMshSkybox = gpEngine->LoadMesh("SkyboxSpace.x");
	mpMdlSkybox = mpMshSkybox->CreateModel(0.0f, -1000.0f, 0.0f);

	// INITIALISE MUSIC
	//------------------------------
	string mMusicFile = "Space_Music.wav";
	DX::XMFLOAT3 mSourcePos = { mpCamMain->GetX(), mpCamMain->GetY(), mpCamMain->GetZ() };
	DX::XMFLOAT3 mSourceVel = { 0.0f, 0.0f, 0.0f };
	DX::XMFLOAT3 listenerPos = { mpCamMain->GetX(), mpCamMain->GetY(), mpCamMain->GetZ() };
	DX::XMFLOAT3 listenerVel = { 0.0f, 0.0f, 0.0f };
	float volume = CStateControl::GetInstance()->GetSettingsManager()->GetMusicVolume();
	mMusic = new CSound(mMusicFile, mSourcePos, mSourceVel, true, volume, listenerPos, listenerVel);
	mMusic->PlaySound();

	// LOAD MODELS
	//------------------------------
	mpPlayerOneFleet->LoadShipModels(-mDisplacement);
	mpPlayerTwoFleet->LoadShipModels(mDisplacement);
	mpMdlSkybox->AttachToParent(mpCamMain);

	// INITIALISE USER INTERFACE
	//-----------------------------
	mFntDebug = gpEngine->LoadFont("Calibri", 20U);
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

	if (gpEngine->KeyHit(Key_C))
	{
		mCamState = (mCamState + 1) % mNumCamStates;
	}

	ChangeCameraPosition();
	//update time, used to slow down the speed of the fight
	mTimeSinceUpdate += gFrameTime;
	mpPlayerOneFleet->ChargeFleetLazers();
	mpPlayerTwoFleet->ChargeFleetLazers();

	if (mTimeSinceUpdate >= mTimeToUpdate)
	{
		//randomizes the order of fleet attack->update
		if (mNewRandom.GetRandomInt(1, 2) == 1)
		{
			mpPlayerOneFleet->Fight();
			mpPlayerTwoFleet->Fight();
		}
		else 
		{
			//finds and removes dead ships
			mpPlayerTwoFleet->Fight();
			mpPlayerOneFleet->Fight();
		}

		
		//reset timer
		mTimeSinceUpdate = 0.0f;
		mTimeSinceEffectsUpdate = mTimeToUpdateEffects;
	}

	//update effects time
	mTimeSinceEffectsUpdate -= gFrameTime;



	if (mTimeSinceEffectsUpdate <= 0.0f)
	{
		//unload the shield models
		mpPlayerOneFleet->UnloadShieldModels();
		mpPlayerTwoFleet->UnloadShieldModels();

		//unload the lazer models
		mpPlayerOneFleet->UnloadLazers();
		mpPlayerTwoFleet->UnloadLazers();

		//update fleet status
		mpPlayerOneFleet->UpdateCondition();
		mpPlayerTwoFleet->UpdateCondition();

		//reset timer
		mTimeSinceEffectsUpdate = 0.0f;
	}

	//moves fleet that has won 

	mpPlayerTwoFleet->MoveFleet();
	mpPlayerOneFleet->MoveFleet();

	mpPlayerOneFleet->IdleFleet();
	mpPlayerTwoFleet->IdleFleet();

	mCamZMovement += 50.0f*gFrameTime;

	DrawFontData();
}

void CSpaceState::StateLoad()
{

}

void CSpaceState::StateSave()
{

}

void CSpaceState::DrawFontData()
{
	int playerOneTextX = 5;
	int playerTwoTextX = 1580;

	//fleet size output
	strStream << "player one fleet size: " << mpPlayerOneFleet->GetSize();
	mFntDebug->Draw(strStream.str(), playerOneTextX, 715, kWhite, kLeft, kTop);
	strStream.str("");

	strStream << "  player two fleet size: " << mpPlayerTwoFleet->GetSize();
	mFntDebug->Draw(strStream.str(), playerTwoTextX, 715, kWhite, kRight, kTop);
	strStream.str("");

	//fleet total health output
	strStream << "player one fleet total health: " << mpPlayerOneFleet->GetFleetTotalHealth();
	mFntDebug->Draw(strStream.str(), playerOneTextX, 730, kWhite, kLeft, kTop);
	strStream.str("");

	strStream << "  player two fleet total health: " << mpPlayerTwoFleet->GetFleetTotalHealth();
	mFntDebug->Draw(strStream.str(), playerTwoTextX, 730, kWhite, kRight, kTop);
	strStream.str("");

	//fleet total health output
	strStream << "player one fleet avarage health: " << mpPlayerOneFleet->GetFleetAvargeHealth();
	mFntDebug->Draw(strStream.str(), playerOneTextX, 745, kWhite, kLeft, kTop);
	strStream.str("");

	strStream << "  player two fleet avarage health: " << mpPlayerTwoFleet->GetFleetAvargeHealth();
	mFntDebug->Draw(strStream.str(), playerTwoTextX, 745, kWhite, kRight, kTop);
	strStream.str("");

	//fleet total accuracy
	strStream << "player one fleet shots hit: " << mpPlayerOneFleet->GetHits() << "//" << mpPlayerOneFleet->GetShotsFired();
	mFntDebug->Draw(strStream.str(), playerOneTextX, 760, kWhite, kLeft, kTop);
	strStream.str("");

	strStream << "  player two fleet shots hit: " << mpPlayerTwoFleet->GetHits() << "//" << mpPlayerTwoFleet->GetShotsFired();
	mFntDebug->Draw(strStream.str(), playerTwoTextX, 760, kWhite, kRight, kTop);
	strStream.str("");

	//fleet total accuracy percentage output
	strStream << "player one fleet avarage accuracy: " << mpPlayerOneFleet->GetHitPercentage()<<"%";
	mFntDebug->Draw(strStream.str(), playerOneTextX, 775, kWhite, kLeft, kTop);
	strStream.str("");

	strStream << "  player two fleet avarage accuracy: " << mpPlayerTwoFleet->GetHitPercentage() << "%";
	mFntDebug->Draw(strStream.str(), playerTwoTextX, 775, kWhite, kRight, kTop);
	strStream.str("");
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

void CSpaceState::ChangeCameraPosition()
{
	switch (mCamState)
	{
	case 0:
		mpCamMain->SetPosition(0.0f, 10.0f, mCamZ + mCamZMovement);
		mpCamMain->LookAt(0.0f, 0.0f, 0.0f);
		mpCamMain->ResetOrientation();
		break;
	case 1:
		mpCamMain->ResetOrientation();
		mpCamMain->SetPosition(mCamZ, -mCamZ / 2.0f, mCamZMovement);
		mpCamMain->LookAt(0.0f, 0.0f, mCamZMovement);
		mpCamMain->RotateX(-40.0f);
		break;
	case 2:
		mpCamMain->ResetOrientation();
		mpCamMain->SetPosition(-mCamZ , -mCamZ / 2.0f, mCamZMovement);
		mpCamMain->LookAt(0.0f, 0.0f, mCamZMovement);
		mpCamMain->RotateX(-40.0f);
		break;
	case 3:
		break;
	default:
		break;
	}
}