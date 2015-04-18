#ifndef _SMOKE_H
#define _SMOKE_H

#include "ParticleEmitter.h"

//using namespace std;
const DX::XMFLOAT3 kVelocity{ 1.0f, 1.5f, 1.0f };
const float kEmitTime = 0.04f;
const float kScaleIncrement = 0.0f;
const float kSmokeLifeTime = 2.5f;
class CSmoke : public CParticleEmitter
{
protected:
	float mEmitterCountdown;
	float mScale;
public:
	CSmoke(IModel* emitter, int particleNumber, EQueueObjectType objectType);
	~CSmoke();

	void EmitParticle();
	void SetEmitPosition(float x, float y, float z);
	bool UpdateSystem();
};

#endif