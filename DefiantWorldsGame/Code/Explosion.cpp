#include "Explosion.h"

IMesh* CExplosion::mspMshExplosionBall = nullptr;

CExplosion::CExplosion(DX::XMFLOAT3 emitterPos, float particleNumber, bool ball)
{
	mParticleNumber = particleNumber;
	mParticleOrigen = emitterPos;

	for (int i = 0; i < mParticleNumber; i++)
	{
		EmitParticle();
	}

	// If there is an exploding ball, create one
	if (ball)
	{
		mHasExplodingBall = true;
		mpMdlExplosionBall = mspMshExplosionBall->CreateModel(mParticleOrigen.x, 0.0f, mParticleOrigen.z);

		// Change the scale
		mpMdlExplosionBall->Scale(1.0f);
		mExplosionBallSize = 1.0f;
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

	// Remove the exploding ball if there is one
	if (mpMdlExplosionBall)
	{
		mspMshExplosionBall->RemoveModel(mpMdlExplosionBall);
	}
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

	if (mHasExplodingBall)
	{
		// Increase size of the explosion ball
		mExplosionBallSize += gFrameTime * 3000.0f;
		mpMdlExplosionBall->ResetScale();
		mpMdlExplosionBall->Scale(mExplosionBallSize);

		// Check max size
		if (mExplosionBallSize > 500.0f)
		{
			// Remove the exploding ball if there is one
			if (mpMdlExplosionBall)
			{
				mspMshExplosionBall->RemoveModel(mpMdlExplosionBall);
				mpMdlExplosionBall = nullptr;
			}
			mHasExplodingBall = false;
		}
	}

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
	return true;
}
