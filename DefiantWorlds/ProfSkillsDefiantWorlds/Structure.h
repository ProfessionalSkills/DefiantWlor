//-----------------------------------------------------
// FILE: Structure.h
//-----------------------------------------------------

#ifndef _STRUCTURE_H_
#define _STRUCTURE_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "GameObject.h"


//-----------------------------------------------------
// STRUCTURE CLASS - CHILD OF GAME OBJECT
//-----------------------------------------------------
class CStructure : public CGameObject
{
private:
	// DATA
	//---------------------------
	float mHealth;
	float mBuildTime;
	float mRepairSpeed;
	float mCurBuildTimeLeft;			// Time left until structure is built
	bool  mIsBuilt;
	//CParticleEmitter* mDestroyParticleFX;
	EObjectStates mState;


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CStructure();
	virtual ~CStructure();


	// ACCESSORS
	//---------------------------
	inline float GetHealth()
	{
		return mHealth;
	}

	inline float GetBuildTime()
	{
		return mBuildTime;
	}

	inline float GetRepairSpeed()
	{
		return mRepairSpeed;
	}

	inline float GetBuildTimeLeft()
	{
		return mCurBuildTimeLeft;
	}

	inline EObjectStates GetState()
	{
		return mState;
	}


	// MUTATORS
	//---------------------------
	bool TakeDamage(float amount);
	bool Repair(float amount);


	// METHODS
	//---------------------------
	void CreateStructure(DX::XMFLOAT3 pos);		// Creating with 3D coords
	void CreateStructure(SPointData pos);		// Create with grid coordinates (grid coordinate of 'central' grid sqaure
	bool Build(float delta);


	// VIRTUAL METHODS
	//---------------------------
private:
	virtual bool Destroy() = 0;
};

#endif /* _STRUCTURE_H_ */
