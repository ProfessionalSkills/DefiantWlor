//-----------------------------------------------------
// FILE: Main.cpp
//-----------------------------------------------------

/*
PROJECT NAME:
ProfSkillsDefiantWorlds


TODO:
	- Create Space 'fight' area
	- Get high res & open source texture of Earth. Currently just a high res placeholder.
	- Get the queue working for each building type.
	- Particle effects need to be fully implemented.
	- More sounds.
	- Implement first prototype of space warfare
	- Unit movement

DONE:
	- Main menu objects.
	- main menu UI
	- Earth grid creation.
	- Mars grid creation
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
	- Units can be built

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
	//setting up windows
	gpEngine->StartWindowed(WINDOW_WIDTH, WINDOW_HEIGHT);
	SetWindowPos((HWND)gpEngine->GetWindow(), HWND_TOP, WINDOW_POS_X, WINDOW_POS_Y, WINDOW_WIDTH, WINDOW_HEIGHT, 0U);
	ShowCursor(false);

	//Get Media Folders
	gpEngine->AddMediaFolder("..\\Media\\Models\\Skybox");
	gpEngine->AddMediaFolder("..\\Media\\Models\\Planets");
	gpEngine->AddMediaFolder("..\\Media\\Models\\Structures");
	gpEngine->AddMediaFolder("..\\Media\\Models\\Grid");
	gpEngine->AddMediaFolder("..\\Media\\Models\\Particles");
	gpEngine->AddMediaFolder("..\\Media\\Models\\Units");
	gpEngine->AddMediaFolder("..\\Media\\Models\\Units\\Vehicles\\SciFi");
	gpEngine->AddMediaFolder("..\\Media\\Models\\Units\\Vehicles\\Military");
	gpEngine->AddMediaFolder("..\\Media\\Models\\Units\\Aircraft\\SciFi");
	gpEngine->AddMediaFolder("..\\Media\\Models\\Units\\Aircraft\\Modern");
	gpEngine->AddMediaFolder("..\\Media\\User Interface");

	
	// Load static meshes
	CTile::mspMshTile = gpEngine->LoadMesh("Tile.x");
	CWorldState::mspMshDrag = gpEngine->LoadMesh("DragSelection.x");
	CMinerals::mspMshMineral = gpEngine->LoadMesh("Mineral2.x");

	//Structure Meshes
	//Command Centre
	CComCentre::mspMshStructureBuilt = gpEngine->LoadMesh("ComCentreBuilt.x");
	CComCentre::mspMshStructurePlacing = gpEngine->LoadMesh("ComCentrePlacing.x");
	//Barracks
	CBarracks::mspMshStructureBuilt = gpEngine->LoadMesh("BarracksBuilt.x");
	CBarracks::mspMshStructurePlacing = gpEngine->LoadMesh("BarracksPlacing.x");
	//Helipad
	CHellipad::mspMshStructureBuilt = gpEngine->LoadMesh("HellipadBuilt.x");
	CHellipad::mspMshStructurePlacing = gpEngine->LoadMesh("HellipadPlacing.x");
	//Space Centre
	CSpaceCentre::mspMshStructureBuilt = gpEngine->LoadMesh("SpaceCentreBuilt.x");
	CSpaceCentre::mspMshStructurePlacing = gpEngine->LoadMesh("SpaceCentrePlacing.x");
	// Wall
	CWall::mspMshStructure = gpEngine->LoadMesh("wall_A1_01.x");

	
	//Spaceship Meshes
	//Space Fighter
	CSpaceFighter::mspMshSpaceFighter = gpEngine->LoadMesh("SciFiBattleship01.x");
	CSpaceFighter::mspMshSheild = gpEngine->LoadMesh("SciFiBattleship01Shield.x");
	//Mothership
	CMothership::mspMshMothership = gpEngine->LoadMesh("Spaceship02Battlecruiser.x");
	CMothership::mspMshSheild = gpEngine->LoadMesh("Spaceship02BattlecruiserShield.x");
	//Transport
	CTransport::mspMshTransport = gpEngine->LoadMesh("Spaceship01Battlecruiser.x");
	CTransport::mspMshSheild = gpEngine->LoadMesh("Spaceship01BattlecruiserShield.x");

	//lazer
	CSpaceUnit::mspMshLazer = gpEngine->LoadMesh("Lazer.x");

	//Ground Unit Meshes
	//Bomber
	CBomber::mspMshBomber = gpEngine->LoadMesh("HeliScout.x");
	//Fighter
	CFighter::mspMshFighter = gpEngine->LoadMesh("Helicopter.x");
	//Worker
	CWorker::mspMshWorker = gpEngine->LoadMesh("transportTruck_german.x");
	//Tank
	CTank::mspMshTank = gpEngine->LoadMesh("HoverTank01.x");
	//Infantry
	CInfantry::mspMshInfantry = gpEngine->LoadMesh("tigerAusfH.x");
	//Artillery
	CArtillery::mspMshArtillery = gpEngine->LoadMesh("marsAA.x");
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
	gpStateController = CStateControl::GetInstance();
}

void Cleanup()
{
	// Cleanup variables
	ShowCursor(true);

	// Cleanup engine
	CleanupEngine();
}
