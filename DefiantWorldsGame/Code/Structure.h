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
protected:
	// DATA
	//---------------------------
	float mHealth;
	float mBuildTime;
	float mRepairSpeed;
	float mCurBuildTimeLeft;			// Time left until structure is built
	int   mBuildCost;
	//CParticleEmitter* mDestroyParticleFX;

	EObjectStates mState;
	EGameStructureTypes mStructureType;

	CGrid* mpGrid;

	SPointData mBuildLoc;			// Grid position of the placement of building
	SPointData mStructureBL;		// Amount of grid squares relative to the centre to find the bottom left grid square
	SPointData mStructureTR;		// Same as above, but for top right grid square
	SAABoundingBox mBoundingBox;	// Axis aligned bounding box for this structure

	std::stringstream mStrDisplay;	// used to output data about the building to the screen


	// UNIT SPAWNING VARIABLES
	//---------------------------
	SPointData mGridSpawnLoc;			// The single grid square in which units spawn (relative to building centre)
	DX::XMFLOAT3 mWorldSpawnLoc;		// The calculated world position of the above grid square


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

	inline SPointData GetBLPosition()
	{
		return mStructureBL;
	}

	inline SPointData GetTRPosition()
	{
		return mStructureTR;
	}

	inline SPointData GetBuildLocation()
	{
		return mBuildLoc;
	}

	inline SPointData GetGridSpawnLocation()
	{
		return mGridSpawnLoc;
	}

	inline DX::XMFLOAT3 GetWorldSpawnLocation()
	{
		return mWorldSpawnLoc;
	}


	// MUTATORS
	//---------------------------
	inline void SetBuildLocation(SPointData &gridPos)
	{
		mBuildLoc = gridPos;
	}

	void SetState(EObjectStates newState);

	bool TakeDamage(float amount);
	bool Repair(float amount);
	void SetBadTexture();
	void SetGoodTexture();
	void SetPlacedTexture();


	// METHODS
	//---------------------------
	void CreateStructure(CGrid* pGrid);
	bool TestStructureArea(CGrid* pGrid, CTile* pTile);
	bool PointCollision(DX::XMFLOAT3 pos);
	void Destroy();


	// VIRTUAL METHODS
	//---------------------------
	// Method called each frome - boolean returns is false when the object has been destroyed
	virtual bool Update() = 0;

	virtual void DisplayInfo(IFont* font) = 0;
	virtual void SetBuiltModel() = 0;
	virtual void UnloadIModel() = 0;
};

#endif /* _STRUCTURE_H_ */
