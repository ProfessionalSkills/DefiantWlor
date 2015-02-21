#ifndef _SMOKE_H
#define _SMOKE_H

#include "Particles.h"

using namespace tle;
//using namespace std;

class CSmoke
{
protected:
	IModel* mEmitter;
	DirectX::XMFLOAT3 mEmitPosition;
	vector<CParticle*> Particles;
	CRandomiser* rand;
	float mEmitterCountdown;
	float mEmitterPeriod;

public:

	CSmoke(IModel* emitter, IMesh* particleMesh);
	~CSmoke();

	void EmitParticle(IMesh* particleMesh);
	void SetEmitPosition();
	DirectX::XMFLOAT3 GetEmitPosition();
	void UpdateSystem(float mFrameTime, IModel* emitter, IMesh* particleMesh, ICamera* myCamera);
};

#endif