
#ifndef _EXPLOSION_H
#define _EXPLOSION_H

#include "ParticleEmitter.h"

class CExplosion : public CParticleEmitter
{
protected:
	// EXPLOSION DATA
	//---------------------------
	IModel* mEmitter; //Source of the Particle System
	DirectX::XMFLOAT3 mEmitPosition; //The initial Emitting position of the Particle
	vector<CParticle*> Particles; // Particles
	CRandomiser* rand; //Randomiser


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CExplosion(IModel* emitter, IMesh* particleMesh);
	~CExplosion();

	//METHODS
	//---------------------------
	void EmitParticle(IMesh* particleMesh);
	void SetEmitPosition();
	DirectX::XMFLOAT3 GetEmitPosition();
	void UpdateSystem(float mFrameTime, IModel* emitter, IMesh* particleMesh, ICamera* myCamera);
};

#endif
