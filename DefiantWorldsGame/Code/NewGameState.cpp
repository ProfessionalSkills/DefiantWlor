//-----------------------------------------------------
// FILE: NewGameState.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "NewGameState.h"
#include "GameStateControl.h"


//-----------------------------------------------------
// NEW GAME STATE CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CNewGameState::CNewGameState() : CGameState()
{

}

CNewGameState::~CNewGameState()
{

}


//-----------------------------------------------------
// MENU STATE CLASS METHODS
//-----------------------------------------------------
void CNewGameState::StartNewGame()
{
	// Save the settings to the settings manager
	CSettingsManager* pSettings = CStateControl::GetInstance()->GetSettingsManager();
	pSettings->SetAIDifficulty(mCurAIDifficulty);

	// Unload any previous players & create new players
	CPlayerManager* pPlayerManager = CStateControl::GetInstance()->GetPlayerManager();
	pPlayerManager->RemovePlayers();
	pPlayerManager->PlayersUnloaded();
	pPlayerManager->CreatePlayers(FAC_EARTH_DEFENSE_FORCE, 1);

	// Set new state
	gCurState = GS_WORLD;
}

void CNewGameState::Cancel()
{
	// Set new state
	gCurState = GS_MAIN_MENU;
}

void CNewGameState::SetAIDifficulty(int difficulty)
{
	// Take the current AI difficulty button and reset its texture
	mpAIDButtonList[mCurAIDifficulty]->SetNewButtonSkin("DefSmallButton.png");

	// Store new difficulty and update that button's texture
	mCurAIDifficulty = difficulty;
	mpAIDButtonList[mCurAIDifficulty]->SetNewButtonSkin("ChoSmallButton.png");
}


