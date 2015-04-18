//-----------------------------------------------------
// FILE: Particles.h
//-----------------------------------------------------

#ifndef _PARTICLES_H
#define _PARTICLES_H


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include <TL-Engine.h>			// TL-Engine stuff
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include "Randomiser.h"

using namespace tle;

// Renamed the DirectX namespace so that DirectX:: is not required all the time. Simply use DX:: instead.
// e.g. Vector3 variable created by doing DX::XMFLOAT3 mPos; just as an example.
namespace DX = DirectX;


extern I3DEngine* gpEngine;

//-----------------------------------------------------
// PARTICLE CLASS
//-----------------------------------------------------
class CParticle
{
protected:
	
	DirectX::XMFLOAT3 mMoveVector; //3D Particle Movement
	DirectX::XMFLOAT3 mPosition; //Particle Position
	string mSkin; //Particle texture
	float mLifeTime; //Life time of Particle
public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CParticle();
	~CParticle();

	IModel* mModel; //Particle Model

	// PARTICLE STATIC MESH
	//---------------------------
	static IMesh* mspMshExplosionParticle;
	static IMesh* mspMshSmokeParticle;
	inline void SetPosition(DX::XMFLOAT3 position)
	{
		mPosition = position;
	}

	inline DX::XMFLOAT3 GetPosition()
	{
		return mPosition;
	}

	inline void SetVector(float x, float y, float z)
	{
		mMoveVector = { x, y, z };
	}

	inline void SetSkin(string skin)
	{
		mModel->SetSkin(skin);
	}

	inline void SetScale(float scale)
	{
		mModel->Scale(scale);
	}

	inline void SetLifeTime(float lifetime)
	{
		mLifeTime = lifetime;
	}

	inline float GetLifeTime()
	{
		return mLifeTime;
	}

	inline DX::XMFLOAT3 GetMoveVector()
	{
		return mMoveVector;
	}
	// PARTICLE DATA (ACCESSED BY PARTICLE SYSTEMS)
	//---------------------------
};

#endif
