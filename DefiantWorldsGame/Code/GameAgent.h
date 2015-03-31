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
class CGameAgent : public CGameObject
{
protected:
	// DATA
	//---------------------------
	SAgentData mAgentInfo;
	DX::XMFLOAT3 mPathTarget;
	std::stringstream mStrDisplay;
	SBoundingSphere mBoundingSphere;

	CGameObject* mAttackTarget;
	float mRange;
	float mFireRate;
	float mAttackTimer;
	vector<SProjectile*>mpProjectiles;
	vector<CExplosion*>mpAttackExplosions;
	float mSpeed;
	float mProductionTime;
	float mProductionCost;
	float mCurProductionTimeLeft;				// Countdown for time remaining until construction complete
	float mDamage;
	//CParticleEmitter* mAttackParticleFX;
	//CParticleEmitter* mDestroyParticleFX;
	EObjectStates mState;
	//CTile* mDestGridSq;
	bool mIsMoving;
	bool mHasPathTarget;
	float mUnitSpacing;


	eFleetPos mFleetPosition;
public:
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

	inline float GetProductionCost()
	{
		return mProductionCost;
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

	inline void SetPathTarget(DX::XMFLOAT3 pathTile)
	{
		mPathTarget = pathTile;
		//mAttackTarget = nullptr;
		mHasPathTarget = true;
	}

	inline void SetAttackTarget(CGameObject* target)
	{
		mAttackTarget = target;
		mHasPathTarget = false;
	}

	inline CGameObject* GetAttackTarget()
	{
		return mAttackTarget;
	}

	inline DX::XMFLOAT3 GetPathTarget()
	{
		return mPathTarget;
	}

	inline bool HasTarget()
	{
		return mHasPathTarget;
	}

	eFleetPos GetPosType();

	virtual float GetUnitSpacing();


	// MUTATORS
	//---------------------------
	bool Heal(float amount);
	void SetState(EObjectStates newState);
	
	inline void SetProdTimeLeft(float timeLeft)
	{
		mCurProductionTimeLeft = timeLeft;
	}

	// METHODS
	//---------------------------
	bool Construct();
	void Destroy();

	// VIRTUAL METHODS
	//---------------------------
	virtual bool Attack(CGameObject* target, float hitMod, float damageMod) = 0;
	virtual void Spawn(CGrid* pGrid, SPointData pCentre) = 0; //Spawns the Unit into the game 
	bool RayCollision(DX::XMFLOAT3 origin, DX::XMFLOAT3 direction, float& distance);
	bool DragBoxCollision(DX::BoundingBox& box);
	void DisplayInfo(IFont* font);
	//virtual void MoveTo(CTile* dest) = 0;
	virtual bool Update() = 0;
	virtual void UnloadIModel() = 0;
	virtual void LoadIModel() = 0;
	virtual void LoadModel(float x, float y, float z) = 0;
	virtual void Move()=0;

	// Save the data for this unit
	virtual void SaveAgent(std::ofstream& outFile);


private:

};

#endif /* _GAME_AGENT_H_ */
