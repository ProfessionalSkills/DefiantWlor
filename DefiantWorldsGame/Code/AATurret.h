//-----------------------------------------------------
// FILE: TurretStructure.h
//-----------------------------------------------------

#ifndef _AATURRET_STRUCTURE_H_
#define _AATURRET_STRUCTURE_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Structure.h"


//-----------------------------------------------------
// STATIC STRUCTURE CLASS - CHILD OF STRUCTURE
//-----------------------------------------------------
class CTurretStructure : public CStructure
{
private:
	// DATA
	//---------------------------
	CGameObject* mAttackTarget = nullptr;
	vector<SProjectile*>mpProjectiles;
	vector<CExplosion*>mpAttackExplosions;

	float mRange = 100.0f;
	float mFireRate = 5.0f;
	float mDamage = 5.0f;
	float mAttackTimer = mFireRate;

public:
	static IMesh* mspMshTurret;
	static IMesh* mspMshTurretShell;

	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CTurretStructure(DX::XMFLOAT3 position);
	CTurretStructure();
	virtual ~CTurretStructure();


	// ACCESSORS
	//---------------------------
	int GetQueueSize()
	{
		return 0;
	}

	CGameObject* GetAttackTarget()
	{
		return mAttackTarget;
	}

	void SetAttackTarget(CGameObject* target)
	{
		mAttackTarget = target;
	}

	float GetRange()
	{
		return mRange;
	}


	// MUTATORS
	//---------------------------



	// METHODS
	//---------------------------


	// OVERRIDE METHODS
	//---------------------------
	// Adds a unit to the queue based on the index which has been passed in
	virtual EErrorTypes AddToQueue(size_t agentIndex, CRTSPlayer* pPlayer);
	// Removes a unit in the queue at the given index
	virtual void RemoveFromQueue(size_t agentIndex, CRTSPlayer* pPlayer)
	{

	}

	// Returns a pointer to the queue of units
	virtual std::deque<CGameAgent*>* GetQueue()
	{
		return nullptr;
	}

	virtual void SetBuiltModel()
	{

	}

	// Method called each frome - boolean returns is false when the object has been destroyed
	void CalculateBoundingBox();
	virtual bool Update(CRTSPlayer* pPlayer);
	virtual void DisplayInfo(IFont* font);
	virtual void UnloadIModel();
	virtual void LoadIModel();

	bool Attack(CGameObject* ptarget, float hitMod, float damageMod);

	// Save the structure's details
	virtual void SaveStructure(std::ofstream& outFile) override;
	virtual void LoadStructure(std::ifstream& inFile, CGrid* pGrid, CRTSPlayer* pPlayer) override;
};

#endif /* _AATURRET_STRUCTURE_H_ */
