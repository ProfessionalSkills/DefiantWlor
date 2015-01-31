//-----------------------------------------------------
// FILE: Main.cpp
//-----------------------------------------------------

/*
PROJECT NAME:
ProfSkillsDefiantWorlds

TODO:
	- Main menu objects
	- Main menu UI
	- Create Earth world
	- Create Mars world
	- Create Space 'fight' area
	- Create debug camera to cycle between the 3 areas
	- Get high res & open source texture of Earth. Currently just a high res placeholder.

BUGS:
	//----------------
	- Desc:
	- Possible File:
	- Possible Location:
	//-----------------
*/


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "GameStateControl.h"


//-----------------------------------------------------
// MAIN GLOBALS
//-----------------------------------------------------

// Game State
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
	float delta = 0.0f;
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
		delta = gpEngine->Timer();

		// Check if there has been a state change
		gpStateController->Update();

		// Call the update for the current state
		gpStateController->GetCurrentState()->StateUpdate(delta);

		// Update window caption
		if (titleChangeCounter >= 1.0f)
		{
			caption << GAME_NAME << "    FT: " << delta << "    FPS: " << 1.0f / delta;
			gpEngine->SetWindowCaption(caption.str().c_str());
			caption.str("");	// Clear the string stream
			titleChangeCounter = 0.0f;
		}
		else
		{
			titleChangeCounter += delta;
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
	gpEngine->StartWindowed(1600, 960);
	gpEngine->AddMediaFolder("..\\Media\\Models\\Skybox");
	gpEngine->AddMediaFolder("..\\Media\\Models\\Planets");
}

void CleanupEngine()
{
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
