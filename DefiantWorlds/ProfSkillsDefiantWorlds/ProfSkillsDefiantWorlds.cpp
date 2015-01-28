/*
PROJECT NAME:
ProfSkillsDefiantWorlds

TODO:
	- Main menu objects
	- Main menu UI
	- Create Earth world
	- Create Mars world
	- Create Universe 'fight' area
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
#include "Common.h"


//-----------------------------------------------------
// MAIN
//-----------------------------------------------------
void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	gpEngine->StartWindowed();

	// Add default folder for meshes and other media
	gpEngine->AddMediaFolder("..\\Media");

	/**** Set up your scene here ****/


	// The main game loop, repeat until engine is stopped
	while (gpEngine->IsRunning())
	{
		// Draw the scene
		gpEngine->DrawScene();

		/**** Update your scene each frame here ****/

	}

	// Delete the 3D engine now we are finished with it
	gpEngine->Delete();
}
