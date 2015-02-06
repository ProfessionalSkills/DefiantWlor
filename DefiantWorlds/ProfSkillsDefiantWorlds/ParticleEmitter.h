//-----------------------------------------------------
// FILE: ParticleEmitter.h
//-----------------------------------------------------

#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Particles.h"


//-----------------------------------------------------
// CONSTANTS
//-----------------------------------------------------
const int MAX_PARTICLE_NUMBER = 1000;

//-----------------------------------------------------
// PARTICLE EMITTER CLASS
//-----------------------------------------------------
class CParticleEmitter
{
protected:
	// EMITTER DATA
	//---------------------------
	int mParticleNumber;
	float mParticleSpeed;
	vector<CParticle> mParticles;

	//EMITTER MODEL SOURCE
	IModel* parent;
	
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CParticleEmitter();
	~CParticleEmitter();


	// VIRTUAL METHODS
	//---------------------------
	virtual void UpdateSystem();
};


#endif