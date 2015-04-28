//-----------------------------------------------------
// FILE: SphericalCamera.h
//-----------------------------------------------------
#ifndef _SPHERICAL_CAMERA_H_
#define _SPHERICAL_CAMERA_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Particles.h"

const float NEAR_CLIP = 1.0f;
const float FAR_CLIP = 10000.0f;

// Game factions
enum EFactions
{
	FAC_EARTH_DEFENSE_FORCE, FAC_THE_CRIMSON_LEGION, FAC_REBELS
};

// Clamp function
float Clampf(float inMin, float inMax, float inVal);


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
	inline void SetPivotPoint(DX::XMFLOAT3& point)
	{
		mPivotPoint = point;
	}

	inline void AdjustPivotPoint(DX::XMFLOAT3& amount)
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
		mφ += amount / 2.0f;
	}

	inline void SetTheta(float θ)
	{
		mθ = θ;
	}

	inline void AdjustTheta(float amount)
	{
		mθ = Clampf(DX::XMConvertToRadians(10.0f), DX::XMConvertToRadians(50.0f), mθ + amount / 2.0f);
	}

	inline void SetShaking(bool shake)
	{
		mShaking = shake;
		mShakingTimer = 0.0f;
	}

	inline void SetFaction(EFactions faction)
	{
		mFaction = faction;
	}


	// ACCESSORS
	//---------------------------
	inline ICamera* GetCamera()
	{
		return mpCamera;
	}

	inline void GetPivotPoint(DX::XMFLOAT3& point)
	{
		point = mPivotPoint;
	}

	inline EFactions GetFaction()
	{
		return mFaction;
	}


	// METHODS
	//---------------------------
	void Update();


private:
	// CAMERA VARIABLES
	//---------------------------
	ICamera* mpCamera;
	DX::XMFLOAT3 mPivotPoint;
	EFactions mFaction;
	float mρ;
	float mφ;
	float mθ;
	bool mShaking = false;
	float mShakingTimer = 0.0f;
};

#endif /* _SPHERICAL_CAMERA_H_ */
