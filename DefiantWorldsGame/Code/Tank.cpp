//-----------------------------------------------------
// FILE: Tank.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Tank.h"

IMesh* CTank::mspMshTank = nullptr;

//-----------------------------------------------------
// TANK CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CTank::CTank()
{
	mAgentInfo = SAgentData(GAV_TANK, "Tank");
	mMaxHealth = 100.0f;
	mHealth = 100.0f;
	mSpeed = 1.0f;
	mProductionTime = 15.0f;
	mProductionCost = 0.0f;
	mCurProductionTimeLeft = mProductionTime;
	mDamage = 1.0f;
	hasTarget = false;
	//mAttackParticleFX;
	//mDestroyParticleFX;
	mState = OBJ_CONSTRUCTING;
	//mDestGridSq;
	mIsMoving = false;
	mScale = 1.5f;
	mBuildCost = 300;
	mPopCost = 3;
}

CTank::~CTank()
{

}

//-----------------------------------------------------
// TANK CLASS METHODS
//-----------------------------------------------------

void CTank::UnloadIModel()
{
	if (mpObjModel != 0)
	{
		mspMshTank->RemoveModel(mpObjModel);
		mpObjModel = nullptr;
	}
}

void CTank::LoadIModel()
{
	if (mpObjModel == nullptr)
	{
		mpObjModel = mspMshTank->CreateModel(mWorldPos.x, mWorldPos.y, mWorldPos.z);
		mpObjModel->Scale(mScale);
		mpObjModel->RotateY(180.0f);
		if (mFaction == FAC_EARTH_DEFENSE_FORCE)
		{
			mpObjModel->SetSkin("Hovertank01.jpg");
		}
		else
		{
			mpObjModel->SetSkin("Hovertank01Mars.jpg");
		}
	}
}


//-----------------------------------------------------
// TANK CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CTank::Attack(CGameObject* target, float hitMod, float damageMod)
{
	return false;
}

bool CTank::Destroy()
{
	UnloadIModel();
	return false;
}

IModel* CTank::CreateModel(DX::XMFLOAT3 pos)
{
	// Load model and set required skin
	IModel* pModel = mspMshTank->CreateModel(pos.x, pos.y, pos.z);
	if (mFaction == FAC_EARTH_DEFENSE_FORCE)
	{
		pModel->SetSkin("Hovertank01.jpg");
	}
	else
	{
		pModel->SetSkin("Hovertank01Mars.jpg");
	}

	return pModel;
}
