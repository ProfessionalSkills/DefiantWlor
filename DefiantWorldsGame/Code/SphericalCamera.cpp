//-----------------------------------------------------
// FILE: SphericalCamera.cpp
//-----------------------------------------------------
#include "SphericalCamera.h"


//-----------------------------------------------------
// SLIDER TOOL CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CSphericalCamera::CSphericalCamera(DX::XMFLOAT3 pivotPoint, float radius, float phi, float theta)
{
	// Set the member variables
	mPivotPoint = pivotPoint;
	mρ = radius;
	mφ = phi;
	mθ = theta;
}

CSphericalCamera::~CSphericalCamera()
{
	
}


//-----------------------------------------------------
// SLIDER TOOL CLASS METHODS
//-----------------------------------------------------
void CSphericalCamera::Update()
{
	
}
