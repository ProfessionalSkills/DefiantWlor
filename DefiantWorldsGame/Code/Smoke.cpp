#include "Smoke.h"

CSmoke::CSmoke(IModel* emitter, IMesh* particleMesh)
{
	rand = new CRandomiser();
	mEmitter = emitter;
	SetEmitPosition();
	mEmitterPeriod = 0.04f;
	mEmitterCountdown = mEmitterPeriod;
	EmitParticle(particleMesh);
}

CSmoke::~CSmoke()
{

}

void CSmoke::SetEmitPosition()
{
	mEmitPosition = { mEmitter->GetX(), mEmitter->GetY(), mEmitter->GetZ() };
}

DirectX::XMFLOAT3 CSmoke::GetEmitPosition()
{
	return mEmitPosition;
}

void CSmoke::EmitParticle(IMesh* particleMesh)
{
	CParticle* mNewParticle = new CParticle();
	mNewParticle->mMesh = particleMesh;

	float mPosX = rand->GetRandomFloat(mEmitPosition.x - 3.0f, mEmitPosition.x + 3.0f);
	float mPosZ = rand->GetRandomFloat(mEmitPosition.z - 3.0f, mEmitPosition.z + 3.0f);

	mNewParticle->mModel = mNewParticle->mMesh->CreateModel(mPosX, mEmitPosition.y, mPosZ);
	mNewParticle->mModel->Scale(0.3f);


	mNewParticle->mMoveVector.x = rand->GetRandomFloat(-2.0f, 2.0f);
	mNewParticle->mMoveVector.y = 5.0f;
	mNewParticle->mMoveVector.z = rand->GetRandomFloat(-2.0f, 2.0f);

	mNewParticle->mPosition = mEmitPosition;
	mNewParticle->mLifeTime = 3.5f;
	Particles.push_back(mNewParticle);
}

void CSmoke::UpdateSystem(float mFrameTime, IModel* emitter, IMesh* particleMesh, ICamera* myCamera)
{

	if (mEmitterCountdown <= 0.0f)
	{
		EmitParticle(particleMesh);
		mEmitterCountdown = mEmitterPeriod;
	}

	vector<CParticle*>::iterator itParticle = Particles.begin();

	while (itParticle != Particles.end())
	{
		(*itParticle)->mModel->MoveX((*itParticle)->mMoveVector.x * mFrameTime);
		(*itParticle)->mModel->MoveY((*itParticle)->mMoveVector.y * mFrameTime);
		(*itParticle)->mModel->MoveZ((*itParticle)->mMoveVector.z * mFrameTime);

		(*itParticle)->mLifeTime -= mFrameTime;

		if ((*itParticle)->mLifeTime <= 0.0f)
		{
			delete(*itParticle);
			itParticle = Particles.erase(itParticle);
		}
		else
		{
			itParticle++;
		}
	}

	mEmitterCountdown -= mFrameTime;
}
