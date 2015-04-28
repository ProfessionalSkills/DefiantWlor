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
	DX::XMFLOAT3 mTargetPos;
	DX::XMFLOAT3 mPivotPos;
	float mSceneTravelRadius;
	float mSceneTravelSpeed;
	float mSceneRotationSpeed;

public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CSpaceUnit();
	virtual ~CSpaceUnit();
	static IMesh* mspMshLazer;

	
	// ACCESSORS
	//---------------------------
	inline bool GetChargiingLazer()
	{
		return mCharged;
	}

	inline int GetCargoValue()
	{
		return mCargoValue;
	}

	inline DX::XMFLOAT3 GetTargetPos()
	{
		return mTargetPos;
	}

	float GetTravelSpeed()
	{
		return mSceneTravelSpeed;
	}

	float GetTravelRadius()
	{
		return mSceneTravelRadius;
	}
	// MUTATORS
	//---------------------------
	inline void SetTargetPos(float x, float y, float z)
	{
		mTargetPos = { x, y, z };
	}

	void SetTargetTexture()
	{
		// Do nothing for space units
	}

	void SetNormalTexture()
	{
		// Do nothing for space units
	}

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
	//manage shield
	virtual void HitFlash()=0;
	virtual void UnloadFlash() = 0;

	virtual void MoveY(float yChange)=0;
	virtual bool StoreUnits(CGameAgent* unit) = 0;
	// Save the data for this unit
	void SaveAgent(std::ofstream& outFile) override;
	void LoadAgent(std::ifstream& inFile) override;

	//manage lazer
	void FireLazer(CGameObject* target);
	void ChargeLazer();
	void UnloadLazer();
	//scene
	inline void CalculateSceneValues(float sceneTime, float StartX)
	{
		mSceneTravelRadius = abs(StartX - mTargetPos.x);
		float TravelDistance = (PI*mSceneTravelRadius) / 2;
		mSceneTravelSpeed = TravelDistance / sceneTime;
		mSceneRotationSpeed = 90.0f / sceneTime;
		if (StartX < 0)
		{
			mSceneRotationSpeed = mSceneRotationSpeed *- 1.0f;
		}
		mPivotPos.x = StartX;
		mPivotPos.z = mTargetPos.z;
	}

	inline void MoveInScene()
	{
		mOrientation -= mSceneRotationSpeed*gFrameTime;
		mpObjModel->SetX(-mPivotPos.x+(mSceneTravelRadius*sinf(ToRadians(mOrientation))));
		mpObjModel->SetZ(-mPivotPos.z+(mSceneTravelRadius*cosf(ToRadians(mOrientation))));
		mpObjModel->RotateY(-mSceneRotationSpeed*gFrameTime);
		mWorldPos.x = mpObjModel->GetX();
		mWorldPos.y = mpObjModel->GetY();
		mWorldPos.z = mpObjModel->GetZ();
	}

};

#endif /* _SPACE_UNIT_H_ */
