//-----------------------------------------------------
// FILE: GameObject.h
//-----------------------------------------------------

#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Common.h"


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
	int   mBuildCost;
	EFactions mFaction;

	// TLE OBJECT DATA
	//---------------------------
	IModel* mpObjModel;

	// SOUND
	//---------------------------
	CSound* mGenSound;

public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CGameObject();
	virtual ~CGameObject();


	// ACCESSORS
	//---------------------------
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
	void InitBaseVariables();


	// VIRTUAL METHODS
	//---------------------------
};


#endif /* _GAME_OBJECT_H_ */
