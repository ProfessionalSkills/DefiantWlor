//-----------------------------------------------------
// FILE: AirUnit.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "AirUnit.h"


//-----------------------------------------------------
// AIR UNIT CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CAirUnit::CAirUnit()
{

}

CAirUnit::~CAirUnit()
{

}


//-----------------------------------------------------
// AIR UNIT CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CAirUnit::Attack(CGameAgent* target, float hitMod, float damageMod)
{
	return false;
}

void CAirUnit::Spawn(CGrid* pGrid, SPointData pCentre)
{
	mpObjModel = CreateModel(DX::XMFLOAT3(pGrid->GetTileData(pCentre)->GetWorldPos().x, 30.0f, (pGrid->GetTileData(pCentre)->GetWorldPos().z)));
	mWorldPos.x = pGrid->GetTileData(pCentre)->GetWorldPos().x;
	mWorldPos.y = 1.0f;
	mWorldPos.z = pGrid->GetTileData(pCentre)->GetWorldPos().z;
	mpObjModel->Scale(mScale);

	DX::XMFLOAT3 centre = { mpObjModel->GetX(), mpObjModel->GetY(), mpObjModel->GetZ() };
	float radius = 5.0f;

	mBoundingSphere = SBoundingSphere(centre, radius);
}
//CAirUnit::void MoveTo(CTile* dest)


bool CAirUnit::Move()
{
	return false;
}

void CAirUnit::Update()
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
				float movement = 10.0f * gFrameTime;
				mpObjModel->MoveLocalZ(movement);
				mWorldPos = DX::XMFLOAT3(mpObjModel->GetX(), mpObjModel->GetY(), mpObjModel->GetZ());
				DX::XMFLOAT3 moveAmount = { matrix[8] * movement, matrix[9] * movement, matrix[10] * movement };
				mBoundingSphere.Move(mWorldPos);
			}
		}
	}
	//int nodes = mpObjModel->GetNumNodes();
	//mpObjModel->GetNode(4)->RotateY(1000.0f * gFrameTime);
}

bool CAirUnit::LookingAt()
{

	DX::XMFLOAT3 targetPosition = { mPathTarget.x, 30.0f, mPathTarget.z };
	DX::XMFLOAT3 vectorZ = { (targetPosition.x - mpObjModel->GetX()), (targetPosition.y - mpObjModel->GetY()), (targetPosition.z - mpObjModel->GetZ()) };
	float matrix[16];
	mpObjModel->GetMatrix(matrix);

	DX::XMFLOAT3 facingVector = { matrix[8], matrix[9], matrix[10] };
	const DX::XMFLOAT3 kYAxis(0.0f, 1.0f, 0.0f);

	float dotProduct = Dot(vectorZ, Cross(kYAxis, facingVector));

	if (dotProduct > 0.1f)
	{
		mpObjModel->RotateY(200.0f * gFrameTime);
		return true;
	}
	else if (dotProduct < -0.1f)
	{
		mpObjModel->RotateY(-200.0f * gFrameTime);
		return true;
	}
	else
	{
		return true;
	}
}

bool CAirUnit::Destroy()
{
	return false;
}


