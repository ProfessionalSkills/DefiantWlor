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
#include "FrontEnd.h"


//-----------------------------------------------------
// FUNCTION PROTOTYPE DECLERATIONS
//-----------------------------------------------------

// Engine-specific
//----------------------
void SetupEngine();


//-----------------------------------------------------
// MAIN
//-----------------------------------------------------
void main()
{
	// Engine setup
	SetupEngine();

	// The main game loop, repeat until engine is stopped
	while (gpEngine->IsRunning())
	{
		// Draw the scene
		gpEngine->DrawScene();
	}

	// Delete the 3D engine now we are finished with it
	gpEngine->Delete();
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
	gpEngine->AddMediaFolder("..\\Media");
}
