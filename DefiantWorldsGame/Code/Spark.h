
#ifndef _SPARK_H
#define _SPARK_H

#include "ParticleEmitter.h"
const float kSparkVelocity = 20.0f;
const float kSparkTimer = 0.02f;
class CSpark : public CParticleEmitter
{
protected:
	float mSparkCountdown;
public:

	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CSpark(DX::XMFLOAT3 emitterPos);
	~CSpark();

	//METHODS
	//---------------------------
	void EmitParticle();
	bool UpdateSystem();
};

#endif
