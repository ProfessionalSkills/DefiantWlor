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

#include "Artillery.h"
#include "Tank.h"
#include "Infantry.h"

#include "Worker.h"

#include "Bomber.h"
#include "Fighter.h"

#include "SpaceFighter.h"
#include "Mothership.h"
#include "Transport.h"


//-----------------------------------------------------
// STRUCTURE CLASS - CHILD OF GAME OBJECT
//-----------------------------------------------------
class CRTSPlayer;
class CWall;

class CStructure : public CGameObject
{
protected:
	// DATA
	//---------------------------
	float mBuildTime;
	float mRepairSpeed;
	float mCurBuildTimeLeft;			// Time left until structure is built
	float mHeight;

	EGameStructureTypes mStructureType;
	CGrid* mpGrid;

	SPointData mBuildLoc;			// Grid position of the placement of building
	SPointData mStructureBL;		// Amount of grid squares relative to the centre to find the bottom left grid square
	SPointData mStructureTR;		// Same as above, but for top right grid square
	SBoundingCube mBoundingBox;		// Axis aligned bounding box for this structure

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

	inline EGameStructureTypes GetStructureType()
	{
		return mStructureType;
	}

	inline void SetSound(CSound* newSound)
	{
		mGenSound = newSound;
	}

	inline CSound* GetSound()
	{
		return mGenSound;
	}

	inline void SetSelectSound(CSound* newSound)
	{
		mSelectSound = newSound;
	}

	inline CSound* GetSelectSound()
	{
		return mSelectSound;
	}


	// MUTATORS
	//---------------------------
	inline void SetBuildLocation(SPointData &gridPos)
	{
		mBuildLoc = gridPos;
	}

	inline void SetHealth(float health)
	{
		mHealth = health;
	}

	// Change the state of the building to a new state
	void SetState(EObjectStates newState);
	// Let the building take a set amount of damage
	bool TakeDamage(float amount);
	// Repair the building by a given amount
	bool Repair(float amount);
	// When placing the building: set the relevant textures
	void SetBadTexture();
	void SetGoodTexture();
	void SetPlacedTexture();
	// Set the selected or deselected texture
	void SetSelectedTexture();
	void SetDeselectedTexture();


	// METHODS
	//---------------------------
	void CreateStructure(CGrid* pGrid);
	bool TestStructureArea(CGrid* pGrid, CTile* pTile);
	bool RayCollision(DX::XMFLOAT3 origin, DX::XMFLOAT3 direction, float& distance);
	bool SphereCollision(SBoundingSphere& sphere);
	void SetGridData(CGrid* pGrid);
	void CalculateBoundingBox();
	void Destroy();

	// Change the texture of the building if the building is a target
	void SetTargetTexture();
	void SetNormalTexture();


	// VIRTUAL METHODS
	//---------------------------
	// Returns the size of the production queue. Returns -1 for non-production structures
	virtual int GetQueueSize() = 0;
	// Method called each frome - boolean returns is false when the object has been destroyed
	virtual bool Update(CRTSPlayer* pPlayer) = 0;
	// Adds a unit to the queue based on the index which has been passed in
	virtual EErrorTypes AddToQueue(size_t agentIndex, CRTSPlayer* pPlayer) = 0;
	// Removes a unit in the queue at the given index
	virtual void RemoveFromQueue(size_t agentIndex, CRTSPlayer* pPlayer) = 0;
	// Returns a pointer to the queue of units
	virtual std::deque<CGameAgent*>* GetQueue() = 0;

	virtual void DisplayInfo(IFont* font) = 0;
	virtual void SetBuiltModel() = 0;
	virtual void UnloadIModel() = 0;
	virtual void LoadIModel() = 0;

	// Save the structure's details
	virtual void SaveStructure(std::ofstream& outFile) = 0;
	virtual void LoadStructure(std::ifstream& inFile, CGrid* pGrid, CRTSPlayer* pPlayer) = 0;
};

#endif /* _STRUCTURE_H_ */
