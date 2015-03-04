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
	mpObjModel->RotateY(180.0f);
	mWorldPos.x = pGrid->GetTileData(pCentre)->GetWorldPos().x;
	mWorldPos.y = 1.0f;
	mWorldPos.z = pGrid->GetTileData(pCentre)->GetWorldPos().z;
	mpObjModel->Scale(mScale);

	DX::XMFLOAT3 centre = { mpObjModel->GetX(), mpObjModel->GetY(), mpObjModel->GetZ() };
	float radius = 5.0f;

	mBoundingSphere = SBoundingSphere(centre, radius);
}


void CGroundUnit::Update( )
{
	if (HasTarget())
	{
		if (LookingAt())
		{
			int MaxX = mPathTarget.x + 1.0f;
			int MinX = mPathTarget.x - 1.0f;

			int MaxZ = mPathTarget.z + 1.0f;
			int MinZ = mPathTarget.z - 1.0f;

			if (mWorldPos.x > MinX && mWorldPos.x < MaxX && mWorldPos.z > MinZ && mWorldPos.z < MaxZ)
			{
			
			}
			else
			{
				float matrix[16];
				mpObjModel->GetMatrix(matrix);
				float movement = 5.0f * gFrameTime;
				mpObjModel->MoveLocalZ(movement);
				mWorldPos = DX::XMFLOAT3(mpObjModel->GetX(), mpObjModel->GetY(), mpObjModel->GetZ());
				DX::XMFLOAT3 moveAmount = { matrix[8] * movement, matrix[9] * movement, matrix[10] * movement };
				mBoundingSphere.Move(mWorldPos);
			}
		}
	}
	//mpObjModel->SetPosition(mPathTarget->GetWorldPos().x, mPathTarget->GetWorldPos().y , mPathTarget->GetWorldPos().z);
}

bool CGroundUnit::LookingAt()
{
	DX::XMFLOAT3 targetPosition = { mPathTarget };
	DX::XMFLOAT3 vectorZ = { (targetPosition.x - mpObjModel->GetX()), (targetPosition.y - mpObjModel->GetY()), (targetPosition.z - mpObjModel->GetZ()) };
	float matrix[16];
	mpObjModel->GetMatrix(matrix);

	DX::XMFLOAT3 facingVector = { matrix[8], matrix[9], matrix[10] };
	const DX::XMFLOAT3 kYAxis(0.0f, 1.0f, 0.0f);

	float dotProduct = Dot(vectorZ, Cross(kYAxis, facingVector));

	if (dotProduct > 0.1f)
	{
		mpObjModel->RotateY(50.0f * gFrameTime);
		return false;
	}
	else if (dotProduct < -0.1f)
	{
		mpObjModel->RotateY(-50.0f * gFrameTime);
		return false;
	}
	else
	{
		return true;
	}
}

bool CGroundUnit::Destroy()
{
	return false;
}
