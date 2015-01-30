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
	// Initialise Camera
	mpCamMain = gpEngine->CreateCamera(kFPS, 0.0f, 0.0f, 0.0f);
	
	// Initialise Skybox
	mpMshSkybox = gpEngine->LoadMesh("SkyboxSpace.x");
	mpMdlSkybox = mpMshSkybox->CreateModel(0.0f, -1000.0f, 0.0f);

	// Initialise planets
	mpMshPlanet = gpEngine->LoadMesh("Planet.x");
	mpMdlEarth = mpMshPlanet->CreateModel(-20.0f, 0.0f, 100.0f);
	mpMdlEarth->Scale(5.0f);
}

CMenuState::~CMenuState()
{

}


//-----------------------------------------------------
// MENU STATE CLASS OVERRIDE METHODS
//-----------------------------------------------------
void CMenuState::StateSetup()
{

}

void CMenuState::StateUpdate(const float inDelta)
{
	// Draw the scene
	gpEngine->DrawScene();
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
