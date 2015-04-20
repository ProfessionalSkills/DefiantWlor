#include "Explosion.h"
CExplosion::CExplosion(IModel* emitter, float particleNumber)
{
	mEmitter = emitter;
	mParticleNumber = particleNumber;
	SetEmitPosition();

	for (int i = 0; i < mParticleNumber; i++)
	{
		EmitParticle();
	}
}

CExplosion::~CExplosion()
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

void CExplosion::SetEmitPosition()
{
	mParticleOrigen = { mEmitter->GetX(), mEmitter->GetY(), mEmitter->GetZ() };
}


void CExplosion::EmitParticle()
{
	CParticle* mNewParticle = new CParticle();

	mNewParticle->mModel = mNewParticle->mspMshExplosionParticle->CreateModel(mParticleOrigen.x, mParticleOrigen.y, mParticleOrigen.z);
	mNewParticle->mModel->Scale(0.1f);
	mNewParticle->SetLifeTime(gpRandomiser->GetRandomFloat(1.5f, 3.0f));
	mNewParticle->SetVector(gpRandomiser->GetRandomFloat(-kExplosionVelocity, kExplosionVelocity),
	gpRandomiser->GetRandomFloat(-kExplosionVelocity, kExplosionVelocity), gpRandomiser->GetRandomFloat(-kExplosionVelocity, kExplosionVelocity));

	mNewParticle->SetPosition(mParticleOrigen);
	mParticles.push_back(mNewParticle);
}

bool CExplosion::UpdateSystem()
{
	if (mParticles.size() <= 0)
	{
		return false;
	}

	vector<CParticle*>::iterator itParticle = mParticles.begin();
	while (itParticle != mParticles.end())
	{
		// Get pointer to the particle instead of dereferencing for each access of the particle (more efficient - derefence is an extra step)
		CParticle* pParticle = (*itParticle);

		// Making the particles bounce is pretty cool :P
		if (pParticle->GetPosition().y <= 5.0f)
		{
			pParticle->SetVector(pParticle->GetMoveVector().x, (pParticle->GetMoveVector().y * -0.9f), pParticle->GetMoveVector().z);
		}
		else
		{
			pParticle->SetVector(pParticle->GetMoveVector().x, (pParticle->GetMoveVector().y - (10.0f * gFrameTime)), pParticle->GetMoveVector().z);
		}

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
	return true;
}
