//-----------------------------------------------------
// FILE: MenuState.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "MenuState.h"


//-----------------------------------------------------
// MENU STATE CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CMenuState::CMenuState() : IGameState()
{
	
}

CMenuState::~CMenuState()
{

}


//-----------------------------------------------------
// MENU STATE CLASS METHODS
//-----------------------------------------------------
void CMenuState::NewGame()
{
	gCurState = GS_WORLD;
}

void CMenuState::LoadGame()
{

}

void CMenuState::ChangeSettings()
{

}

void CMenuState::Quit()
{

}


//-----------------------------------------------------
// MENU STATE CLASS OVERRIDE METHODS
//-----------------------------------------------------
void CMenuState::StateSetup()
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
	ALfloat mSourcePos[3] = { mpCamMain->GetX(), mpCamMain->GetY(), mpCamMain->GetZ() };
	ALfloat mSourceVel[3] = { 0.0f, 0.0f, 0.0f };
	mMusic = new CSound(mMusicFile, mSourcePos, mSourceVel);
	mMusic->PlaySound();


	// CREATE SPRITES & BUTTONS
	//------------------------------
	mpSprBackground = gpEngine->CreateSprite("MenuBG.png", 400.0f, 50.0f, 0.9f);
	mpSprLogo = gpEngine->CreateSprite("Logo.png", 800.0f, 100.0f, 0.8f);

	CButton* pNewButton = new CButton("DefMenuButton.png", "SelMenuButton.png", SPointData(815, 350), SAABoundingBox(400.0f, 1215.0f, 350.0f, 815.0f), NewGame);
	mpButtonList.push_back(pNewButton);
}

void CMenuState::StateUpdate()
{
	// SCENE DRAW
	//------------------------------
	gpEngine->DrawScene();

	// ANIMATE ATMOSPHERE
	//------------------------------
	mpMdlAtmosphere->RotateLocalX(gpRandomiser->GetRandomFloat(0.0f, 2.6f) * gFrameTime);
	mpMdlAtmosphere->RotateLocalY(gpRandomiser->GetRandomFloat(-1.0f,  2.8f) * gFrameTime);
	mpMdlAtmosphere->RotateLocalZ(gpRandomiser->GetRandomFloat(0.0f,  2.0f) * gFrameTime);


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
	mMousePos.x = (int)gpEngine->GetMouseX();
	mMousePos.y = (int)gpEngine->GetMouseY();
	for (miterButtons = mpButtonList.begin(); miterButtons != mpButtonList.end(); miterButtons++)
	{
		// Check if the mouse is colliding with the object
		if ((*miterButtons)->GetBoundingBox().IsColliding(DX::XMFLOAT3(mMousePos.x, 0.0f, mMousePos.y)))
		{
			(*miterButtons)->SetMouseOver(true);
		}
		else
		{
			(*miterButtons)->SetMouseOver(false);
		}

		// Check for click
		if (gpEngine->KeyHit(Mouse_LButton))
		{
			// Check if the mouse is over the button
			if ((*miterButtons)->GetMouseOver())
			{
				// Raise click flag
				(*miterButtons)->SetClick(true);
			}
		}

		// Update the button
		(*miterButtons)->Update();
	}


	// STATE CHANGE TEST
	//------------------------------
	if (gpEngine->KeyHit(Key_Return))
	{
		gCurState = GS_WORLD;
	}
}

void CMenuState::StateLoad()
{

}

void CMenuState::StateSave()
{

}

void CMenuState::StateCleanup()
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
	gpEngine->RemoveSprite(mpSprLogo);

	// Remove buttons
	while (!mpButtonList.empty())
	{
		CButton* tmp;
		tmp = mpButtonList.back();
		if (tmp)
		{
			delete tmp;
			tmp = nullptr;
		}

		mpButtonList.pop_back();
	}
}
