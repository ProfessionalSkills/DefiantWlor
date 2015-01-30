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

BUGS:
	//----------------
	- Desc:
	- Poss. File:
	- Poss. Location:
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


	// MAIN PROGRAM LOOP
	//------------------------------
	while (gpEngine->IsRunning())
	{
		// UPDATE CURRENT STATE
		//------------------------------
		delta = gpEngine->Timer();
		gpStateController->GetCurrentState()->StateUpdate(delta);


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
	gpEngine->StartWindowed();
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
