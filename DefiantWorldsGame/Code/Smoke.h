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
	float mWidth;
	float mLength;

public:
	CSmoke(DX::XMFLOAT3 emitterPos, float relativeHeight, float scale, float width, float length);
	~CSmoke();

	void EmitParticle();
	void SetEmitPosition(const DX::XMFLOAT3& emitterPos);
	bool UpdateSystem();
	bool UpdateSystem(const DX::XMFLOAT3& emitterPos);
};

#endif