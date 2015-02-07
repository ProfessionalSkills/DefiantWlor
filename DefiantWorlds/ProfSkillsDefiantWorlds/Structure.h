//-----------------------------------------------------
// FILE: Structure.h
//-----------------------------------------------------

#ifndef _STRUCTURE_H_
#define _STRUCTURE_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "GameObject.h"
#include "Grid.h"


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
	int   mBuildCost;
	bool  mIsBuilt;
	//CParticleEmitter* mDestroyParticleFX;
	EObjectStates mState;

	SPointData mStructureSize;			// Size in X and Y axis - used for determing clearance
										// of area around the building when being placed


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

	inline int GetBuildCost()
	{
		return mBuildCost;
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
	void CreateStructure(DX::XMFLOAT3 pos);					// Creating with 3D coords
	bool TestStructureArea(CGrid* pGrid, CTile* pTile);
	bool Build(float delta);


	// VIRTUAL METHODS
	//---------------------------
private:
	virtual bool Destroy() = 0;
};

#endif /* _STRUCTURE_H_ */
