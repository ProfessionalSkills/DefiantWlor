//-----------------------------------------------------
// FILE: GameObject.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "GameObject.h"


//-----------------------------------------------------
// GAME OBJECT CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CGameObject::CGameObject()
{
	mpObjModel = nullptr;
}

CGameObject::~CGameObject()
{

}


//-----------------------------------------------------
// GAME OBJECT CLASS MUTATORS
//-----------------------------------------------------
void CGameObject::SetGridPos(SPointData pos)
{

}

void CGameObject::SetWorldPos(DX::XMFLOAT3 pos)
{
	mpObjModel->SetPosition(pos.x, pos.y, pos.z);
}

void CGameObject::SetOrientation(float orientation)
{

}

void CGameObject::ChangeRelativeOrientation(float amount)
{

}


//-----------------------------------------------------
// GAME OBJECT CLASS METHODS
//-----------------------------------------------------
void CGameObject::InitBaseVariables()
{

}
