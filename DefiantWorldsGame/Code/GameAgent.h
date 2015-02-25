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
	int  mPopCost;


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

	inline SAgentData* GetAgentData()
	{
		return &mAgentInfo;
	}


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
	//virtual void MoveTo(CTile* dest) = 0;
	virtual bool Move() = 0;
	virtual void UnloadIModel() = 0;
	virtual void LoadIModel() = 0;

private:
	virtual bool Destroy() = 0;
};

#endif /* _GAME_AGENT_H_ */
