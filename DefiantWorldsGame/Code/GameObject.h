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
	int mPopCost;
	float mMaxHealth;
	float mHealth;

	EFactions mFaction;

	// TLE OBJECT DATA
	//---------------------------
	IModel* mpObjModel = nullptr;

	CExplosion* mDestructionExplosion;
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

	void TakeDamage(int amount)
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

	virtual bool RayCollision(DX::XMFLOAT3 origin, DX::XMFLOAT3 direction, float& distance)
	{
		return false;
	}

	virtual bool SphereCollision(SBoundingSphere& sphere)
	{
		return false;
	}


	// METHODS
	//---------------------------
};


#endif /* _GAME_OBJECT_H_ */
