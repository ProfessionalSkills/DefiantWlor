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
// CONSTANTS
//-----------------------------------------------------
const int MAX_UNITS = 50;
const int MAX_SPACE_UNITS = 10;


//-----------------------------------------------------
// INTERFACE CLASS FOR PLAYERS
//-----------------------------------------------------
class CGameObject
{
protected:
	// DATA
	//---------------------------
	DX::XMFLOAT3 mWorldPos;
	SPointData   mGridPos;
	float        mOrientation;
	int          mPopCost;


	// TLE OBJECT DATA
	//---------------------------
	IMesh*  mpMesh;
	IModel* mpModel;


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CGameObject();
	CGameObject(DX::XMFLOAT3 pos, float orientation);
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

	inline float GetGridX()
	{
		return mGridPos.mPosX;
	}

	inline float GetGridY()
	{
		return mGridPos.mPosY;
	}

	inline float GetOrientation()
	{
		return mOrientation;
	}


	// MUTATORS
	//---------------------------
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
