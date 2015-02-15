#include "Fleet.h"


CFleet::CFleet()
{
	mDamegMod = 1.0f;
	mHitMod = 1.0f;
	mSize = 0;
	mFleetTactics = None;
}


CFleet::~CFleet()
{
	//returns ships to the player class
}


void CFleet::Fight()
{
	switch (mFleetTactics)
	{
		case tactics::None:
			for (int i = 0; i < mSize; i++)
			{
				CRandomiser shipToAttack;
				int target = shipToAttack.GetRandomInt(0, mpEnemyFleet->GetSize() - 1);
				mpFleet[i]->Attack(mpEnemyFleet->GetShip(target), mHitMod, mDamegMod);
			}
			break;
		case tactics::Rapid:
			for (int i = 0; i < mSize; i++)
			{
				CRandomiser shipToAttack;
				int target = shipToAttack.GetRandomInt(0, mpEnemyFleet->GetSize() - 1);
				mpFleet[i]->Attack(mpEnemyFleet->GetShip(target), mHitMod, mDamegMod);
				mpFleet[i]->Attack(mpEnemyFleet->GetShip(target), mHitMod, mDamegMod);
			}
			break;
		case tactics::Targeted:
			CRandomiser shipToAttack;
			int target = shipToAttack.GetRandomInt(0, mpEnemyFleet->GetSize() - 1);
			for (int i = 0; i < mSize; i++)
			{
				int variance = shipToAttack.GetRandomInt(target - 5, target + 5)%mSize;
				mpFleet[i]->Attack(mpEnemyFleet->GetShip(variance), mHitMod, mDamegMod);
			}
			break;
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

void CFleet::LaunchFleet(vector <CSpaceUnit*> possibleShips)
{
	//recives a vector of ships from player
	//asks player how many it should add to the fleet
	//pops them of player and onto fleet
	//asks the player to choose some tactics
	//moves game into space state?
}

void CFleet::SetTactic()
{
	//recives player input to set tactics, does so during launch attack
}