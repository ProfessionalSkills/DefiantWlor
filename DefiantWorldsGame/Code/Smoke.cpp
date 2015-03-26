#include "Smoke.h"

CSmoke::CSmoke(IModel* emitter)
{
	mEmitter = emitter;
	SetEmitPosition();
	mEmitterCountdown = kEmitTime;
	EmitParticle();
}

CSmoke::~CSmoke()
{

}

void CSmoke::SetEmitPosition()
{
	mParticleOrigen = { mEmitter->GetX(), mEmitter->GetY(), mEmitter->GetZ() };
}

void CSmoke::EmitParticle()
{
	CParticle* mNewParticle = new CParticle();

	float mPosX = gpRandomiser->GetRandomFloat(mParticleOrigen.x - 3.0f, mParticleOrigen.x + 3.0f);
	float mPosZ = gpRandomiser->GetRandomFloat(mParticleOrigen.z - 3.0f, mParticleOrigen.z + 3.0f);

	mNewParticle->mModel = mNewParticle->mspMshParticle->CreateModel(mPosX, mParticleOrigen.y, mPosZ);
	mNewParticle->SetScale(0.7f);

	mNewParticle->SetVector(gpRandomiser->GetRandomFloat(-kVelocity.x, kVelocity.x), kVelocity.y, gpRandomiser->GetRandomFloat(-kVelocity.z, kVelocity.z));

	mNewParticle->SetPosition(mParticleOrigen);
	mNewParticle->SetLifeTime(kSmokeLifeTime);
	mParticles.push_back(mNewParticle);
}

bool CSmoke::UpdateSystem()
{
	if (mParticles.size() == 0)
	{
		return false;
	}
	if (mEmitterCountdown <= 0.0f)
	{
		if (mParticles.size() <= mParticleNumber)
		{
			EmitParticle();
			mEmitterCountdown = kEmitTime;
		}
	}

	vector<CParticle*>::iterator itParticle = mParticles.begin();

	while (itParticle != mParticles.end())
	{
		(*itParticle)->mModel->MoveX((*itParticle)->GetMoveVector().x * gFrameTime);
		(*itParticle)->mModel->MoveY((*itParticle)->GetMoveVector().y * gFrameTime);
		(*itParticle)->mModel->MoveZ((*itParticle)->GetMoveVector().z * gFrameTime);

		(*itParticle)->SetLifeTime((*itParticle)->GetLifeTime() - gFrameTime);

		if ((*itParticle)->GetLifeTime() <= 0.0f)
		{
			(*itParticle)->SetPosition(mParticleOrigen);

		}
		else
		{
			itParticle++;
		}
	}
	mEmitterCountdown -= gFrameTime;
	return true;
}
