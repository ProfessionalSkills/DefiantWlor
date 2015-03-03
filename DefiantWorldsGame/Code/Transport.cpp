//-----------------------------------------------------
// FILE: Transport.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Transport.h"
IMesh* CTransport::mspMshTransport = nullptr;


//-----------------------------------------------------
// TRANSPORT CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CTransport::CTransport()
{
	//Game Values
	mHealth = 100.0f;
	mSpeed = 1.0f;
	mDamage = 0.5f;
	mHitChance = 0.4f;
	mFleetPosition = centre;
	mPopCost = 1;	

	//Production Values
	mProductionTime = 0.5f;
	mProductionCost = 0.0f;
	mCurProductionTimeLeft = mProductionTime;
	
	//Model Values
	mScale = 0.1f;
	
	//Misc
	//mAttackParticleFX;
	//mDestroyParticleFX;
	mState = OBJ_CONSTRUCTING;
	//mDestGridSq;
	mIsMoving = false;
	hasTarget = false;
	mAgentInfo = SAgentData(GAV_TRANSPORT, "Transport");
}

CTransport::~CTransport()
{
	UnloadIModel();
}

//-----------------------------------------------------
// TRANSPORT CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CTransport::Attack(CGameAgent* target, float hitMod, float damageMod)
{
	CRandomiser toHitRoll;
	if (toHitRoll.GetRandomFloat(1.0, 100.0) < (hitMod*mHitChance) * 100)
	{
		target->TakeDamage(mDamage*damageMod);
		return true;
	}
	return false;
}

void CTransport::Spawn(CGrid* pGrid, SPointData pCentre)
{

}

void CTransport::LoadModel(float x, float y, float z)
{
	mpObjModel = mspMshTransport->CreateModel(x, y, z);
	//mpObjModel->SetSkin("Spaceship02Battlecruiser.jpg");
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


void CTransport::UnloadIModel()
{
	if (mpObjModel != 0)
	{
		mspMshTransport->RemoveModel(mpObjModel);
		mpObjModel = nullptr;
	}
}



bool CTransport::Move()
{
	return false;
}

bool CTransport::Destroy()
{
	return false;
}