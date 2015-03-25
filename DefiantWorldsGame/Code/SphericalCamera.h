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
	CSphericalCamera(DX::XMFLOAT3 pivotPoint, float radius, float phi, float theta);
	~CSphericalCamera();


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