//-----------------------------------------------------
// MENU STATE CLASS OVERRIDE METHODS
//-----------------------------------------------------
void CNewGameState::StateSetup()
{
	// INITIALISE ADDITIONAL VARIABLES
	//------------------------------
	mOrbitCentre = DX::XMFLOAT3(-60.0f, 0.0f, 50.0f);

	mEarthDistance = 35.0f;
	mMarsDistance = 65.0f;

	mMinAngle = ToRadians(33.0f);
	mMaxAngle = ToRadians(145.0f);


	// INITIALISE CAMERAS
	//------------------------------
	mpCamMain = gpEngine->CreateCamera(kManual, 0.0f, 0.0f, 0.0f);


	// INITIALISE SKYBOX
	//------------------------------
	mpMshSkybox = gpEngine->LoadMesh("SkyboxSpace.x");
	mpMdlSkybox = mpMshSkybox->CreateModel(0.0f, -1000.0f, 0.0f);
	mpMdlSkybox->RotateY(130.0f);


	// INITIALISE PLANETS
	//------------------------------
	// Earth
	mEarthAngle = gpRandomiser->GetRandomFloat(mMinAngle, mMaxAngle);
	mpMshPlanet = gpEngine->LoadMesh("Planet.x");
	mpMdlEarth = mpMshPlanet->CreateModel(-60.0f, 0.0f, 45.0f);
	mpMdlEarth->Scale(5.0f);

	// Mars
	mMarsAngle = gpRandomiser->GetRandomFloat(mMinAngle, mMaxAngle);
	mpMdlMars = mpMshPlanet->CreateModel(-60.0f, 0.0f, 45.0f);
	mpMdlMars->SetSkin("Mars.jpg");
	mpMdlMars->Scale(2.6f);


	// INITIALISE ATMOSPHERE
	//------------------------------
	mpMshAtmosphere = gpEngine->LoadMesh("Atmos.x");
	mpMdlAtmosphere = mpMshAtmosphere->CreateModel(0.0f, 0.0f, 0.0f);
	mpMdlAtmosphere->AttachToParent(mpMdlEarth);
	mpMdlAtmosphere->Scale(1.02f);


	// INITIALISE MUSIC
	//------------------------------
	string mMusicFile = "Intro.wav";
	DX::XMFLOAT3 mSourcePos = { mpCamMain->GetX(), mpCamMain->GetY(), mpCamMain->GetZ() };
	DX::XMFLOAT3 mSourceVel = { 0.0f, 0.0f, 0.0f };
	DX::XMFLOAT3 listenerPos = { mpCamMain->GetX(), mpCamMain->GetY(), mpCamMain->GetZ() };
	DX::XMFLOAT3 listenerVel = { 0.0f, 0.0f, 0.0f };
	float volume = CStateControl::GetInstance()->GetSettingsManager()->GetMusicVolume();
	mMusic = new CSound(mMusicFile, mSourcePos, mSourceVel, true, volume, listenerPos, listenerVel);
	mMusic->PlaySound();


	// CREATE SPRITES, BUTTONS & FONTS
	//------------------------------
	mpButtonFont = gpEngine->LoadFont("font2.bmp", 15U);
	mpIncDecFont = gpEngine->LoadFont("font2.bmp", 25U);
	mpTitleFont = gpEngine->LoadFont("font2.bmp", 35U);

	mpSprBackground = gpEngine->CreateSprite("MenuBG.png", 400.0f, 50.0f, 0.9f);
	mpSprCursor = gpEngine->CreateSprite("BaseCursor.png", 5.0f, 50.0f, 0.0f);

	CAdvancedButton<CNewGameState, void>* pNewButton = new CAdvancedButton<CNewGameState, void>("DefMenuButton.png", "SelMenuButton.png", SPointData(815, 620),
		SAABoundingBox(670.0f, 1215.0f, 620.0f, 815.0f), *this, &CNewGameState::StartNewGame);
	mpButtonList.push_back(pNewButton);

	pNewButton = new CAdvancedButton<CNewGameState, void>("DefMenuButton.png", "SelMenuButton.png", SPointData(815, 690),
		SAABoundingBox(740.0f, 1215.0f, 690.0f, 815.0f), *this, &CNewGameState::Cancel);
	mpButtonList.push_back(pNewButton);


	// AI Difficulty buttons
	CAdvancedButton<CNewGameState, void, int>* pNewAIButton = new CAdvancedButton<CNewGameState, void, int>("DefSmallButton.png", "SelSmallButton.png", SPointData(755, 230),
		SAABoundingBox(280.0f, 855.0f, 230.0f, 755.0f), *this, &CNewGameState::SetAIDifficulty);
	mpAIDButtonList.push_back(pNewAIButton);

	pNewAIButton = new CAdvancedButton<CNewGameState, void, int>("DefSmallButton.png", "SelSmallButton.png", SPointData(895, 230),
		SAABoundingBox(280.0f, 995.0f, 230.0f, 895.0f), *this, &CNewGameState::SetAIDifficulty);
	mpAIDButtonList.push_back(pNewAIButton);

	pNewAIButton = new CAdvancedButton<CNewGameState, void, int>("DefSmallButton.png", "SelSmallButton.png", SPointData(1035, 230),
		SAABoundingBox(280.0f, 1135.0f, 230.0f, 1035.0f), *this, &CNewGameState::SetAIDifficulty);
	mpAIDButtonList.push_back(pNewAIButton);

	pNewAIButton = new CAdvancedButton<CNewGameState, void, int>("DefSmallButton.png", "SelSmallButton.png", SPointData(1175, 230),
		SAABoundingBox(280.0f, 1275.0f, 230.0f, 1175.0f), *this, &CNewGameState::SetAIDifficulty);
	mpAIDButtonList.push_back(pNewAIButton);

	// Highlight already selected AI
	CSettingsManager* pSettings = CStateControl::GetInstance()->GetSettingsManager();
	mCurAIDifficulty = pSettings->GetAIDifficulty();
	mpAIDButtonList[mCurAIDifficulty]->SetNewButtonSkin("ChoSmallButton.png");
}

