//-----------------------------------------------------
// FILE: Particles.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Particles.h"

I3DEngine*   gpEngine = New3DEngine(kTLX);
IMesh* CParticle::mMesh = nullptr;


//-----------------------------------------------------
// PARTICLE EMITTER CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------

CParticle::CParticle() 
{

}

CParticle::~CParticle()
{
	//mMesh->RemoveModel(mModel);
}