//-----------------------------------------------------
// FILE: Minerals.h
//-----------------------------------------------------

#ifndef _MINERALS_H_
#define _MINERALS_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "GameObject.h"
#include "Grid.h"


//-----------------------------------------------------
// MINERALS CLASS - CHILD OF GAME OBJECT
//-----------------------------------------------------
class CMinerals : public CGameObject
{
private:
	// DATA
	//---------------------------
	int mAmount;
	float mHeight = 9.0f;					// Used for bounding box creation
	SBoundingCube mBoundingBox;				// Axis aligned bounding box for this structure
	bool mIsUsed = false;

	SPointData mStructureBL;				// Amount of grid squares relative to the centre to find the bottom left grid square
	SPointData mStructureTR;				// Same as above, but for top right grid square


public:
	static IMesh* mspMshMineral;


	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CMinerals();
	virtual ~CMinerals();


	// ACCESSORS
	//---------------------------
	inline int GetResourceAmount()
	{
		return mAmount;
	}

	inline bool IsBeingUsed()
	{
		return mIsUsed;
	}


	// MUTATORS
	//---------------------------
	inline void SetUsage(bool usage)
	{
		mIsUsed = usage;
	}



	// METHODS
	//---------------------------
	void CreateResource(CGrid* pGrid, SPointData gridPos);
	bool RayCollision(DX::XMFLOAT3 origin, DX::XMFLOAT3 direction, float& distance);
	void CalculateBoundingBox();
	void LoadIModel();
	void UnloadIModel();

	void SetTargetTexture()
	{
		// Do nothing for minerals
	}

	virtual void SetNormalTexture()
	{
		// Do nothing for minerals
	}

};

#endif /* _MINERALS_H_ */
