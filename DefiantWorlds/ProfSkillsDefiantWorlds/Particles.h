//-----------------------------------------------------
// FILE: Particles.h
//-----------------------------------------------------

#ifndef _PARTICLES_H
#define _PARTICLES_H


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include <TL-Engine.h>			// TL-Engine stuff
#include "Randomiser.h"
#include "Common.h"
using namespace tle;

//-----------------------------------------------------
// PARTICLE CLASS
//-----------------------------------------------------
class CParticle
{
protected:
	// PARTICLE DATA
	//---------------------------
	IMesh* mMesh;
	IModel* mModel;
	DX::XMFLOAT3 mMoveVector; //Thank Kyle :)
	string mSkin;
	float mLifeTime;

	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CParticle(DX::XMFLOAT3 position);
	~CParticle();
};

IMesh* CParticle::mMesh = I3DEngine->LoadMesh("particle.x");

#endif