void CNewGameState::StateUpdate()
{
	// SCENE DRAW
	//------------------------------
	gpEngine->DrawScene();

	// ANIMATE ATMOSPHERE
	//------------------------------
	mpMdlAtmosphere->RotateLocalX(gpRandomiser->GetRandomFloat(0.0f, 2.6f) * gFrameTime);
	mpMdlAtmosphere->RotateLocalY(gpRandomiser->GetRandomFloat(-1.0f, 2.8f) * gFrameTime);
	mpMdlAtmosphere->RotateLocalZ(gpRandomiser->GetRandomFloat(0.0f, 2.0f) * gFrameTime);


	// CALCULATE PLANET ORBIT POSITIONS
	//------------------------------
	mpMdlEarth->SetX(mOrbitCentre.x + mEarthDistance * sinf(mEarthAngle));
	mpMdlEarth->SetY(mOrbitCentre.y + mEarthDistance* cosf(mEarthAngle));
	mEarthAngle += gFrameTime * 0.01f;
	if (mEarthAngle > mMaxAngle)
	{
		mEarthAngle = mMinAngle;
	}

	mpMdlMars->SetX(mOrbitCentre.x + mMarsDistance * sinf(mMarsAngle));
	mpMdlMars->SetY(mOrbitCentre.y + mMarsDistance * cosf(mMarsAngle));
	mMarsAngle += gFrameTime * 0.025f;
	if (mMarsAngle > mMaxAngle)
	{
		mMarsAngle = mMinAngle;
	}


	// UPDATE BUTTONS
	//------------------------------
	mpTitleFont->Draw("NEW GAME OPTIONS", 1015, 90, kCyan, kCentre, kTop);

	mpButtonFont->Draw("START NEW GAME", 1015, 635, kWhite, kCentre, kTop);
	mpButtonFont->Draw("CANCEL", 1015, 705, kWhite, kCentre, kTop);

	mpIncDecFont->Draw("AI DIFFICULTY", 1015, 180, kWhite, kCentre, kTop);

	mpButtonFont->Draw("EASY", 805, 245, kWhite, kCentre, kTop);
	mpButtonFont->Draw("MEDIUM", 945, 245, kWhite, kCentre, kTop);
	mpButtonFont->Draw("HARD", 1085, 245, kWhite, kCentre, kTop);
	mpButtonFont->Draw("INSANE", 1225, 245, kWhite, kCentre, kTop);

	mMousePos.x = (float)gpEngine->GetMouseX();
	mMousePos.y = (float)gpEngine->GetMouseY();

	// Check for mouse click
	bool leftClicked = false;
	if (gpEngine->KeyHit(Mouse_LButton))
	{
		leftClicked = true;
	}


	// CHECK FOR BUTTON MOUSE OVER AND CLICKS
	for (miterButtons = mpButtonList.begin(); miterButtons != mpButtonList.end(); miterButtons++)
	{
		CAdvancedButton<CNewGameState, void>* pButton = (*miterButtons);
		// Check if the mouse is colliding with the object
		if (pButton->GetBoundingBox().IsColliding(DX::XMFLOAT3(mMousePos.x, 0.0f, mMousePos.y)))
		{
			pButton->SetMouseOver(true);
		}
		else
		{
			pButton->SetMouseOver(false);
		}

		// Check for click 
		if (pButton->GetMouseOver())
		{
			// Check if the mouse is over the button
			if (leftClicked)
			{
				// Raise click flag
				pButton->Execute();
			}
		}

		// Update the button
		pButton->Update();
	}

	int counter = 0;
	for (miterAIDButtons = mpAIDButtonList.begin(); miterAIDButtons != mpAIDButtonList.end(); miterAIDButtons++)
	{
		CAdvancedButton<CNewGameState, void, int>* pButton = (*miterAIDButtons);
		// Check if the mouse is colliding with the object
		if (pButton->GetBoundingBox().IsColliding(DX::XMFLOAT3(mMousePos.x, 0.0f, mMousePos.y)))
		{
			pButton->SetMouseOver(true);
		}
		else
		{
			pButton->SetMouseOver(false);
		}

		// Check for click 
		if (pButton->GetMouseOver())
		{
			// Check if the mouse is over the button
			if (leftClicked)
			{
				// Raise click flag
				pButton->Execute(counter);
			}
		}

		// Update the button
		pButton->Update();
		counter++;
	}

	leftClicked = false;


	// UPDATE CURSOR
	//------------------------------
	mpSprCursor->SetPosition(mMousePos.x, mMousePos.y);
}

void CNewGameState::StateLoad()
{

}

void CNewGameState::StateSave()
{

}

void CNewGameState::StateCleanup()
{
	mMusic->StopSound();

	mpMshAtmosphere->RemoveModel(mpMdlAtmosphere);
	mpMshPlanet->RemoveModel(mpMdlMars);
	mpMshPlanet->RemoveModel(mpMdlEarth);
	mpMshSkybox->RemoveModel(mpMdlSkybox);

	gpEngine->RemoveCamera(mpCamMain);
	gpEngine->RemoveMesh(mpMshAtmosphere);
	gpEngine->RemoveMesh(mpMshPlanet);
	gpEngine->RemoveMesh(mpMshSkybox);

	gpEngine->RemoveSprite(mpSprBackground);
	gpEngine->RemoveSprite(mpSprCursor);

	gpEngine->RemoveFont(mpButtonFont);
	gpEngine->RemoveFont(mpIncDecFont);
	gpEngine->RemoveFont(mpTitleFont);

	// Remove buttons
	while (!mpButtonList.empty())
	{
		CAdvancedButton<CNewGameState, void>* tmp;
		tmp = mpButtonList.back();
		if (tmp)
		{
			delete tmp;
			tmp = nullptr;
		}

		mpButtonList.pop_back();
	}

	while (!mpAIDButtonList.empty())
	{
		CAdvancedButton<CNewGameState, void, int>* tmp;
		tmp = mpAIDButtonList.back();
		if (tmp)
		{
			delete tmp;
			tmp = nullptr;
		}

		mpAIDButtonList.pop_back();
	}
}
