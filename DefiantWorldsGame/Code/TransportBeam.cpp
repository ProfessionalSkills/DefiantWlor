//-----------------------------------------------------
// FILE: TransportBeam.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "TransportBeam.h"

IMesh* CTransportBeam::mspMshBeam = nullptr;


//-----------------------------------------------------
// TRANSPORT BEAM CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CTransportBeam::CTransportBeam(const DX::XMFLOAT3& position)
{
	// Create the beam at the location
	mpMdlBeam = mspMshBeam->CreateModel(position.x, 0.0f, position.z);

	// Scale the beam to the sky and make it fatter
	mpMdlBeam->ScaleY(300.0f);
	mpMdlBeam->ScaleX(mCounter);
	mpMdlBeam->ScaleZ(mCounter);
	mpMdlBeam->SetSkin("tlxadd_lazer - red.tga");
}

CTransportBeam::~CTransportBeam()
{
	// Remove the beam
	mspMshBeam->RemoveModel(mpMdlBeam);
}


//-----------------------------------------------------
// TRANSPORT BEAM CLASS METHODS
//-----------------------------------------------------
bool CTransportBeam::Update()
{
	// Increment timer
	mCounter -= gFrameTime * 2.0f;
	
	// Simple scaling & rotating animation
	mpMdlBeam->RotateY(100.0f * gFrameTime);
	mpMdlBeam->ResetScale();
	mpMdlBeam->ScaleY(300.0f);
	mpMdlBeam->ScaleX(mCounter);
	mpMdlBeam->ScaleZ(mCounter);

	// Check timer
	if (mCounter <= 0.0f) return false;

	return true;
}
