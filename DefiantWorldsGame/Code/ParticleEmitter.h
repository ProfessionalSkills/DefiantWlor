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
const int mParticleNumber = 100;
//-----------------------------------------------------
// PARTICLE EMITTER CLASS
//-----------------------------------------------------
class CParticleEmitter
{
protected:
	// EMITTER DATA
	//---------------------------
	vector<CParticle*> mParticles;
	DX::XMFLOAT3 mParticleVelocity;
	DX::XMFLOAT3 mParticleOrigen;
	//EMITTER MODEL SOURCE
	IModel* mEmitter;
public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CParticleEmitter();
	~CParticleEmitter();

	// VIRTUAL METHODS
	//---------------------------
	virtual bool UpdateSystem();
	virtual void EmitParticle();
	virtual void SetEmitPosition();
};


#endif