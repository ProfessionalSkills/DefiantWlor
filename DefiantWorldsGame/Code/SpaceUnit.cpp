//-----------------------------------------------------
// FILE: SpaceUnit.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "SpaceUnit.h"
IMesh* CSpaceUnit::mspMshLazer = nullptr;

//-----------------------------------------------------
// SPACE UNIT CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CSpaceUnit::CSpaceUnit()
{
	mSpeed = 50.0f;
	mpTempShield = nullptr;
	mpTempLazer = nullptr;

	mpToHitRoll = new CRandomiser();
}

CSpaceUnit::~CSpaceUnit()
{
	SafeDelete(mpToHitRoll);
}

//-----------------------------------------------------
// SPACE UNIT CLASS ACCESSOR METHODS
//-----------------------------------------------------


//-----------------------------------------------------
// SPACE UNIT CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CSpaceUnit::Attack(CGameAgent* target, float hitMod, float damageMod)
{
	return false;
}

void CSpaceUnit::Spawn(CGrid* pGrid, SPointData pCentre)
{

}

//CSpaceUnit::void MoveTo(CTile* dest)


bool CSpaceUnit::Move()
{
	return false;
}

bool CSpaceUnit::Destroy()
{
	return false;
}

void CSpaceUnit::UnloadIModel()
{

}

void CSpaceUnit::LoadIModel()
{

}

void CSpaceUnit::LoadModel(float x, float y, float z)
{

}
void CSpaceUnit::Update()
{

}

void HitFlash()
{
	IModel*TempShield;
}

float Sq(float x)
{
	return x*x;
}

void CSpaceUnit::FireLazer(CGameAgent* target)
{
	if (mpTempLazer) return;
	mpTempLazer = mspMshLazer->CreateModel(mWorldPos.x, mWorldPos.y, mWorldPos.z);
	if (mWorldPos.x > 0)
	{
		mpTempLazer->SetSkin("tlxadd_lazer - red.tga");
	}

	mpTempLazer->LookAt(target->GetWorldPos().x, target->GetWorldPos().y, target->GetWorldPos().z);
	float length = sqrtf(Sq(target->GetWorldPos().x - mWorldPos.x) + Sq(target->GetWorldPos().y - mWorldPos.y) + Sq(target->GetWorldPos().z - mWorldPos.z));
	mpTempLazer->ScaleZ(length);
	mpTempLazer->ScaleX(0.2f);
	mpTempLazer->ScaleY(0.2f);
}

void CSpaceUnit::UnloadLazer()
{
	if (mpTempLazer)
	{
		mspMshLazer->RemoveModel(mpTempLazer);
		mpTempLazer = nullptr;
	}
}