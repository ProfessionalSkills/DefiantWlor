//-----------------------------------------------------
// FILE: Fleet.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Fleet.h"
#include "GameStateControl.h"


//-----------------------------------------------------
// FLEET CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CFleet::CFleet() :mFleetRowSize(15), mFleetRowSeperation(7)
{
	//Value Mods
	mDamegMod = 1.0f;
	mHitMod = 1.0f;
	mSize = 0;
	mTargetedFireVariance = 5;

	//Tactics
	mFleetTactics = None;

	//Misc
	mTarget = new CRandomiser();

	mNumMothership = 0;
	mNumSpaceFighter = 0;
	mNumTransport = 0;

	mFleetWidth = (mFleetRowSize/2)-1;
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
	if (mpEnemyFleet->GetSize() != 0)
	{
		switch (mFleetTactics)
		{
			case tactics::None:
				//just gets each ship to attack one random enemy ship, no effects on accuracy or damage
				for (int i = 0; i < mSize; i++)
				{
					int target = mTarget->GetRandomInt(0, mpEnemyFleet->GetSize() - 1);
					mpFleet[i]->Attack(mpEnemyFleet->GetShip(target), mHitMod, mDamegMod);
				}

				break;
			case tactics::Rapid:
				//gets each ship to attack the same enemy twice
				//reduces accuracy of the ship
				for (int i = 0; i < mSize; i++)
				{
					int target = mTarget->GetRandomInt(0, mpEnemyFleet->GetSize() - 1);
					mpFleet[i]->Attack(mpEnemyFleet->GetShip(target), mHitMod, mDamegMod);
					mpFleet[i]->Attack(mpEnemyFleet->GetShip(target), mHitMod, mDamegMod);
				}

				break;
			case tactics::Targeted:
				//picks a section of the enemy fleet to attack
				//only attacks ship withn a range of that target
				int target = mTarget->GetRandomInt(0, mpEnemyFleet->GetSize() - 1);
				for (int i = 0; i < mSize; i++)
				{
					int variance = mTarget->GetRandomInt(target - mTargetedFireVariance, target + mTargetedFireVariance) % mSize;
					mpFleet[i]->Attack(mpEnemyFleet->GetShip(variance), mHitMod, mDamegMod);
				}

				break;
		}
	}
}

void CFleet::UpdateCondition()
{
	//gets the health of all the ships in the fleet, then gets rid of any that are less than zero
	for (int i = 0; i < mSize; i++)
	{
		if (mpFleet[i]->GetHealth() < 0.0f)
		{
			switch (mpFleet[i]->GetPosType())
			{
			case front:
				mNumSpaceFighter--;
				break;
			case centre:
				mNumTransport--;
				break;
			case back:
				mNumMothership--;
				break;
			}

			CGameAgent*temp = mpFleet[i];
			mpFleet[i] = mpFleet[mSize - 1];
			mpFleet.pop_back();
			delete temp;
			mSize--;
		}
	}
}

int CFleet::YSwitch(int x)
{
	if (x < -mFleetWidth || x>mFleetWidth) return 0;
	if (x > 0) return x*-1;
	else return (x*-1) + 1;
}

void CFleet::LoadShipModels(float xPos)
{
	int SpaceFighterLoaded = 0;
	int TransportLoaded = 0;
	int MothershipLoaded = 0;

	int SpaceFighterY = 0;
	int TransportY = 0;
	int MothershipY = 0;

	int TransportRowsBack = (mNumSpaceFighter / mFleetRowSize)+1;
	int MotherShipRowsBack = (mNumTransport / mFleetRowSize) + TransportRowsBack + 1;

	for (int i = 0; i < mSize; i++)
	{
		//uses intager deviosion to seperate ships into rows of x, where x is the fleet row size, and each row is seperated by a distance of fleet row seperation
		if (xPos < 0.0f)
		{
			switch (mpFleet[i]->GetPosType())
			{
			case front:
				mpFleet[i]->LoadModel(xPos - (float)((SpaceFighterLoaded / mFleetRowSize) * mFleetRowSeperation), (float)SpaceFighterY, 0.0f);
				SpaceFighterLoaded++;
				SpaceFighterY = YSwitch(SpaceFighterY);
				break;
			case centre:
				mpFleet[i]->LoadModel(xPos - (float)(((TransportLoaded / mFleetRowSize) + TransportRowsBack) * mFleetRowSeperation), (float)(TransportLoaded - (TransportLoaded / mFleetRowSize) * mFleetRowSize), 0.0f);
				TransportLoaded++;
				break;
			case back:
				mpFleet[i]->LoadModel(xPos - (float)(((MothershipLoaded / mFleetRowSize) + MotherShipRowsBack) * mFleetRowSeperation), (float)(MothershipLoaded - (MothershipLoaded / mFleetRowSize) * mFleetRowSize), 0.0f);
				MothershipLoaded++;
				break;
			default:
				break;
			}
		}
		else
		{
			switch (mpFleet[i]->GetPosType())
			{
			case front:
				mpFleet[i]->LoadModel(xPos + (float)((SpaceFighterLoaded / mFleetRowSize) * mFleetRowSeperation), (float)SpaceFighterY, 0.0f);
				SpaceFighterLoaded++;
				SpaceFighterY = YSwitch(SpaceFighterY);
				break;
			case centre:
				mpFleet[i]->LoadModel(xPos + (float)(((TransportLoaded / mFleetRowSize) + TransportRowsBack) * mFleetRowSeperation), (float)(TransportLoaded - (TransportLoaded / mFleetRowSize) * mFleetRowSize), 0.0f);
				TransportLoaded++;
				break;
			case back:
				mpFleet[i]->LoadModel(xPos + (float)(((MothershipLoaded / mFleetRowSize) + MotherShipRowsBack) * mFleetRowSeperation), (float)(MothershipLoaded - (MothershipLoaded / mFleetRowSize) * mFleetRowSize), 0.0f);
				MothershipLoaded++;
				break;
			default:
				break;
			}
		}
	}
}

//-----------------------------------------------------
// FLEET CLASS ACCESSORS
//-----------------------------------------------------

CGameAgent* CFleet::GetShip(int i)
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

vector <CGameAgent*>* CFleet::LaunchFleet(vector <CGameAgent*>* possibleShips)
{
	//temporary functionthat just transfers all of the ships from player onto fleet
	if (possibleShips->size() != 0)
	{
		for (int i = possibleShips->size() - 1; i >= 0; i--)
		{
			switch ((*possibleShips)[i]->GetPosType())
			{
			case front:
				mNumSpaceFighter++;
				break;
			case centre:
				mNumTransport++;
				break;
			case back:
				mNumMothership++;
				break;
			}
			mpFleet.push_back((*possibleShips)[i]);
			possibleShips->pop_back();
			mSize++;
		}
	}
	return possibleShips;
	//recives a vector of ships from player
	//asks player how many it should add to the fleet
	//pops them of player and onto fleet
	//asks the player to choose some tactics
}

void CFleet::SetTactic()
{
	//recives player input to set tactics, does so during launch attack
}

void CFleet::ReturnFleet(CRTSPlayer* Player)
{
	for (int i = mSize-1; i >= 0; i--)
	{
		mpFleet[i]->UnloadIModel();
		Player->GetSpaceUnitList()->push_back(mpFleet[i]);
		mpFleet.pop_back();
		mSize--;
	}

	Player->SetNumMothership(mNumMothership);
	Player->SetNumSpaceFighter(mNumSpaceFighter);
	Player->SetNumTransport(mNumTransport);

	mNumMothership = 0;
	mNumSpaceFighter = 0;
	mNumTransport = 0;
}

