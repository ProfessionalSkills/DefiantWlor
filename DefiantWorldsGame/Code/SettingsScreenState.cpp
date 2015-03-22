//-----------------------------------------------------
// FILE: MenuState.cpp
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
	gCurState = GS_MAIN_MENU;
}

void CSettingsScreenState::Cancel()
{
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

}

void CSettingsScreenState::DecrementEffects()
{

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


	// INITIALISE MUSIC
	//------------------------------
	string mMusicFile = "Intro.wav";
	DX::XMFLOAT3 mSourcePos = { mpCamMain->GetX(), mpCamMain->GetY(), mpCamMain->GetZ() };
	DX::XMFLOAT3 mSourceVel = { 0.0f, 0.0f, 0.0f };
	DX::XMFLOAT3 listenerPos = { mpCamMain->GetX(), mpCamMain->GetY(), mpCamMain->GetZ() };
	DX::XMFLOAT3 listenerVel = { 0.0f, 0.0f, 0.0f };
	mMusic = new CSound(mMusicFile, mSourcePos, mSourceVel, true, 0.6f, listenerPos, listenerVel);
	mMusic->PlaySound();


	// CREATE SPRITES, BUTTONS & FONTS
	//------------------------------
	mpButtonFont = gpEngine->LoadFont("font2.bmp", 15U);

	mpSprBackground = gpEngine->CreateSprite("MenuBG.png", 400.0f, 50.0f, 0.9f);

	CAdvancedButton<CSettingsScreenState, void>* pNewButton = new CAdvancedButton<CSettingsScreenState, void>("DefMenuButton.png", "SelMenuButton.png", SPointData(815, 620),
		SAABoundingBox(670.0f, 1215.0f, 620.0f, 815.0f), *this, &CSettingsScreenState::SaveSettings);
	mpButtonList.push_back(pNewButton);

	pNewButton = new CAdvancedButton<CSettingsScreenState, void>("DefMenuButton.png", "SelMenuButton.png", SPointData(815, 690),
		SAABoundingBox(740.0f, 1215.0f, 690.0f, 815.0f), *this, &CSettingsScreenState::Cancel);
	mpButtonList.push_back(pNewButton);

	// Settings buttons
	pNewButton = new CAdvancedButton<CSettingsScreenState, void>("DefSmallButton.png", "SelSmallButton.png", SPointData(615, 200),
		SAABoundingBox(250.0f, 715.0f, 200.0f, 615.0f), *this, &CSettingsScreenState::DecrementMusic);
	mpButtonList.push_back(pNewButton);

	pNewButton = new CAdvancedButton<CSettingsScreenState, void>("DefSmallButton.png", "SelSmallButton.png", SPointData(1225, 200),
		SAABoundingBox(250.0f, 1325.0f, 200.0f, 1225.0f), *this, &CSettingsScreenState::IncrementMusic);
	mpButtonList.push_back(pNewButton);


	// ADDITIONAL USER INTERFACE ELEMENTS
	//------------------------------
	mpMusicSlider = new CSliderTool(SPointData{720, 213}, 10, 0);
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
	mpButtonFont->Draw("SAVE SETTINGS", 1015, 635, kWhite, kCentre, kTop);
	mpButtonFont->Draw("CANCEL", 1015, 705, kWhite, kCentre, kTop);

	mMousePos.x = (float)gpEngine->GetMouseX();
	mMousePos.y = (float)gpEngine->GetMouseY();
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
			if (gpEngine->KeyHit(Mouse_LButton))
			{
				// Raise click flag
				pButton->Execute();
			}
		}

		// Update the button
		pButton->Update();
	}
}

void CSettingsScreenState::StateLoad()
{

}

void CSettingsScreenState::StateSave()
{

}

void CSettingsScreenState::StateCleanup()
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
