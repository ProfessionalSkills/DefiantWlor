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

	mpCamera = gpEngine->CreateCamera(kManual);
	mpCamera->SetNearClip(NEAR_CLIP);
	mpCamera->SetFarClip(FAR_CLIP);
}

CSphericalCamera::~CSphericalCamera()
{
	gpEngine->RemoveCamera(mpCamera);
}


//-----------------------------------------------------
// SLIDER TOOL CLASS METHODS
//-----------------------------------------------------
void CSphericalCamera::Update()
{
	// Check if the camera is shaking
	if (mShaking)
	{
		// Randomly move the pivot point around
		AdjustPhi(gpRandomiser->GetRandomFloat(-3.0f, 3.0f) * gFrameTime);
		AdjustRho(gpRandomiser->GetRandomFloat(-3.0f, 3.0f) * gFrameTime);
		AdjustTheta(gpRandomiser->GetRandomFloat(-3.0f, 3.0f) * gFrameTime);

		// Increase shaking timer
		mShakingTimer += gFrameTime;
		if (mShakingTimer > 0.5f)
		{
			mShaking = false;
			mShakingTimer = 0.0f;
		}
	}
	
	// Calculate the camera's position based on the pivot point
	float x = mρ * sinf(mθ) * cosf(mφ);
	x += mPivotPoint.x;
	float y = mρ * cosf(mθ);
	float z = mρ * sinf(mθ) * sinf(mφ);
	z += mPivotPoint.z;

	// Set camera's new position
	mpCamera->SetPosition(x, y, z);

	mpCamera->LookAt(mPivotPoint.x, mPivotPoint.y, mPivotPoint.z);
}

// Clamp function
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
