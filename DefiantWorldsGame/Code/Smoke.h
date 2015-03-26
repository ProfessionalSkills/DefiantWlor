#ifndef _SMOKE_H
#define _SMOKE_H

#include "ParticleEmitter.h"

//using namespace std;
const DX::XMFLOAT3 kVelocity{ 1.0f, 1.5f, 1.0f };
const float kEmitTime = 0.04f;
const float kScaleIncrement = 0.0f;
const float kSmokeLifeTime = 3.5f;
class CSmoke : public CParticleEmitter
{
protected:
	float mEmitterCountdown;
public:
	CSmoke(IModel* emitter);
	~CSmoke();

	void EmitParticle();
	void SetEmitPosition();
	bool UpdateSystem();
};

#endif