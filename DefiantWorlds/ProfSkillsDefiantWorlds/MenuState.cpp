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
	mOrbitCentre = DX::XMFLOAT3(-80.0f, 0.0f, 50.0f);
	mEarthDistance = 60.0f;
	mMarsDistance = 100.0f;
	mMinAngle = ToRadians(45.0f);
	mMaxAngle = ToRadians(125.0f);

	mEarthAngle = gpRandomiser->GetRandomFloat(mMinAngle, mMaxAngle);
	mMarsAngle = gpRandomiser->GetRandomFloat(mMinAngle, mMaxAngle);


	// INITIALISE CAMERA
	//------------------------------
	mpCamMain = gpEngine->CreateCamera(kManual, 0.0f, 0.0f, 0.0f);


	// INITIALISE SKYBOX
	//------------------------------
	mpMshSkybox = gpEngine->LoadMesh("SkyboxSpace.x");
	mpMdlSkybox = mpMshSkybox->CreateModel(0.0f, -1000.0f, 0.0f);


	// INITIALISE PLANETS
	//------------------------------
	// Earth
	mpMshPlanet = gpEngine->LoadMesh("Planet.x");
	mpMdlEarth = mpMshPlanet->CreateModel(-20.0f, 0.0f, 45.0f);
	mpMdlEarth->Scale(5.0f);

	// Mars
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
	mpMdlAtmosphere->RotateLocalX(gpRandomiser->GetRandomFloat(0.0f, 0.8f) * inDelta);
	mpMdlAtmosphere->RotateLocalY(gpRandomiser->GetRandomFloat(0.5f,  1.4f) * inDelta);
	mpMdlAtmosphere->RotateLocalZ(gpRandomiser->GetRandomFloat(0.0f,  1.0f) * inDelta);


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
}

void CMenuState::StateLoad()
{

}

void CMenuState::StateSave()
{

}

void CMenuState::StateCleanup()
{

}
