//-----------------------------------------------------
// FILE: Common.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Common.h"


//-----------------------------------------------------
// DEFINES
//-----------------------------------------------------
// Actual initialisation of extern pointer variable.
// extern = will be defined elsewhere, but I want anything that includes this to be aware it
// will exist at some point.
I3DEngine*   gpEngine = New3DEngine(kTLX);
CRandomiser* gpRandomiser = new CRandomiser();
EGameStates  gCurState = GS_MAIN_MENU;
float        gFrameTime = 0.0f;

//-----------------------------------------------------
// FUNCTION DEFINITIONS
//-----------------------------------------------------
float Clampf(float inMin, float inMax, float inVal)
{
	if (inVal < inMin)
	{
		// Too small - return minimum
		return inMin;
	}
	else if (inVal > inMax)
	{
		// too great - return maximum
		return inMax;
	}

	// Values is fine
	return inVal;
}

float ToRadians(float inDegrees)
{
	return (PI / 180.0f) * inDegrees;
}

float ToDegrees(float inRadians)
{
	return (180 / PI) * inRadians;
}
