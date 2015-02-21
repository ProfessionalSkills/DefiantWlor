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
CRandomiser* gpRandomiser = new CRandomiser();
EGameStates  gCurState = GS_MAIN_MENU;
float        gFrameTime = 0.0f;
short        gMouseWheelDelta = 0;


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
	return (DX::XM_PI / 180.0f) * inDegrees;
}

float ToDegrees(float inRadians)
{
	return (180.0f / DX::XM_PI) * inRadians;
}
