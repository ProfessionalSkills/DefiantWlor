#include "Smoke.h"

CSmoke::CSmoke(DX::XMFLOAT3 emitterPos, float relativeHeight, float scale, float width, float length)
{
	mRelativeHeight = relativeHeight;
	SetEmitPosition(emitterPos);
	mScale = scale;
	mWidth = width;
	mLength = length;

	mEmitterCountdown = gpRandomiser->GetRandomFloat(0.05, 0.2);
}

CSmoke::~CSmoke()
{
	// Remove all particles
	while (mParticles.size() > 0)
	{
		CParticle* pParticle = mParticles.back();
		IMesh* pMesh = pParticle->mModel->GetMesh();
		pMesh->RemoveModel(pParticle->mModel);
		SafeDelete(pParticle);
		mParticles.pop_back();
	}
}

void CSmoke::SetEmitPosition(const DX::XMFLOAT3& emitterPos)
{
	mParticleOrigen = { emitterPos.x, emitterPos.y + mRelativeHeight + 1.0f, emitterPos.z };
}

void CSmoke::EmitParticle()
{
	CParticle* mNewParticle = new CParticle();
	float mPosX = gpRandomiser->GetRandomFloat(mParticleOrigen.x - mWidth, mParticleOrigen.x + mWidth);
	float mPosZ = gpRandomiser->GetRandomFloat(mParticleOrigen.z - mLength, mParticleOrigen.z + mLength);

	mNewParticle->mModel = mNewParticle->mspMshSmokeParticle->CreateModel(mPosX, mParticleOrigen.y, mPosZ);
	mNewParticle->SetScale(mScale);

	mNewParticle->SetVector(gpRandomiser->GetRandomFloat(-1.0f, 1.0f), gpRandomiser->GetRandomFloat(3.0f, 5.0f), gpRandomiser->GetRandomFloat(-1.0f, 1.0f));

	mNewParticle->SetLifeTime(kSmokeLifeTime);
	mParticles.push_back(mNewParticle);
}

bool CSmoke::UpdateSystem()
{
	if (mEmitterCountdown <= 0.0f)
	{
		EmitParticle();
		mEmitterCountdown = gpRandomiser->GetRandomFloat(0.05, 0.2);;
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
		pParticle->mModel->LookAt(gpCurWorldCamera);

		// Rescale after lookat
		pParticle->mModel->Scale(mScale);

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

bool CSmoke::UpdateSystem(const DX::XMFLOAT3& emitterPos)
{
	// Update emitter position
	SetEmitPosition(emitterPos);
	
	// Call the base function
	return UpdateSystem();
}
