//-----------------------------------------------------
// FILE: SpaceFighter.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "SpaceFighter.h"
IMesh* CSpaceFighter::mspMshSpaceFighter = nullptr;

//-----------------------------------------------------
// SPACE FIGHTER CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CSpaceFighter::CSpaceFighter()
{
	mAgentInfo = SAgentData(GAV_SPACE_FIGHTER, "Space Fighter");

	mHealth = 100.0f;
	mSpeed = 1.0f;
	mProductionTime = 20.0f;
	mProductionCost = 0.0f;
	mCurProductionTimeLeft = mProductionTime;
	mDamage = 1.0f;
	//mAttackParticleFX;
	//mDestroyParticleFX;
	mState = OBJ_CONSTRUCTING;
	//mDestGridSq;
	mIsMoving = false;
	mPopCost = 1;
	mHitChance = 0.7f;
	mScale = 0.1f;
}

CSpaceFighter::~CSpaceFighter()
{
	RemoveModel();
}

//-----------------------------------------------------
// SPACE FIGHTER CLASS METHODS
//-----------------------------------------------------
void CSpaceFighter::LoadModel(float x,float y, float z)
{
	mpObjModel = mspMshSpaceFighter->CreateModel(x, y, z);
	mpObjModel->SetSkin("Spaceship02Battlecruiser.jpg");
	if (z < 0.0f)
	{
		mpObjModel->RotateLocalY(90.0f); 
	}
	else
	{
		mpObjModel->RotateLocalY(-90.0f);
	}
	mpObjModel->Scale(mScale);
}

void CSpaceFighter::RemoveModel()
{
	if (mpObjModel != 0)
	{
		mspMshSpaceFighter->RemoveModel(mpObjModel);
	}
}

//-----------------------------------------------------
// SPACE FIGHTER CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CSpaceFighter::Attack(CGameAgent* target, float hitMod, float damageMod)
{
	CRandomiser toHitRoll;
	if (toHitRoll.GetRandomFloat(1.0,100.0) < (hitMod*mHitChance) * 100)
	{
		target->TakeDamage(mDamage*damageMod);
		return true;
	}
	return false;
}

void CSpaceFighter::Spawn(CGrid* pGrid, SPointData pCentre)
{

}


//CSpaceFighter::void MoveTo(CTile* dest)


bool CSpaceFighter::Move()
{
	return false;
}

bool CSpaceFighter::Destroy()
{
	return false;
}
