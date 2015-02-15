#include "Fleet.h"


CFleet::CFleet()
{
}


CFleet::~CFleet()
{
}


void CFleet::Fight()
{
	
	for (int i = 0; i < mSize; i++)
	{
		CRandomiser shipToAttack;
		int target = shipToAttack.GetRandomInt(0, mpEnemyFleet->GetSize() - 1);
		mpFleet[i]->Attack(mpEnemyFleet->GetShip(target), mHitMod, mDamegMod);
	}

}

void CFleet::UpdateCondition()
{
	for (int i = 0; i < mSize; i++)
	{
		if (mpFleet[i]->GetHealth() < 0.0f)
		{
			CSpaceUnit*temp = mpFleet[i];
			mpFleet[i] = mpFleet[mSize - 1];
			mpFleet.pop_back();
			delete temp;
			mSize--;
		}
	}
}

CSpaceUnit* CFleet::GetShip(int i)
{
	return mpFleet[i];
}

int CFleet::GetSize()
{
	return mSize;
}

void CFleet::SetEnemy(CFleet* myEnemy)
{
	mpEnemyFleet = myEnemy;
}
