//-----------------------------------------------------
// FILE: Particles.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Particles.h"

I3DEngine*   gpEngine = New3DEngine(kTLX);
CRandomiser* gpRandomiser = new CRandomiser();
float        gFrameTime = 0.0f;

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