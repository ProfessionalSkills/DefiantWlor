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

}

void CExplosion::SetEmitPosition()
{
	mParticleOrigen = { mEmitter->GetX(), mEmitter->GetY(), mEmitter->GetZ() };
}


void CExplosion::EmitParticle()
{
	CParticle* mNewParticle = new CParticle();

	mNewParticle->mModel = mNewParticle->mspMshParticle->CreateModel(mParticleOrigen.x, mParticleOrigen.y, mParticleOrigen.z);
	mNewParticle->mModel->Scale(0.1f);
	mNewParticle->SetLifeTime(3.0f);
	//mNewParticle->mModel->SetSkin("Smoke1_tlxmul.jpg");
	mNewParticle->SetVector(gpRandomiser->GetRandomFloat(-kExplosionVelocity, kExplosionVelocity),
		gpRandomiser->GetRandomFloat(-kExplosionVelocity, kExplosionVelocity), gpRandomiser->GetRandomFloat(-kExplosionVelocity, kExplosionVelocity));

	mNewParticle->SetPosition(mParticleOrigen);
	mNewParticle->SetLifeTime(kExplosionLifeTime);
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
		if ((*itParticle)->GetPosition().y <= 5.0f)
		{
			(*itParticle)->SetVector((*itParticle)->GetMoveVector().x, ((*itParticle)->GetMoveVector().y * -0.9f), (*itParticle)->GetMoveVector().z);
		}
		(*itParticle)->SetVector((*itParticle)->GetMoveVector().x, ((*itParticle)->GetMoveVector().y - (10.0f * gFrameTime)), (*itParticle)->GetMoveVector().z);
		float lifeTime = (*itParticle)->GetLifeTime();
		lifeTime -= gFrameTime;
		(*itParticle)->SetLifeTime(lifeTime);
		(*itParticle)->mModel->MoveX((*itParticle)->GetMoveVector().x * gFrameTime);
		(*itParticle)->mModel->MoveY((*itParticle)->GetMoveVector().y * gFrameTime);
		(*itParticle)->mModel->MoveZ((*itParticle)->GetMoveVector().z * gFrameTime);
		(*itParticle)->SetLifeTime((*itParticle)->GetLifeTime() - gFrameTime);

		if ((*itParticle)->GetLifeTime() <= 0.0f)
		{
			//(*itParticle)->~CParticle();
			SafeDelete((*itParticle));
			itParticle = mParticles.erase(itParticle);
		}
		else
		{
			itParticle++;
		}
	}
	return true;
}
