//-----------------------------------------------------
// FILE: ParticleEmitter.h
//-----------------------------------------------------

#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Common.h"

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
	vector<float> test;

	//EMITTER MODEL SOURCE
	IModel* parent;
	
	// VIRTUAL METHODS
	//---------------------------
	void UpdateSystem();

};


#endif