
#ifndef _EXPLOSION_H
#define _EXPLOSION_H

#include "ParticleEmitter.h"
const int kParticleNumber = 100;
const float kExplosionLifeTime = 1.5f;
const float kExplosionVelocity = 10.0f;

class CExplosion : public CParticleEmitter
{
protected:
	// Stores whether or not the explosion has an exploding ball effect
	bool mHasExplodingBall = false;
	float mExplosionBallSize = 0.0f;
	IModel* mpMdlExplosionBall = nullptr;

public:
	static IMesh* mspMshExplosionBall;

	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CExplosion(DX::XMFLOAT3 emitterPos, float particleNumber, bool ball);
	~CExplosion();

	//METHODS
	//---------------------------
	void EmitParticle();
	bool UpdateSystem();
};

#endif
