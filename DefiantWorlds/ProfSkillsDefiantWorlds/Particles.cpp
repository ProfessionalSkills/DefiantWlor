//-----------------------------------------------------
// FILE: Particles.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Particles.h"


//-----------------------------------------------------
// PARTICLE EMITTER CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------

CParticle::CParticle(DX::XMFLOAT3 position) 
{
	/*CRandomiser rand;
	mModel = mMesh->CreateModel(position.x, position.y, position.z);
	mMoveVector.x = rand.GetRandomInt();*/
	//CONSTRUCTOR WILL BE DEPENDANT ON THE SYSTEM THAT THE PARTICLE IS IN
}

CParticle::~CParticle()
{
	mMesh->RemoveModel(mModel);
}