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
CSpaceState::CSpaceState() :mTimeToUpdate(1.0f), mTimeToUpdateEffects(0.1f), mCamRotSpeed(0.7), mCamZAdjust(-10.4f), mBaseCamZ(-188.0f), mCameraMoveSpeed(4.0f),
mDisplacement(30), mNumCamStates(4),CGameState()
{
	mpMdlSkybox = 0;
	mpMdlEarth = 0;
	mpMdlJupiter = 0;
	mpMdlMars = 0;
	mpMdlMoon = 0;
	mpMdlSun = 0;
	mpMdlVenus = 0;
	mpMdlMercury = 0;
	mpMdlNeptune = 0;
	mpMdlEarthAtmos = 0;

	PlayerOneVictory = false;
	mTimeSinceUpdate = 0.0f;

	mpPlayerOneFleet = 0;
	mpPlayerTwoFleet = 0;

	//Set Camera Values
	mCamZ = 0.0f;
	mCamState = 0;
	mCamZMovement = 0.0f;

	//Set Planet Positions
	mEarthPos = { -1000,-50 , 50, 100 };
	mMoonPos = { 100, 100, 100, 10 };
	mMarsPos = { 500, -400, 100, 50 };
	mSunPos = { 100, 50, -1000, 400 };
	mJupiterPos={ -900, -900, 800, 80 };
	mMercuryPos = { -200, -900, -600, 10 };
	mVenusPos ={ -200, 0, 700, 15 };
	mNeptunePos ={ 600, 100, 650, 25 };
}

CSpaceState::~CSpaceState()
{
	StateCleanup();
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
	ChangeCameraPosition();

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
	LoadPlanets();

	// CREATE SPRITES, BUTTONS & FONTS
	//------------------------------
	mpSprCursor = gpEngine->CreateSprite("BaseCursor.png", 5.0f, 50.0f, 0.0f);


	// INITIALISE USER INTERFACE
	//-----------------------------
	mFntDebug = gpEngine->LoadFont("Calibri", 20U);
} 

