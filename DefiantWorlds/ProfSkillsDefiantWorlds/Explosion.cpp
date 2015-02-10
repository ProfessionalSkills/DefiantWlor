#include "Explosion.h"

CExplosion::CExplosion(IModel* emitter, IMesh* particleMesh)
{
	rand = new CRandomiser();
	mEmitter = emitter;
	SetEmitPosition();

	for (int i = 0; i < mParticleNumber; i++)
	{
		EmitParticle(particleMesh);
	}
}

CExplosion::~CExplosion()
{

}

void CExplosion::SetEmitPosition()
{
	mEmitPosition = { mEmitter->GetX(), mEmitter->GetY(), mEmitter->GetZ() };
}

DirectX::XMFLOAT3 CExplosion::GetEmitPosition()
{
	return mEmitPosition;
}

void CExplosion::EmitParticle(IMesh* particleMesh)
{
	CParticle* mNewParticle = new CParticle();
	mNewParticle->mMesh = particleMesh;
	mNewParticle->mModel = mNewParticle->mMesh->CreateModel(mEmitPosition.x, mEmitPosition.y, mEmitPosition.z);
	mNewParticle->mModel->Scale(0.1f);
	mNewParticle->mModel->SetSkin("OrangeStar.tga");
	mNewParticle->mMoveVector.x = rand->GetRandomFloat(-8.0f, 8.0f);
	mNewParticle->mMoveVector.y = rand->GetRandomFloat(-8.0f, 8.0f);
	mNewParticle->mMoveVector.z = rand->GetRandomFloat(-8.0f, 8.0f);

	mNewParticle->mPosition = mEmitPosition;
	mNewParticle->mLifeTime = 3.5f;
	Particles.push_back(mNewParticle);
}

void CExplosion::UpdateSystem(float mFrameTime, IModel* emitter, IMesh* particleMesh, ICamera* myCamera)
{
	vector<CParticle*>::iterator itParticle = Particles.begin();

	while (itParticle != Particles.end())
	{
		(*itParticle)->mModel->MoveX((*itParticle)->mMoveVector.x * mFrameTime);
		(*itParticle)->mModel->MoveY((*itParticle)->mMoveVector.y * mFrameTime);
		(*itParticle)->mModel->MoveZ((*itParticle)->mMoveVector.z * mFrameTime);

		(*itParticle)->mLifeTime -= mFrameTime;

		if ((*itParticle)->mLifeTime <= 0.0f)
		{
			(*itParticle)->~CParticle();
			itParticle = Particles.erase(itParticle);
		}
		else
		{
			itParticle++;
		}
	}
}
