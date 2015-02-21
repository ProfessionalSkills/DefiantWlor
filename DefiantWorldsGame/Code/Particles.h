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


extern I3DEngine* gpEngine;

//-----------------------------------------------------
// PARTICLE CLASS
//-----------------------------------------------------
class CParticle
{
protected:

public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CParticle();
	~CParticle();


	// PARTICLE STATIC MESH
	//---------------------------
	static IMesh* mMesh;


	// PARTICLE DATA (ACCESSED BY PARTICLE SYSTEMS)
	//---------------------------
	IModel* mModel; //Particle Model
	DirectX::XMFLOAT3 mMoveVector; //3D Particle Movement
	DirectX::XMFLOAT3 mPosition; //Particle Position
	string mSkin; //Particle texture
	float mLifeTime; //Life time of Particle
};

//IMesh* CParticle::mMesh = gpEngine->LoadMesh("particle.x");

#endif
