
#ifndef _EXPLOSION_H
#define _EXPLOSION_H

#include "ParticleEmitter.h"
const int kParticleNumber = 100;
const float kExplosionLifeTime = 1.5f;
const float kExplosionVelocity = 10.0f;

class CExplosion : public CParticleEmitter
{
protected:

public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CExplosion(IModel* emitter, float particleNumber);
	CExplosion(DX::XMFLOAT3 emitterPos, float particleNumber);
	~CExplosion();

	//METHODS
	//---------------------------
	void EmitParticle();
	void SetEmitPosition();
	bool UpdateSystem();
};

#endif
