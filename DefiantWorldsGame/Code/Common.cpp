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
CNewsTicker* gpNewsTicker = nullptr;
EGameStates gCurState = GS_MAIN_MENU;
ICamera* gpCam = nullptr;
CSphericalCamera* gpCurWorldCamera = nullptr;
float gMouseWheelDelta = 0.0f;

//-----------------------------------------------------
// FUNCTION DEFINITIONS
//-----------------------------------------------------
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

float distance(float x, float y, float z)
{
	return ((x * x) + (y * y) + (z * z));
}

bool BoxCollision(const DX::XMFLOAT3 v1, const DX::XMFLOAT3 v2, float radius)
{
	if (v1.x > (v2.x + radius) || v1.x < (v2.x - radius))
	{
		return false;
	}
	else if (v1.y >(v2.y + radius) || v1.y < (v2.y - radius))
	{
		return false;
	}
	else if (v1.z >(v2.z + radius) || v1.z < (v2.z - radius))
	{
		return false;
	}
	else
	{
		return true;
	}
}