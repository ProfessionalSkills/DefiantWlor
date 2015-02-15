//-----------------------------------------------------
// FILE: Fleet.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Fleet.h"

//-----------------------------------------------------
// FLEET CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CFleet::CFleet()
{
	mDamegMod = 1.0f;
	mHitMod = 1.0f;
	mSize = 0;
	mFleetTactics = None;
	mTargetedFireVariance = 5;
}

CFleet::~CFleet()
{
	//returns ships to the player class
}

//-----------------------------------------------------
// FLEET CLASS METHODS
//-----------------------------------------------------
void CFleet::Fight()
{
	switch (mFleetTactics)
	{
		case tactics::None:
			//just gets each ship to attack one random enemy ship, no effects on accuracy or damage
			for (int i = 0; i < mSize; i++)
			{
				CRandomiser shipToAttack;
				int target = shipToAttack.GetRandomInt(0, mpEnemyFleet->GetSize() - 1);
				mpFleet[i]->Attack(mpEnemyFleet->GetShip(target), mHitMod, mDamegMod);
			}

			break;
		case tactics::Rapid:
			//gets each ship to attack the same enemy twice
			//reduces accuracy of the ship
			for (int i = 0; i < mSize; i++)
			{
				CRandomiser shipToAttack;
				int target = shipToAttack.GetRandomInt(0, mpEnemyFleet->GetSize() - 1);
				mpFleet[i]->Attack(mpEnemyFleet->GetShip(target), mHitMod, mDamegMod);
				mpFleet[i]->Attack(mpEnemyFleet->GetShip(target), mHitMod, mDamegMod);
			}

			break;
		case tactics::Targeted:
			//picks a section of the enemy fleet to attack
			//only attacks ship withn a range of that target
			CRandomiser shipToAttack;
			int target = shipToAttack.GetRandomInt(0, mpEnemyFleet->GetSize() - 1);
			for (int i = 0; i < mSize; i++)
			{
				int variance = shipToAttack.GetRandomInt(target - mTargetedFireVariance, target + mTargetedFireVariance) % mSize;
				mpFleet[i]->Attack(mpEnemyFleet->GetShip(variance), mHitMod, mDamegMod);
			}

			break;
	}
}

void CFleet::UpdateCondition()
{
	//gets the health of all the ships in the fleet, then gets rid of any that are less than zero
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

//-----------------------------------------------------
// FLEET CLASS ACCESSORS
//-----------------------------------------------------

CSpaceUnit* CFleet::GetShip(int i)
{
	return mpFleet[i];
}

int CFleet::GetSize()
{
	return mSize;
}

//-----------------------------------------------------
// FLEET CLASS MUTATORS
//-----------------------------------------------------

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