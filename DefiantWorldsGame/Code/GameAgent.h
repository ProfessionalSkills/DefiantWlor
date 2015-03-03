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
	float mHealth;
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
	bool hasTarget;
	int  mPopCost;

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
		hasTarget = true;
	}

	inline DX::XMFLOAT3 GetPathTarget()
	{
		return mPathTarget;
	}

	inline bool HasTarget()
	{
		return hasTarget;
	}

	eFleetPos GetPosType();

	// MUTATORS
	//---------------------------
	bool TakeDamage(float amount);
	bool Heal(float amount);


	// METHODS
	//---------------------------
	bool Construct();


	// VIRTUAL METHODS
	//---------------------------
	virtual bool Attack(CGameAgent* target, float hitMod, float damageMod) = 0;
	virtual void Spawn(CGrid* pGrid, SPointData pCentre) = 0; //Spawns the Unit into the game 
	bool RayCollision(DX::XMFLOAT3 origin, DX::XMFLOAT3 direction, float& distance);
	void DisplayInfo(IFont* font);
	//virtual void MoveTo(CTile* dest) = 0;
	virtual void Update() = 0;
	virtual void UnloadIModel() = 0;
	virtual void LoadIModel() = 0;
	virtual void LoadModel(float x, float y, float z) = 0;

private:
	virtual bool Destroy() = 0;
};

#endif /* _GAME_AGENT_H_ */
