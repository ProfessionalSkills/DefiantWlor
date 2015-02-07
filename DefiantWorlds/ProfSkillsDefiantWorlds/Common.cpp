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

DX::XMVECTOR XMMinusVectors(DX::XMVECTOR vec1, DX::XMVECTOR vec2)
{
	DX::XMFLOAT3 v1;
	DX::XMFLOAT3 v2;
	DX::XMFLOAT3 vResult;

	DX::XMStoreFloat3(&v1, vec1);
	DX::XMStoreFloat3(&v2, vec2);

	vResult.x = v1.x - v2.x;
	vResult.y = v1.y - v2.y;
	vResult.z = v1.z - v2.z;

	return DX::XMLoadFloat3(&vResult);
}
