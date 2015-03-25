//-----------------------------------------------------
// FILE: SphericalCamera.h
//-----------------------------------------------------
#ifndef _SPHERICAL_CAMERA_H_
#define _SPHERICAL_CAMERA_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Common.h"


//-----------------------------------------------------
// SPHERICAL CAMERA CLASS
//-----------------------------------------------------
class CSphericalCamera
{
public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CSphericalCamera(DX::XMFLOAT3 pivotPoint, float rho, float phi, float theta);
	~CSphericalCamera();


	// MUTATORS
	//---------------------------
	inline void SetPivotPoint(DX::XMFLOAT3 point)
	{
		mPivotPoint = point;
	}

	inline void AdjustPivotPoint(DX::XMFLOAT3 amount)
	{
		mPivotPoint.x += amount.x;
		mPivotPoint.y += amount.y;
		mPivotPoint.z += amount.z;
	}

	inline void SetRho(float ρ)
	{
		mρ = ρ;
	}

	inline void AdjustRho(float amount)
	{
		mρ = Clampf(50.0f, 400.0f, mρ + amount);
	}

	inline void SetPhi(float φ)
	{
		mφ = φ;
	}

	inline void AdjustPhi(float amount)
	{
		mφ += amount;
	}

	inline void SetTheta(float θ)
	{
		mθ = θ;
	}

	inline void AdjustTheta(float amount)
	{
		mθ = Clampf(10.0f, 80.0f, mθ + amount);
	}


	// ACCESSORS
	//---------------------------
	inline ICamera* GetCamera()
	{
		return mpCamera;
	}


	// METHODS
	//---------------------------
	void Update();


private:
	// CAMERA VARIABLES
	//---------------------------
	ICamera* mpCamera;
	DX::XMFLOAT3 mPivotPoint;
	float mρ;
	float mφ;
	float mθ;
};

#endif /* _SPHERICAL_CAMERA_H_ */
