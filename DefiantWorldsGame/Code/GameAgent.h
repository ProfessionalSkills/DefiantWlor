//-----------------------------------------------------
// FILE: GameAgent.h
//-----------------------------------------------------

#ifndef _GAME_AGENT_H_
#define _GAME_AGENT_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "GameObject.h"
#include "Grid.h"

enum eFleetPos { front, centre, back };

//-----------------------------------------------------
// STRUCTURE
//-----------------------------------------------------
struct SAgentData
{
	EGameAgentVariations mAgentType;
	std::string	         mAgentName;
	
	SAgentData()
	{
		mAgentType = GAV_WORKER;
		mAgentName = "NULL";
	}

	SAgentData(EGameAgentVariations agentType, std::string agentName)
	{
		mAgentType = agentType;
		mAgentName = agentName;
	}
};


//-----------------------------------------------------
// GAME AGENT CLASS - CHILD OF GAME OBJECT
//-----------------------------------------------------
class CRTSPlayer;
class CStructure;

class CGameAgent : public CGameObject
{
protected:

	// DATA
	//---------------------------
	SAgentData mAgentInfo;
	DX::XMFLOAT3 mPathTarget;
	bool mHasModelMatrix = false;
	DX::XMFLOAT4X4 mModelMatrix;
	std::stringstream mStrDisplay;
	SBoundingSphere mBoundingSphere;

	CGameObject* mAttackTarget = nullptr;
	float mRange = 100.0f;
	float mFireRate;
	float mAttackTimer;
	float AutoTargettingTimer;
	float mAutoTargetTime;
	vector<SProjectile*>mpProjectiles;
	vector<CExplosion*>mpAttackExplosions;
	float mSpeed;
	float mProductionTime;
	float mCurProductionTimeLeft;				// Countdown for time remaining until construction complete
	float mDamage;
	float mTargettingTimer;
	float mAutoTargetting;
	bool mIsMoving;
	bool mHasPathTarget;
	float mUnitSpacing;

	bool mUnitSelected = false;

	CRTSPlayer* mpOwner = nullptr;

	eFleetPos mFleetPosition;
	EAirspaces mAirspace;						// Stores which airspace this unit is currently flying in

	CSound* mAttackSound = nullptr;

public:
	// Waypoint Arrow Mesh
	static IMesh* mspMshWaypointArrow;
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CGameAgent();
	virtual ~CGameAgent();


	// ACCESSORS
	//---------------------------
	inline float GetHealth()
	{
		return mHealth;
	}

	inline float GetSpeed()
	{
		return mSpeed;
	}

	inline float GetProductionTime()
	{
		return mProductionTime;
	}

	inline float GetCurProductionTimeLeft()
	{
		return mCurProductionTimeLeft;
	}

	inline float GetDamage()
	{
		return mDamage;
	}

	inline EObjectStates GetAgentState()
	{
		return mState;
	}

	inline void SetAgentState(EObjectStates newState)
	{
		mState = newState;
	}

	inline SAgentData* GetAgentData()
	{
		return &mAgentInfo;
	}

	inline CRTSPlayer* Getowner()
	{
		return mpOwner;
	}

	inline void SetPathTarget(DX::XMFLOAT3 pathTile)
	{
		mPathTarget = pathTile;
		mHasPathTarget = true;

		// Place a waypoint at the target if it is selected
		if (mUnitSelected)
		{
			SetWaypointArrow();
			// *TO DO *
		}
	}

	inline CGameObject* GetAttackTarget()
	{
		return mAttackTarget;
	}

	inline DX::XMFLOAT3 GetPathTarget()
	{
		return mPathTarget;
	}

	inline EGameAgentVariations GetUnitType()
	{
		return mAgentInfo.mAgentType;
	}

	inline bool HasTarget()
	{
		return mHasPathTarget;
	}

