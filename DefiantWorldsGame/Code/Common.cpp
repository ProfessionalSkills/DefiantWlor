//-----------------------------------------------------
// FILE: Common.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Common.h"
#include "NewsTicker.h"


//-----------------------------------------------------
// DEFINES
//-----------------------------------------------------
// Actual initialisation of extern pointer variable.
// extern = will be defined elsewhere, but I want anything that includes this to be aware it
// will exist at some point.
CRandomiser* gpRandomiser = new CRandomiser();
CNewsTicker* gpNewsTicker = nullptr;
EGameStates  gCurState = GS_MAIN_MENU;
float        gFrameTime = 0.0f;
float        gMouseWheelDelta = 0.0f;

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


float Dot(const DX::XMFLOAT3 v1, const DX::XMFLOAT3 v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

DX::XMFLOAT3 Cross(const  DX::XMFLOAT3 v1, const  DX::XMFLOAT3 v2)
{
	return  DX::XMFLOAT3(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}
