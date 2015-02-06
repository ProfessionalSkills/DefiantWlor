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
using namespace tle;


//-----------------------------------------------------
// PARTICLE CLASS
//-----------------------------------------------------
class CParticle
{
protected:
	// PARTICLE DATA
	//---------------------------
	IModel* mModel;
	float mMoveVector[3];		// HIGGY:  You can change this to a DX::XMFLOAT3 variable. Uses intrinsics (calculations with multiple floats in one tick) - much like D3DXVECTOR3 you use in graphics, it contains an x, y, and z set of variables ;)
	float lifeTime;
};

#endif