	inline float GetAutoTimer()
	{
		return mAutoTargetting;
	}
	inline float GetAttackTimer()
	{
		return mAutoTargetTime;
	}

	inline void SetAttackTimer(float timer)
	{
		mAutoTargetTime = timer;
	}
	inline void SetAutoTimer(float timer)
	{
		mAutoTargetting = timer;
	}

	inline void SetOwner(CRTSPlayer* pPlayer)
	{
		mpOwner = pPlayer;
	}

	inline void RemoveBoundingSphere()
	{
		mBoundingSphere = SBoundingSphere();
	}

	inline EAirspaces GetAirspacePosition()
	{
		return mAirspace;
	}

	eFleetPos GetPosType();

	virtual float GetUnitSpacing();


	// MUTATORS
	//---------------------------
	bool Heal(float amount);
	void SetState(EObjectStates newState);
	
	inline void SetAttackSound(CSound* newSound)
	{
		mAttackSound = newSound;
	}


	inline void SetProdTimeLeft(float timeLeft)
	{
		mCurProductionTimeLeft = timeLeft;
	}

	inline void SetHealth(float health)
	{
		mHealth = health;
	}

	inline void CancelPathTarget()
	{
		mHasPathTarget = false;
	}

	inline void UpdateBoundingSphere()
	{
		mBoundingSphere.MoveTo(mWorldPos);
	}

	inline void RotateModelY(float DegRot)
	{
		if (mpObjModel != 0)
		{
			mpObjModel->RotateY(DegRot);
		}
	}

	inline void SetWaypointArrow()
	{
		if (!mpObjWaypoint)
		{
			mpObjWaypoint = mspMshWaypointArrow->CreateModel(mPathTarget.x, 20.0f, mPathTarget.z);
			mpObjWaypoint->RotateX(90.0f);
		}
		else
		{
			mpObjWaypoint->SetPosition(mPathTarget.x, 10.0f, mPathTarget.z);
		}
		
	}

	inline void UpdateWaypointArrow()
	{
		if (mpObjWaypoint != nullptr)
		{
			mpObjWaypoint->RotateY(50.0f * gFrameTime);
		}
	}

	inline void DeleteWaypointArrow()
	{
		if (mpObjWaypoint)
		{
			mspMshWaypointArrow->RemoveModel(mpObjWaypoint);
			mpObjWaypoint = nullptr; 
			//SafeDelete(mpObjWaypoint);
		}
	}

	// METHODS
	//---------------------------
	bool Construct();
	void CalculateBoundingSphere();
	void Destroy();
	void SetAttackTarget(CGameObject* target);
	void Stop();
	void SetAttackSound(string soundFile);

	// VIRTUAL METHODS
	//---------------------------
	virtual bool Attack(CGameObject* target, float hitMod, float damageMod) = 0;
	virtual void Spawn(CGrid* pGrid, SPointData pCentre) = 0; //Spawns the Unit into the game 
	bool RayCollision(DX::XMFLOAT3 origin, DX::XMFLOAT3 direction, float& distance);
	bool SphereCollision(SBoundingSphere& sphere);
	bool DragBoxCollision(DX::BoundingBox& box);
	bool CheckLOS(CStructure* pStructure);			// Check if the object is in reaching distance of a wall
	void DisplayInfo(IFont* font);
	virtual bool Update() = 0;
	virtual void UnloadIModel() = 0;
	virtual void LoadIModel() = 0;
	virtual void LoadModel(float x, float y, float z) = 0;
	virtual void Move()=0;

	// Set the selected or deselected texture
	void SetSelectedTexture();
	void SetDeselectedTexture();

	// Save the data for this unit
	virtual void SaveAgent(std::ofstream& outFile);
	virtual void LoadAgent(std::ifstream& inFile);

	virtual void SetTargetTexture()
	{

	}

	virtual void SetNormalTexture()
	{

	}


private:

};

#endif /* _GAME_AGENT_H_ */
