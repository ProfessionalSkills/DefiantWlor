//-----------------------------------------------------
// FILE: Mothership.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Mothership.h"
IMesh* CMothership::mspMshMothership = nullptr;

//-----------------------------------------------------
// MOTHERSHIP CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CMothership::CMothership()
{
	mAgentInfo = SAgentData(GAV_MOTHERSHIP, "Mothership");

	//Game Values
	mHealth = 100.0f;
	mSpeed = 1.0f;
	mDamage = 4.0f;
	mPopCost = 1;
	mHitChance = 0.5f;
	mFleetPosition = back;

	//Production Values
	mProductionTime = 4.0f;
	mProductionCost = 0.0f;
	mCurProductionTimeLeft = mProductionTime;

	//Model Values
	mScale = 0.2f;

	//Misc
	//mAttackParticleFX;
	//mDestroyParticleFX;
	mState = OBJ_CONSTRUCTING;
	//mDestGridSq;
	mIsMoving = false;
}

CMothership::~CMothership()
{
	UnloadIModel();
}


//-----------------------------------------------------
// MOTHERSHIP CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CMothership::Attack(CGameAgent* target, float hitMod, float damageMod)
{
	CRandomiser toHitRoll;
	if (toHitRoll.GetRandomFloat(1.0, 100.0) < (hitMod*mHitChance) * 100)
	{
		target->TakeDamage(mDamage*damageMod);
		return true;
	}
	return false;
}
void CMothership::Spawn(CGrid* pGrid, SPointData pCentre)
{

}

void CMothership::LoadModel(float x, float y, float z)
{
	mpObjModel = mspMshMothership->CreateModel(x, y, z);
	if (x < 0.0f)
	{
		mpObjModel->RotateLocalY(90.0f);
	}
	else
	{
		mpObjModel->RotateLocalY(-90.0f);
	}
	mpObjModel->Scale(mScale);
}

void CMothership::UnloadIModel()
{
	if (mpObjModel != 0)
	{
		mspMshMothership->RemoveModel(mpObjModel);
		mpObjModel = nullptr;
	}
}



//CMothership::void MoveTo(CTile* dest)


bool CMothership::Move()
{
	return false;
}

bool CMothership::Destroy()
{
	return false;
}

