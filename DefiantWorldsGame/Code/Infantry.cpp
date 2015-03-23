//-----------------------------------------------------
// FILE: Infantry.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Infantry.h"

IMesh* CInfantry::mspMshInfantry = nullptr;


//-----------------------------------------------------
// INFANTRY CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CInfantry::CInfantry()
{
	mAgentInfo = SAgentData(GAV_INFANTRY, "Infantry");
	mMaxHealth = 100.0f;
	mHealth = 100.0f;
	mSpeed = 1.0f;
	mProductionTime = 10.0f;
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
	mBuildCost = 100;
	mPopCost = 2;
}

CInfantry::~CInfantry()
{

}

//-----------------------------------------------------
// INFANTRY CLASS METHODS
//-----------------------------------------------------

void CInfantry::UnloadIModel()
{
	if (mpObjModel != 0)
	{
		mspMshInfantry->RemoveModel(mpObjModel);
		mpObjModel = nullptr;
	}
}

void CInfantry::LoadIModel()
{
	if (mpObjModel == nullptr)
	{
		mpObjModel = mspMshInfantry->CreateModel(mWorldPos.x, mWorldPos.y, mWorldPos.z);
		mpObjModel->Scale(mScale);
		mpObjModel->RotateY(180.0f);
		if (mFaction == FAC_EARTH_DEFENSE_FORCE)
		{
			mpObjModel->SetSkin("tiger.jpg");
		}
		else
		{
			mpObjModel->SetSkin("tigerMars.jpg");
		}
	}
}


//-----------------------------------------------------
// INFANTRY CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CInfantry::Attack(CGameAgent* target, float hitMod, float damageMod)
{
	return false;
}

//CInfantry::void MoveTo(CTile* dest)


bool CInfantry::Move()
{
	//mpObjModel->SetPosition(mPathTarget->GetWorldPos().x, 0, mPathTarget->GetWorldPos().z);
	return false;
}

bool CInfantry::Destroy()
{
	UnloadIModel();
	return false;
}

IModel* CInfantry::CreateModel(DX::XMFLOAT3 pos)
{
	// Load model and set required skin
	IModel* pModel = mspMshInfantry->CreateModel(pos.x, pos.y, pos.z);
	if (mFaction == FAC_EARTH_DEFENSE_FORCE)
	{
		pModel->SetSkin("tiger.jpg");
	}
	else
	{
		pModel->SetSkin("tigerMars.jpg");
	}

	return pModel;
}
