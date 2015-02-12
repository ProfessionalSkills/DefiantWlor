//-----------------------------------------------------
// FILE: Main.cpp
//-----------------------------------------------------

/*
PROJECT NAME:
ProfSkillsDefiantWorlds

TODO:
	- Main menu UI
	- Create Mars world
	- Create Space 'fight' area
	- Get high res & open source texture of Earth. Currently just a high res placeholder.
	- Send requests from buildings for units to be built.
	- Get the queue working for each building type.
	- Particle effects need to be fully implemented.
	- More sounds.
	- Implement first prototype of space warfare
	- Update textures for earth cube map and mars cube map

DONE:
	- Main menu objects.
	- Earth grid creation.
	- Covert mouse from 2D to 3D coordinates.
	- Get mouse's grid co-ordinate.
	- Know which grid (earth or mars) the mouse is in.
	- Know when the mouse is not pointing at any grid.
	- Building selection for placement.
	- Building placement.
	- Building picking after placement.
	- Each building takes a set amount of time to be built after being placed.
	- Background music for main menu and in the world state.
	- Particle effects written.

BUGS:
	//----------------
	- Desc:					
	- Possible File:
	- Possible Location:
	//-----------------

NOTES:
	- Entity interface class
		- Derived classes for units and buildings
		- Further derived classes for units and buildings
	- 2D grid for buildable areas on Earth / Mars
	- 3D grid for space battle
	- Space battle outcome calculated in real-time rather than pre-determined
	- Handling Player classes during state changes
*/


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "GameStateControl.h"


//-----------------------------------------------------
// MAIN GLOBALS
//-----------------------------------------------------

// Game Manager
//----------------------
CStateControl* gpStateController;


//-----------------------------------------------------
// FUNCTION PROTOTYPE DECLERATIONS
//-----------------------------------------------------

// Engine-specific
//----------------------
void SetupEngine();
void CleanupEngine();

// Scene
//----------------------
void Initialise();
void Cleanup();


//-----------------------------------------------------
// MAIN
//-----------------------------------------------------
void main()
{
	// ENGINE SETUP & INITIALISATION
	//------------------------------
	SetupEngine();
	Initialise();

	
	// FRAMETIME
	//------------------------------
	gpEngine->Timer();


	// WINDOW TITLE
	//------------------------------
	float titleChangeCounter = 1.0f;
	const std::string GAME_NAME = "Defiant Worlds";
	std::stringstream caption;


	// MAIN PROGRAM LOOP
	//------------------------------
	while (gpEngine->IsRunning())
	{
		// UPDATE CURRENT STATE
		//------------------------------
		gFrameTime = gpEngine->Timer();

		// Check if there has been a state change
		gpStateController->Update();

		// Call the update for the current state
		gpStateController->GetCurrentState()->StateUpdate();

		// Update window caption
		if (titleChangeCounter >= 1.0f)
		{
			caption << GAME_NAME << "    FT: " << gFrameTime << "    FPS: " << 1.0f / gFrameTime;
			gpEngine->SetWindowCaption(caption.str().c_str());
			caption.str("");	// Clear the string stream
			titleChangeCounter = 0.0f;
		}
		else
		{
			titleChangeCounter += gFrameTime;
		}


		// GLOBAL KEY PRESSES
		//------------------------------
		if (gpEngine->KeyHit(Key_Escape))
		{
			gpEngine->Stop();
		}
	}

	Cleanup();
}


//-----------------------------------------------------
// FUNCTION DEFINITIONS
//-----------------------------------------------------

// Engine-specific
//----------------------
void SetupEngine()
{
	// Do we want to define our own resolution? Maybe fullscreen?
	gpEngine->StartWindowed(WINDOW_WIDTH, WINDOW_HEIGHT);
	gpEngine->AddMediaFolder("..\\Media\\Models\\Skybox");
	gpEngine->AddMediaFolder("..\\Media\\Models\\Planets");
	gpEngine->AddMediaFolder("..\\Media\\Models\\Structures");
	gpEngine->AddMediaFolder("..\\Media\\Models\\Grid");
	gpEngine->AddMediaFolder("..\\Media\\Models\\Particles");

	SetWindowPos((HWND)gpEngine->GetWindow(), HWND_TOP, WINDOW_POS_X, WINDOW_POS_Y, WINDOW_WIDTH, WINDOW_HEIGHT, 0U);

	// Load static meshes
	CTile::mspMshTile = gpEngine->LoadMesh("Tile.x");

	CComCentre::mspMshStructureBuilt = gpEngine->LoadMesh("ComCentreBuilt.x");
	CComCentre::mspMshStructurePlacing = gpEngine->LoadMesh("ComCentrePlacing.x");
	CBarracks::mspMshStructureBuilt = gpEngine->LoadMesh("BarracksBuilt.x");
	CBarracks::mspMshStructurePlacing = gpEngine->LoadMesh("BarracksPlacing.x");
	CHellipad::mspMshStructureBuilt = gpEngine->LoadMesh("HellipadBuilt.x");
	CHellipad::mspMshStructurePlacing = gpEngine->LoadMesh("HellipadPlacing.x");
	CSpaceCentre::mspMshStructureBuilt = gpEngine->LoadMesh("SpaceCentreBuilt.x");
	CSpaceCentre::mspMshStructurePlacing = gpEngine->LoadMesh("SpaceCentrePlacing.x");
}

void CleanupEngine()
{
	// Cleanup engine
	gpEngine->Delete();
}


// Scene
//----------------------
void Initialise()
{
	// Initialise states
	gpStateController = new CStateControl(GS_MAIN_MENU);
}

void Cleanup()
{
	// Cleanup variables

	// Cleanup engine
	CleanupEngine();
}
