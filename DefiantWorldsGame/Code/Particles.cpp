//-----------------------------------------------------
// FILE: Particles.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Particles.h"

I3DEngine*   gpEngine = New3DEngine(kTLX);
IMesh* CParticle::mspMshParticle = nullptr;


//-----------------------------------------------------
// PARTICLE EMITTER CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------

CParticle::CParticle() 
{

}

CParticle::~CParticle()
{
	mspMshParticle->RemoveModel(mModel);
}