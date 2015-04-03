//-----------------------------------------------------
// FILE: SettingsScreenState.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "SettingsScreenState.h"
#include "GameStateControl.h"


//-----------------------------------------------------
// MENU STATE CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CSettingsScreenState::CSettingsScreenState() : CGameState()
{

}

CSettingsScreenState::~CSettingsScreenState()
{

}


//-----------------------------------------------------
// MENU STATE CLASS METHODS
//-----------------------------------------------------
void CSettingsScreenState::SaveSettings()
{
	// Save the settings to the settings manager
	CSettingsManager* pSettings = CStateControl::GetInstance()->GetSettingsManager();
	pSettings->SetMusicVolume((float)(mpMusicSlider->GetSliderSetting() / 50.0f));
	pSettings->SetEffectsVolume((float)(mpEffectsSlider->GetSliderSetting() / 50.0f));

	// Set new state
	gCurState = GS_MAIN_MENU;
}

void CSettingsScreenState::Cancel()
{
	// Set new state
	gCurState = GS_MAIN_MENU;
}

void CSettingsScreenState::IncrementMusic()
{
	mpMusicSlider->IncrementSlider();
}

void CSettingsScreenState::DecrementMusic()
{
	mpMusicSlider->DecrementSlider();
}

void CSettingsScreenState::IncrementEffects()
{
	mpEffectsSlider->IncrementSlider();
}

void CSettingsScreenState::DecrementEffects()
{
	mpEffectsSlider->DecrementSlider();
}


//-----------------------------------------------------
// MENU STATE CLASS OVERRIDE METHODS
//-----------------------------------------------------
void CSettingsScreenState::StateSetup()
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


	// CREATE SPRITES, BUTTONS & FONTS
	//------------------------------
	mpButtonFont = gpEngine->LoadFont("font2.bmp", 15U);
	mpIncDecFont = gpEngine->LoadFont("font2.bmp", 25U);
	mpTitleFont = gpEngine->LoadFont("font2.bmp", 35U);

	mpSprBackground = gpEngine->CreateSprite("MenuBG.png", 400.0f, 50.0f, 0.9f);
	mpSprCursor = gpEngine->CreateSprite("BaseCursor.png", 5.0f, 50.0f, 0.0f);

	CAdvancedButton<CSettingsScreenState, void>* pNewButton = new CAdvancedButton<CSettingsScreenState, void>("DefMenuButton.png", "SelMenuButton.png", SPointData(815, 620),
		DX::XMFLOAT2(400.0f, 50.0f), *this, &CSettingsScreenState::SaveSettings);
	mpButtonList.push_back(pNewButton);

	pNewButton = new CAdvancedButton<CSettingsScreenState, void>("DefMenuButton.png", "SelMenuButton.png", SPointData(815, 690),
		DX::XMFLOAT2(400.0f, 50.0f), *this, &CSettingsScreenState::Cancel);
	mpButtonList.push_back(pNewButton);


	// Settings buttons
	pNewButton = new CAdvancedButton<CSettingsScreenState, void>("DefSmallButton.png", "SelSmallButton.png", SPointData(650, 200),
		DX::XMFLOAT2(100.0f, 50.0f), *this, &CSettingsScreenState::DecrementMusic);
	mpButtonList.push_back(pNewButton);

	pNewButton = new CAdvancedButton<CSettingsScreenState, void>("DefSmallButton.png", "SelSmallButton.png", SPointData(1270, 200),
		DX::XMFLOAT2(100.0f, 50.0f), *this, &CSettingsScreenState::IncrementMusic);
	mpButtonList.push_back(pNewButton);

	pNewButton = new CAdvancedButton<CSettingsScreenState, void>("DefSmallButton.png", "SelSmallButton.png", SPointData(650, 310),
		DX::XMFLOAT2(100.0f, 50.0f), *this, &CSettingsScreenState::DecrementEffects);
	mpButtonList.push_back(pNewButton);

	pNewButton = new CAdvancedButton<CSettingsScreenState, void>("DefSmallButton.png", "SelSmallButton.png", SPointData(1270, 310),
		DX::XMFLOAT2(100.0f, 50.0f), *this, &CSettingsScreenState::IncrementEffects);
	mpButtonList.push_back(pNewButton);


	// ADDITIONAL USER INTERFACE ELEMENTS
	//------------------------------
	CSettingsManager* pSettings = CStateControl::GetInstance()->GetSettingsManager();
	mpMusicSlider = new CSliderTool(SPointData{ 760, 213 }, 100, (int)(pSettings->GetMusicVolume() * 50));
	mpEffectsSlider = new CSliderTool(SPointData{ 760, 323 }, 100, (int)(pSettings->GetEffectsVolume() * 50));
}

