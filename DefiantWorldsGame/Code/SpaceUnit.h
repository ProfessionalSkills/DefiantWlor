//-----------------------------------------------------
// FILE: SpaceUnit.h
//-----------------------------------------------------

#ifndef _SPACE_UNIT_H_
#define _SPACE_UNIT_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "GameAgent.h"
#include "Randomiser.h"


//-----------------------------------------------------
// SPACE UNIT CLASS - CHILD OF GAME AGENT
//-----------------------------------------------------


class CSpaceUnit : public CGameAgent
{
protected:
	// DATA
	//---------------------------

	//ship attack chance
	float mHitChance;
	CRandomiser* mpToHitRoll;
	float mUnitSpacing;
	float mCargoValue;

	//lazer/shield models
	IModel* mpTempShield;
	IModel* mpTempLazer;

	//values to mange charging of lazers
	const float mChargeTimeMax;//max time a laser should charge for
	const float mChargeTimeMin;//minnimum time a lazer should charge for
	float mChargeTime;//time laser is currently charging for
	bool mChargingLazers;
	bool mCharged;
	bool mFiredLazer;

	//sound variables
	CSound* mSoundExplosion;	
	string  mSoundFileLazer;
	string mSoundFileExplosion;

	//scene variables
	//DX::XMFLOAT3 mTargetPos;
	float mTravelRadius;
	float mTravelSpeed;

public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CSpaceUnit();
	virtual ~CSpaceUnit();
	static IMesh* mspMshLazer;

	
	// ACCESSORS
	//---------------------------


	// MUTATORS
	//---------------------------

	// METHODS
	//---------------------------
	inline void Explode()
	{
		if (mSoundExplosion) mSoundExplosion->PlaySound();
	}
	// OVERRIDE METHODS
	//---------------------------
	virtual bool Attack(CGameObject* target,float hitMod, float damageMod);
	virtual void Spawn(CGrid* pGrid, SPointData pCentre);
	virtual bool Update();
	//virtual void MoveTo(CTile* dest);
	virtual void Move();
	virtual void UnloadIModel();
	virtual void LoadIModel();
	virtual void LoadModel(float x, float y, float z);
	virtual void HitFlash()=0;
	virtual void UnloadFlash() = 0;
	virtual void MoveY(float yChange)=0;
	virtual bool StoreUnits(CGameAgent* unit) = 0;
	// Save the data for this unit
	virtual void SaveAgent(std::ofstream& outFile) override;
	virtual void LoadAgent(std::ifstream& inFile) override;

	void FireLazer(CGameObject* target);
	void ChargeLazer();

	inline bool GetChargiingLazer()
	{
		return mCharged;
	}

	inline int GetCargoValue()
	{
		return mCargoValue;
	}
	void UnloadLazer();

	void SetTargetTexture()
	{
		// Do nothing for space units
	}

	void SetNormalTexture()
	{
		// Do nothing for space units
	}
};

#endif /* _SPACE_UNIT_H_ */
