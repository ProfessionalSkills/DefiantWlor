//-----------------------------------------------------
// FILE: Bomber.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Bomber.h"

IMesh* CBomber::mspMshBomber = nullptr;


//-----------------------------------------------------
// BOMBER CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CBomber::CBomber()
{
	mAgentInfo = SAgentData(GAV_BOMBER, "Bomber");
	mMaxHealth = 100.0f;
	mHealth = 100.0f;
	mSpeed = 1.0f;
	mProductionTime = 10.0f;
	mProductionCost = 0.0f;
	mCurProductionTimeLeft = mProductionTime;				
	mDamage = 1.0f;
	//mAttackParticleFX;
	//mDestroyParticleFX;
	mState = OBJ_CONSTRUCTING;
	//mDestGridSq;
	mIsMoving = false;
	hasTarget = false;
	mPopCost = 1;
	mScale = 1.5f;
	mBuildCost = 900;
}

CBomber::~CBomber()
{

}

//-----------------------------------------------------
// BOMBER CLASS METHODS
//-----------------------------------------------------

void CBomber::UnloadIModel()
{
	if (mpObjModel != nullptr)
	{
		mspMshBomber->RemoveModel(mpObjModel);
		mpObjModel = nullptr;
	}
}

void CBomber::LoadIModel()
{
	if (mpObjModel == nullptr)
	{
		mpObjModel = mspMshBomber->CreateModel(mWorldPos.x, mWorldPos.y, mWorldPos.z);
		mpObjModel->Scale(mScale);
		mpObjModel->RotateY(180.0f);
	}
}


//-----------------------------------------------------
// BOMBER CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CBomber::Attack(CGameAgent* target, float hitMod, float damageMod)
{
	return false;
}

//CBomber::void MoveTo(CTile* dest)


bool CBomber::Move()
{
	return false;
}

bool CBomber::Destroy()
{
	return false;
}

IModel* CBomber::CreateModel(DX::XMFLOAT3 pos)
{
	return mspMshBomber->CreateModel(pos.x, pos.y, pos.z);
}