void CSettingsScreenState::StateUpdate()
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
	mpTitleFont->Draw("GAME SETTINGS", 1015, 90, kCyan, kCentre, kTop);

	mpButtonFont->Draw("SAVE SETTINGS", 1015, 635, kWhite, kCentre, kTop);
	mpButtonFont->Draw("CANCEL", 1015, 705, kWhite, kCentre, kTop);

	mpIncDecFont->Draw("MUSIC VOLUME", 1015, 180, kWhite, kCentre, kTop);
	mpIncDecFont->Draw("EFFECTS VOLUME", 1015, 290, kWhite, kCentre, kTop);

	mpIncDecFont->Draw("--", 700, 210, kWhite, kCentre, kTop);
	mpIncDecFont->Draw("++", 1320, 212, kWhite, kCentre, kTop);
	mpIncDecFont->Draw("--", 700, 320, kWhite, kCentre, kTop);
	mpIncDecFont->Draw("++", 1320, 322, kWhite, kCentre, kTop);

	mMousePos.x = (float)gpEngine->GetMouseX();
	mMousePos.y = (float)gpEngine->GetMouseY();

	// Check for mouse click
	bool leftClicked = false;
	if (gpEngine->KeyHit(Mouse_LButton))
	{
		leftClicked = true;
	}


	// CHECK FOR SLIDER MOUSE OVER AND CLICKS
	if (mpMusicSlider->GetBoundingBox().IsColliding(DX::XMFLOAT3(mMousePos.x, 0.0f, mMousePos.y)))
	{
		mpMusicSlider->SetMouseOver(true);
		if (leftClicked)
		{
			mpMusicSlider->OnClick(mMousePos.x);
			leftClicked = false;
		}
	}
	else
	{
		mpMusicSlider->SetMouseOver(false);
	}

	if (mpEffectsSlider->GetBoundingBox().IsColliding(DX::XMFLOAT3(mMousePos.x, 0.0f, mMousePos.y)))
	{
		mpEffectsSlider->SetMouseOver(true);
		if (leftClicked)
		{
			mpEffectsSlider->OnClick(mMousePos.x);
			leftClicked = false;
		}
	}
	else
	{
		mpEffectsSlider->SetMouseOver(false);
	}


	// CHECK FOR BUTTON MOUSE OVER AND CLICKS
	for (miterButtons = mpButtonList.begin(); miterButtons != mpButtonList.end(); miterButtons++)
	{
		CAdvancedButton<CSettingsScreenState, void>* pButton = (*miterButtons);
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


	// UPDATE SLIDERS
	//------------------------------
	mpMusicSlider->Update();
	mpEffectsSlider->Update();

	leftClicked = false;


	// UPDATE CURSOR
	//------------------------------
	mpSprCursor->SetPosition(mMousePos.x, mMousePos.y);
}

void CSettingsScreenState::StateCleanup()
{
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
		CAdvancedButton<CSettingsScreenState, void>* tmp;
		tmp = mpButtonList.back();
		if (tmp)
		{
			delete tmp;
			tmp = nullptr;
		}

		mpButtonList.pop_back();
	}

	SafeDelete(mpEffectsSlider);
	SafeDelete(mpMusicSlider);
}
