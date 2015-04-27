//-----------------------------------------------------
// FILE: GameObject.h
//-----------------------------------------------------

#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Common.h"
#include "Explosion.h"
#include "Smoke.h"
#include "Spark.h"


//-----------------------------------------------------
// GAME OBJECT CLASS
//-----------------------------------------------------
class CGameObject
{
protected:
	// DATA
	//---------------------------
	DX::XMFLOAT3 mWorldPos;
	SPointData mGridPos;

	float mOrientation;
	float mScale;
	int mBuildCost;
	int mPopCost = 0;
	float mMaxHealth = -1.0f;
	float mHealth = -1.0f;

	// Store whether or not this entity is of ground or air type
	bool mIsGroundType = true;

	EObjectStates mState;
	EFactions mFaction;


	// TLE OBJECT DATA
	//---------------------------
	IModel* mpObjModel = nullptr;
	IModel* mpObjShadow = nullptr;

	CExplosion* mDestructionExplosion = nullptr;
	CSmoke* mWarningSmoke = nullptr;


	// SOUND
	//---------------------------
	CSound* mGenSound;
	CSound* mSelectSound;


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CGameObject();
	virtual ~CGameObject();


	// ACCESSORS
	//---------------------------
	inline int GetPopValue()
	{
		return mPopCost;
	}

	inline DX::XMFLOAT3 GetWorldPos()
	{
		return mWorldPos;
	}

	inline float GetWorldXPos()
	{
		return mWorldPos.x;
	}

	inline float GetWorldZPos()
	{
		return mWorldPos.z;
	}

	inline SPointData GetGridPos()
	{
		return mGridPos;
	}

	inline int GetGridX()
	{
		return mGridPos.mPosX;
	}

	inline int GetGridY()
	{
		return mGridPos.mPosY;
	}

	inline float GetOrientation()
	{
		return mOrientation;
	}

	inline EFactions GetFaction()
	{
		return mFaction;
	}

	inline int GetBuildCost()
	{
		return mBuildCost;
	}
	
	inline void TakeDamage(int amount)
	{
		mHealth -= amount;
	}

	inline float GetHealth()
	{
		return mHealth;
	}

	inline float GetMaxHealth()
	{
		return mMaxHealth;
	}

	inline IModel* GetModel()
	{
		return mpObjModel;
	}

	inline bool IsGroundType()
	{
		return mIsGroundType;
	}

	inline EObjectStates GetState()
	{
		return mState;
	}


	// MUTATORS
	//---------------------------
	inline void SetFaction(EFactions faction)
	{
		mFaction = faction;
	}

	void SetGridPos(SPointData pos);
	void SetWorldPos(DX::XMFLOAT3 pos);
	void SetOrientation(float orientation);
	void ChangeRelativeOrientation(float amount);


	// METHODS
	//---------------------------
	virtual bool RayCollision(DX::XMFLOAT3 origin, DX::XMFLOAT3 direction, float& distance)
	{
		return false;
	}

	virtual bool SphereCollision(SBoundingSphere& sphere)
	{
		return false;
	}

	virtual void SetTargetTexture() = 0;
	virtual void SetNormalTexture() = 0;
};


#endif /* _GAME_OBJECT_H_ */
