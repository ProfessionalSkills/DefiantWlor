#include "Spark.h"


CSpark::CSpark(DX::XMFLOAT3 emitterPos)
{
	mParticleOrigen = emitterPos;
	mSparkCountdown = kSparkTimer;
	EmitParticle();
}

CSpark::~CSpark()
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

void CSpark::EmitParticle()
{
	CParticle* mNewParticle = new CParticle();

	mNewParticle->mModel = mNewParticle->mspMshExplosionParticle->CreateModel(mParticleOrigen.x, mParticleOrigen.y, mParticleOrigen.z);
	mNewParticle->mModel->SetSkin("RedStar.tga");
	mNewParticle->mModel->Scale(0.1f);
	mNewParticle->SetLifeTime(gpRandomiser->GetRandomFloat(0.1f, 0.3f));
	mNewParticle->SetVector(gpRandomiser->GetRandomFloat(-kSparkVelocity, kSparkVelocity),
		gpRandomiser->GetRandomFloat(-kSparkVelocity, kSparkVelocity), gpRandomiser->GetRandomFloat(-kSparkVelocity, kSparkVelocity));

	mNewParticle->SetPosition(mParticleOrigen);
	mParticles.push_back(mNewParticle);
}

bool CSpark::UpdateSystem()
{
	vector<CParticle*>::iterator itParticle = mParticles.begin();

	while (itParticle != mParticles.end())
	{
		// Get pointer to the particle instead of dereferencing for each access of the particle (more efficient - derefence is an extra step)
		CParticle* pParticle = (*itParticle);

		float lifeTime = pParticle->GetLifeTime();
		lifeTime -= gFrameTime;
		pParticle->SetLifeTime(lifeTime);

		DX::XMFLOAT3 movement = pParticle->GetMoveVector();
		pParticle->mModel->Move(movement.x * gFrameTime, movement.y * gFrameTime, movement.z * gFrameTime);
		pParticle->mModel->LookAt(gpCurWorldCamera);

		// Rescale after lookat
		pParticle->mModel->Scale(0.1f);

		if (lifeTime <= 0.0f)
		{
			pParticle->mspMshExplosionParticle->RemoveModel(pParticle->mModel);
			SafeDelete(pParticle);
			itParticle = mParticles.erase(itParticle);
		}
		else
		{
			itParticle++;
		}
	}

	if (mSparkCountdown <= 0.0f)
	{
		mSparkCountdown = kSparkTimer;
		EmitParticle();
	}

	mSparkCountdown -= gFrameTime;
	return true;
}
