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
	mMaxAngle = ToRadians(150.0f);


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
	mpMdlEarth = mpMshPlanet->CreateModel(-20.0f, 0.0f, 45.0f);
	mpMdlEarth->Scale(5.0f);

	// Mars
	mMarsAngle = gpRandomiser->GetRandomFloat(mMinAngle, mMaxAngle);
	mpMdlMars = mpMshPlanet->CreateModel(30.0f, 0.0f, 45.0f);
	mpMdlMars->SetSkin("Mars.jpg");
	mpMdlMars->Scale(2.6f);


	// INITIALISE ATMOSPHERE
	//------------------------------
	mpMshAtmosphere = gpEngine->LoadMesh("Atmos.x");
	mpMdlAtmosphere = mpMshAtmosphere->CreateModel(0.0f, 0.0f, 0.0f);
	mpMdlAtmosphere->AttachToParent(mpMdlEarth);
	mpMdlAtmosphere->Scale(1.02f);
}

void CMenuState::StateUpdate(const float inDelta)
{
	// SCENE DRAW
	//------------------------------
	gpEngine->DrawScene();


	// ANIMATE ATMOSPHERE
	//------------------------------
	mpMdlAtmosphere->RotateLocalX(gpRandomiser->GetRandomFloat(0.0f, 1.6f) * inDelta);
	mpMdlAtmosphere->RotateLocalY(gpRandomiser->GetRandomFloat(1.0f,  2.8f) * inDelta);
	mpMdlAtmosphere->RotateLocalZ(gpRandomiser->GetRandomFloat(0.0f,  2.0f) * inDelta);


	// CALCULATE PLANET ORBIT POSITIONS
	//------------------------------
	mpMdlEarth->SetX(mOrbitCentre.x + mEarthDistance * sinf(mEarthAngle));
	mpMdlEarth->SetY(mOrbitCentre.y + mEarthDistance* cosf(mEarthAngle));
	mEarthAngle += inDelta * 0.01f;
	if (mEarthAngle > mMaxAngle)
	{
		mEarthAngle = mMinAngle;
	}

	mpMdlMars->SetX(mOrbitCentre.x + mMarsDistance * sinf(mMarsAngle));
	mpMdlMars->SetY(mOrbitCentre.y + mMarsDistance * cosf(mMarsAngle));
	mMarsAngle += inDelta * 0.025f;
	if (mMarsAngle > mMaxAngle)
	{
		mMarsAngle = mMinAngle;
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
	mpMshAtmosphere->RemoveModel(mpMdlAtmosphere);
	mpMshPlanet->RemoveModel(mpMdlMars);
	mpMshPlanet->RemoveModel(mpMdlEarth);
	mpMshSkybox->RemoveModel(mpMdlSkybox);

	gpEngine->RemoveCamera(mpCamMain);
	gpEngine->RemoveMesh(mpMshAtmosphere);
	gpEngine->RemoveMesh(mpMshPlanet);
	gpEngine->RemoveMesh(mpMshSkybox);
}
