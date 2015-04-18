//-----------------------------------------------------
// FILE: Particles.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Particles.h"

I3DEngine*   gpEngine = New3DEngine(kTLX);
IMesh* CParticle::mspMshExplosionParticle = nullptr;
IMesh* CParticle::mspMshSmokeParticle = nullptr;

//-----------------------------------------------------
// PARTICLE EMITTER CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------

CParticle::CParticle() 
{

}

CParticle::~CParticle()
{
	
}