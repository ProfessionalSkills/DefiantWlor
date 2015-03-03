//-----------------------------------------------------
// FILE: GroundUnit.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "GroundUnit.h"

//-----------------------------------------------------
// GROUND UNIT CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CGroundUnit::CGroundUnit()
{
	mScale = 2.0f;

}

CGroundUnit::~CGroundUnit()
{	

}


//-----------------------------------------------------
// GROUND UNIT CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CGroundUnit::Attack(CGameAgent* target, float hitMod, float damageMod)
{
	return false;
}

//CGroundUnit::void MoveTo(CTile* dest)

void CGroundUnit::Spawn(CGrid* pGrid, SPointData pCentre)
{					
	//Sets the tile usage so vehicles cannot overlap and spawns the vehicle in

	mpObjModel = CreateModel(DX::XMFLOAT3(pGrid->GetTileData(pCentre)->GetWorldPos().x, 1.0f, (pGrid->GetTileData(pCentre)->GetWorldPos().z)));
	mWorldPos.x = pGrid->GetTileData(pCentre)->GetWorldPos().x;
	mWorldPos.y = 1.0f;
	mWorldPos.z = pGrid->GetTileData(pCentre)->GetWorldPos().z;
	mpObjModel->Scale(mScale);

	DX::XMFLOAT3 centre = { mpObjModel->GetX(), mpObjModel->GetY(), mpObjModel->GetZ() };
	float radius = 5.0f;

	mBoundingSphere = SBoundingSphere(centre, radius);
}


bool CGroundUnit::Move()
{
	mpObjModel->SetPosition(mPathTarget->GetWorldPos().x, mPathTarget->GetWorldPos().y , mPathTarget->GetWorldPos().z);
	return false;
}

bool CGroundUnit::Destroy()
{
	return false;
}
