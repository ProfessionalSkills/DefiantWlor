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
CSpaceUnit::CSpaceUnit() :mChargeTimeMax(3.0f), mChargeTimeMin(1.0f)
{
	mSpeed = 5.0f;
	mpTempShield = nullptr;
	mpTempLazer = nullptr;

	mpToHitRoll = new CRandomiser();
	mChargingLazers = true;
	mChargeTime = mChargeTimeMax;
}

CSpaceUnit::~CSpaceUnit()
{
	SafeDelete(mpToHitRoll);
	if (mpTempLazer)
	{
		mpTempLazer->GetMesh()->RemoveModel(mpTempLazer);
		mpTempLazer = nullptr;
	}
	if (mpTempShield)
	{
		mpTempShield->GetMesh()->RemoveModel(mpTempShield);
		mpTempShield = nullptr;
	}
}

//-----------------------------------------------------
// SPACE UNIT CLASS ACCESSOR METHODS
//-----------------------------------------------------


//-----------------------------------------------------
// SPACE UNIT CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CSpaceUnit::Attack(CGameObject* target, float hitMod, float damageMod)
{
	return false;
}

void CSpaceUnit::Spawn(CGrid* pGrid, SPointData pCentre)
{

}

//CSpaceUnit::void MoveTo(CTile* dest)


void CSpaceUnit::Move()
{
	mpObjModel->MoveZ(mSpeed*gFrameTime);
	mWorldPos.z += mSpeed*gFrameTime;
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

bool CSpaceUnit::Update()
{
	return true;
}

float Sq(float x)
{
	return x*x;
}

void CSpaceUnit::FireLazer(CGameObject* target)
{
	//set models position
	DirectX::XMFLOAT4X4 ModelMatrix;
	DirectX::XMFLOAT3 ModelZNormal;

	mpObjModel->GetMatrix(&ModelMatrix.m[0][0]);

	ModelZNormal.x = ModelMatrix.m[2][0] * 6;
	ModelZNormal.y = ModelMatrix.m[2][1] * 6;
	ModelZNormal.z = ModelMatrix.m[2][2] * 6;

	if (!mpTempLazer)
	{
		mpTempLazer = mspMshLazer->CreateModel(mWorldPos.x + ModelZNormal.x, mWorldPos.y + ModelZNormal.y, mWorldPos.z + ModelZNormal.z);
		if (mWorldPos.x > 0)
		{
			mpTempLazer->SetSkin("tlxadd_lazer - red.tga");
		}
	}
	else
	{
		mpTempLazer->SetPosition(mWorldPos.x + ModelZNormal.x, mWorldPos.y + ModelZNormal.y, mWorldPos.z + ModelZNormal.z);
	}

	mpTempLazer->LookAt(target->GetWorldPos().x, target->GetWorldPos().y, target->GetWorldPos().z);
	float length = sqrtf(Sq(target->GetWorldPos().x - mWorldPos.x) + Sq(target->GetWorldPos().y - mWorldPos.y) + Sq(target->GetWorldPos().z - mWorldPos.z));

	mpTempLazer->ScaleZ(length);
	mpTempLazer->ScaleX(0.2f);
	mpTempLazer->ScaleY(0.2f);
	mChargingLazers = true;
}

void CSpaceUnit::ChargeLazer()
{
	if (mChargingLazers)
	{
		//set models position
		DirectX::XMFLOAT4X4 ModelMatrix;
		DirectX::XMFLOAT3 ModelZNormal;

		mpObjModel->GetMatrix(&ModelMatrix.m[0][0]);

		ModelZNormal.x = ModelMatrix.m[2][0] * 6;
		ModelZNormal.y = ModelMatrix.m[2][1] * 6;
		ModelZNormal.z = ModelMatrix.m[2][2] * 6;

		if (!mpTempLazer)
		{
			mpTempLazer = mspMshLazer->CreateModel(mWorldPos.x + ModelZNormal.x, mWorldPos.y + ModelZNormal.y, mWorldPos.z + ModelZNormal.z);
			if (mWorldPos.x > 0)
			{
				mpTempLazer->SetSkin("tlxadd_lazer - red.tga");
			}
		}
		else
		{
			mpTempLazer->SetPosition(mWorldPos.x + ModelZNormal.x, mWorldPos.y + ModelZNormal.y, mWorldPos.z + ModelZNormal.z);
		}

		mChargeTime -= gFrameTime;
		if (mChargeTime <= 0)
		{
			mChargeTime = mChargeTimeMax;
			mChargingLazers = false;
		}
	}
}

void CSpaceUnit::UnloadLazer()
{
	if (mpTempLazer&&!mChargingLazers)
	{
		DX::XMFLOAT4X4 tmp;
		mpTempLazer->ResetScale();
		mpTempLazer->GetMatrix(&tmp.m[0][0]);
		mpTempLazer->SetY(-5000);
	}
	else if (mpTempLazer)
	{
		mpTempLazer->ResetScale();
		mpTempLazer->Scale(1 / (mChargeTime + 1.0f));
	}
}