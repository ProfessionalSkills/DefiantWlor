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
// GAME AGENT CLASS - CHILD OF GAME OBJECT
//-----------------------------------------------------
class CGameAgent : public CGameObject
{
private:
	// DATA
	//---------------------------
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

	// MUTATORS
	//---------------------------
	bool TakeDamage(float amount);
	bool Heal(float amount);


	// METHODS
	//---------------------------
	bool Construct();


	// VIRTUAL METHODS
	//---------------------------
	virtual bool Attack(CGameAgent* target) = 0;
	//virtual void MoveTo(CTile* dest) = 0;
	virtual bool Move() = 0;
	virtual void UnloadIModel() = 0;

private:
	virtual bool Destroy() = 0;
};

#endif /* _GAME_AGENT_H_ */
