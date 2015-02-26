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
	mAgentInfo = SAgentData(GAV_TRANSPORT, "Transport");

	mHealth = 100.0f;
	mSpeed = 1.0f;
	mProductionTime = 25.0f;
	mProductionCost = 0.0f;
	mCurProductionTimeLeft = mProductionTime;
	mDamage = 0.5f;
	//mAttackParticleFX;
	//mDestroyParticleFX;
	mState = OBJ_CONSTRUCTING;
	//mDestGridSq;
	mIsMoving = false;
	mPopCost = 1;	
	mHitChance = 0.4f;
	mScale = 0.3f;
}

CTransport::~CTransport()
{

}

//-----------------------------------------------------
// TRANSPORT CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CTransport::Attack(CGameAgent* target, float hitMod, float damageMod)
{
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


//CTransport::void MoveTo(CTile* dest)


bool CTransport::Move()
{
	return false;
}

bool CTransport::Destroy()
{
	return false;
}