void CSpaceState::StateUpdate()
{
	if (mpPlayerOneFleet->GetSize() == 0 || mpPlayerTwoFleet->GetSize() == 0)
	{
		gCurState = GS_WORLD;
	}

	gpEngine->DrawScene();

	//Space Controls
	if (gpEngine->KeyHit(Key_R))
	{
		gCurState = GS_WORLD;
	}

	if (gpEngine->KeyHit(Key_M))
	{
		gCurState = GS_MAIN_MENU;
	}

	if (gpEngine->KeyHit(Key_Escape))
	{
		gCurState = GS_WORLD;
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

	//moves fleets
	mpPlayerTwoFleet->MoveFleet();
	mpPlayerOneFleet->MoveFleet();

	mpPlayerOneFleet->IdleFleet();
	mpPlayerTwoFleet->IdleFleet();

	mCamZMovement += mCameraMoveSpeed*gFrameTime;

	// UPDATE CURSOR
	//------------------------------
	mMousePos.x = (float)gpEngine->GetMouseX();
	mMousePos.y = (float)gpEngine->GetMouseY();

	mpSprCursor->SetPosition(mMousePos.x, mMousePos.y);

	DrawFontData();
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
	//remove sprites
	gpEngine->RemoveSprite(mpSprCursor);

	//stop sound
	mMusic->StopSound();

	//unload models
	if (mpMdlSkybox) mpMshSkybox->RemoveModel(mpMdlSkybox);

	if (mpMdlEarth) mpMshPlanet->RemoveModel(mpMdlEarth);
	if (mpMdlJupiter) mpMshPlanet->RemoveModel(mpMdlJupiter);
	if (mpMdlMars) mpMshPlanet->RemoveModel(mpMdlMars);
	if (mpMdlMoon) mpMshPlanet->RemoveModel(mpMdlMoon);
	if (mpMdlSun) mpMshPlanet->RemoveModel(mpMdlSun);
	if (mpMdlVenus) mpMshPlanet->RemoveModel(mpMdlVenus);
	if (mpMdlMercury) mpMshPlanet->RemoveModel(mpMdlMercury);
	if (mpMdlNeptune) mpMshPlanet->RemoveModel(mpMdlNeptune);

	if (mpMdlEarthAtmos) mpMshAtmosphere->RemoveModel(mpMdlEarthAtmos);

	//decide which player won, or if neither won
	if (mpPlayerOneFleet->GetSize() == 0)
	{
		PlayerOneVictory = false;
		PlayerTwoVictory = true;
	}
	else if (mpPlayerTwoFleet->GetSize() == 0)
	{
		PlayerOneVictory = true;
		PlayerTwoVictory = false;
	}
	else
	{
		PlayerOneVictory = false;
		PlayerTwoVictory = false;
	}
	mpHumanPlayer->SetWonLastSpaceBattle(PlayerOneVictory);
	mpAIPlayer->SetWonLastSpaceBattle(PlayerTwoVictory);

	//return fleets
	if (mpPlayerOneFleet) mpPlayerOneFleet->ReturnFleet(mpHumanPlayer,PlayerOneVictory);
	if (mpPlayerTwoFleet) mpPlayerTwoFleet->ReturnFleet(mpAIPlayer,PlayerTwoVictory);

	//reset camera
	mCamZMovement = 0.0f;

	//set pointers to null
	mpPlayerOneFleet = nullptr;
	mpPlayerTwoFleet = nullptr;
}

void CSpaceState::ChangeCameraPosition()
{
	switch (mCamState)
	{
	case 0:
		mpCamMain->ResetOrientation();
		mpCamMain->SetPosition(0.0f, 10.0f, mCamZ + mCamZMovement);
		mpCamMain->LookAt(0.0f, 0.0f, mCamZMovement);
		
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
		mpCamMain->ResetOrientation();
		mpCamMain->SetPosition(0.0f, 160.0f, -mCamZ + mCamZMovement);
		mpCamMain->LookAt(0.0f, 0.0f, mCamZMovement);
		
		break;
	default:
		break;
	}
}

void CSpaceState::LoadPlanets()
{
	// INITIALISE PLANETS
	//------------------------------
	// Earth
	mpMshPlanet = gpEngine->LoadMesh("Planet.x");
	mpMdlEarth = mpMshPlanet->CreateModel(mEarthPos.x, mEarthPos.y, mEarthPos.z);
	mpMdlEarth->Scale(mEarthPos.w);

	// Earth Atmosphere
	mpMshAtmosphere = gpEngine->LoadMesh("Atmos.x");
	mpMdlEarthAtmos = mpMshAtmosphere->CreateModel();
	mpMdlEarthAtmos->AttachToParent(mpMdlEarth);
	mpMdlEarthAtmos->Scale(1.02f);

	// Moon
	mpMdlMoon = mpMshPlanet->CreateModel(mEarthPos.x + mMoonPos.x, mEarthPos.y + mMoonPos.y, mEarthPos.z +mMoonPos.z);
	mpMdlMoon->SetSkin("texture_moon.jpg");
	mpMdlMoon->Scale(mMoonPos.w);

	// Mars
	mpMdlMars = mpMshPlanet->CreateModel(mMarsPos.x, mMarsPos.y, mMarsPos.z);
	mpMdlMars->SetSkin("Mars.jpg");
	mpMdlMars->Scale(mMarsPos.w);

	// Sun
	mpMdlSun = mpMshPlanet->CreateModel(mSunPos.x, mSunPos.y, mSunPos.z);
	mpMdlSun->SetSkin("texture_sun.jpg");
	mpMdlSun->Scale(mSunPos.w);

	// Jupiter
	mpMdlJupiter = mpMshPlanet->CreateModel(mJupiterPos.x, mJupiterPos.y, mJupiterPos.z);
	mpMdlJupiter->SetSkin("texture_jupiter.jpg");
	mpMdlJupiter->Scale(mJupiterPos.w);

	// Mercury
	mpMdlJupiter = mpMshPlanet->CreateModel(mMercuryPos.x, mMercuryPos.y, mMercuryPos.z);
	mpMdlJupiter->SetSkin("texture_mercury.jpg");
	mpMdlJupiter->Scale(mMercuryPos.w);

	// Venus
	mpMdlJupiter = mpMshPlanet->CreateModel(mVenusPos.x, mVenusPos.y, mVenusPos.z);
	mpMdlJupiter->SetSkin("texture_venus_surface.jpg");
	mpMdlJupiter->Scale(mVenusPos.w);

	// Neptune
	mpMdlNeptune = mpMshPlanet->CreateModel(mNeptunePos.x, mNeptunePos.y, mNeptunePos.z);
	mpMdlNeptune->SetSkin("texture_neptune.jpg");
	mpMdlNeptune->Scale(mNeptunePos.w);
}