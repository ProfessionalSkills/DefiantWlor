#ifndef _SMOKE_H
#define _SMOKE_H

#include "ParticleEmitter.h"

const float kScaleIncrement = 0.0f;
const float kSmokeLifeTime = 2.5f;

class CSmoke : public CParticleEmitter
{
protected:
	float mEmitterCountdown;
	float mScale;
	float mRelativeHeight;			// Additional height on top of the current emitter height

public:
	CSmoke(DX::XMFLOAT3 emitterPos, int particleNumber, float relativeHeight, float scale);
	~CSmoke();

	void EmitParticle();
	void SetEmitPosition(const DX::XMFLOAT3& emitterPos);
	bool UpdateSystem();
	bool UpdateSystem(const DX::XMFLOAT3& emitterPos);
};

#endif