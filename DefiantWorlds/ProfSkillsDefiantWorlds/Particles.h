//-----------------------------------------------------
// FILE: Particles.h
//-----------------------------------------------------

#ifndef _PARTICLES_H
#define _PARTICLES_H


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include <TL-Engine.h>			// TL-Engine stuff
#include <DirectXMath.h>		
#include "Randomiser.h"

using namespace tle;

// Renamed the DirectX namespace so that DirectX:: is not required all the time. Simply use DX:: instead.
// e.g. Vector3 variable created by doing DX::XMFLOAT3 mPos; just as an example.
namespace DX = DirectX;


//-----------------------------------------------------
// PARTICLE CLASS
//-----------------------------------------------------
class CParticle
{
protected:
	// PARTICLE DATA
	//---------------------------
	static IMesh* mMesh;
	IModel* mModel;
	DX::XMFLOAT3 mMoveVector; //Thank Kyle :)
	string mSkin;
	float mLifeTime;

public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CParticle(DX::XMFLOAT3 position);
	~CParticle();
};

//IMesh* CParticle::mMesh = gpEngine->LoadMesh("particle.x");

#endif
