#include "Smoke.h"

CSmoke::CSmoke(IModel* emitter, int particleNumber, EQueueObjectType objectType)
{
	mEmitter = emitter;
	if (objectType == Q_BARRACKS || objectType == Q_HELLIPAD || objectType == Q_SPACE_CENTRE)
	{
		SetEmitPosition(emitter->GetX(), emitter->GetY() + 20.0f, emitter->GetZ());
		mScale = 1.0f;
	}
	else
	{
		SetEmitPosition(emitter->GetX(), emitter->GetY(), emitter->GetZ());
		mScale = 0.5f;
	}
	mEmitterCountdown = kEmitTime;
	mParticleNumber = particleNumber;
}

CSmoke::~CSmoke()
{
	// Remove all particles
	while (mParticles.size() > 0)
	{
		CParticle* pParticle = mParticles.back();
		IMesh* pMesh = pParticle->mModel->GetMesh();
		pMesh->RemoveModel(pParticle->mModel);
		if (pParticle) SafeDelete(pParticle);
		mParticles.pop_back();
	}
}

void CSmoke::SetEmitPosition(float x, float y, float z)
{
	mParticleOrigen = { x, y, z };
}

void CSmoke::EmitParticle()
{
	if (mScale == 1.0f)
	{
		SetEmitPosition(mEmitter->GetX(), mEmitter->GetY() + 20.0f, mEmitter->GetZ());
	}
	else if (mScale == 0.5f)
	{
		SetEmitPosition(mEmitter->GetX(), mEmitter->GetY(), mEmitter->GetZ());
	}
	CParticle* mNewParticle = new CParticle();
	float mPosX = gpRandomiser->GetRandomFloat(mParticleOrigen.x - 3.0f, mParticleOrigen.x + 3.0f);
	float mPosZ = gpRandomiser->GetRandomFloat(mParticleOrigen.z - 3.0f, mParticleOrigen.z + 3.0f);

	mNewParticle->mModel = mNewParticle->mspMshSmokeParticle->CreateModel(mPosX, mParticleOrigen.y, mPosZ);
	mNewParticle->SetScale(mScale);

	mNewParticle->SetVector(gpRandomiser->GetRandomFloat(-kVelocity.x, kVelocity.x), kVelocity.y, gpRandomiser->GetRandomFloat(-kVelocity.z, kVelocity.z));

	mNewParticle->SetPosition(mParticleOrigen);
	mNewParticle->SetLifeTime(kSmokeLifeTime);
	mParticles.push_back(mNewParticle);
}

bool CSmoke::UpdateSystem()
{

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
		// Store particle pointer to avoid dereferencing everywhere
		CParticle* pParticle = (*itParticle);

		float lifeTime = pParticle->GetLifeTime() - gFrameTime;
		DX::XMFLOAT3 movement = pParticle->GetMoveVector();

		// Changed to one function call instead of 3
		pParticle->mModel->Move(movement.x * gFrameTime, movement.y * gFrameTime, movement.z * gFrameTime);

		pParticle->SetLifeTime(lifeTime);

		if (lifeTime <= 0.0f)
		{
			pParticle->mspMshSmokeParticle->RemoveModel(pParticle->mModel);
			SafeDelete(pParticle);
			itParticle = mParticles.erase(itParticle);
		}
		else
		{
			itParticle++;
		}
	}
	mEmitterCountdown -= gFrameTime;
	return true;
}
