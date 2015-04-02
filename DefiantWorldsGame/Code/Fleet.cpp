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
CFleet::CFleet() :mFleetRowSize(20), mFleetRowSeperation(7), mFleetZAdjust(8), mFleetYCyleHeight(0.01f), mNumFleetSections(5)
{
	//Value Mods
	mDamegMod = 1.0f;
	mHitMod = 1.0f;
	mSize = 0;
	mTargetedFireVariance = 5;
	mFleetYAdjust = 20.0f;

	//Tactics
	mFleetTactics = None;
	mShotsFired = 0;
	mHits = 0;
	mFleetSectionFiring = 0;
	//Misc
	mTarget = new CRandomiser();
	mFleetYHeighCycle = mTarget->GetRandomFloat(0.0, 6.0);//starts the fleet adjust cycle on a random point, so both fleets dont just float at the same hight all the time
	mNumMothership = 0;
	mNumSpaceFighter = 0;
	mNumTransport = 0;

	mFleetWidth = (mFleetRowSize/2)-1;
}

CFleet::~CFleet()
{
	if (mTarget != 0) delete mTarget;
}

//-----------------------------------------------------
// FLEET CLASS METHODS
//-----------------------------------------------------
void CFleet::MoveFleet()
{
	for (int i = 0; i < mSize; i++)
	{
		mpFleet[i]->Move();
	}
}

void CFleet::IdleFleet()
{
	mFleetYHeighCycle += gFrameTime;
	float yChange=mFleetYCyleHeight*sinf(mFleetYHeighCycle);
	CSpaceUnit* mpTemp = nullptr;
	for (int i = 0; i < mSize; i++)
	{
		mpTemp = (CSpaceUnit*)(mpFleet[i]);
		mpTemp->MoveY(yChange);
	}
}

void CFleet::ChargeFleetLazers()
{
	for (int i = 0; i < mSize; i++)
	{
		CSpaceUnit* mpTemp = (CSpaceUnit*)(mpFleet[i]);
		mpTemp->ChargeLazer();
	}
}


void CFleet::Fight()
{
	if (mpEnemyFleet->GetSize() != 0)
	{
		int target = 0;
		
		switch (mFleetTactics)
		{
			case Tactics::None:
				//just gets each ship to attack one random enemy ship, no effects on accuracy or damage
				for (int i = mFleetSectionFiring*mSize / mNumFleetSections; i < (mFleetSectionFiring + 1)*mSize / mNumFleetSections; i++)
				{
					target = mTarget->GetRandomInt(0, mpEnemyFleet->GetSize() - 1);
					if (mpFleet[i]->Attack(mpEnemyFleet->GetShip(target), mHitMod, mDamegMod)) mHits++;
					mShotsFired++;
				}

				break;
			case Tactics::Rapid:
				//gets each ship to attack the same enemy twice
				//reduces accuracy of the ship
				for (int i = mFleetSectionFiring*mSize / mNumFleetSections; i < (mFleetSectionFiring + 1)*mSize / mNumFleetSections; i++)
				{
					target = mTarget->GetRandomInt(0, mpEnemyFleet->GetSize() - 1);
					if (mpFleet[i]->Attack(mpEnemyFleet->GetShip(target), mHitMod, mDamegMod)) mHits++;
					if (mpFleet[i]->Attack(mpEnemyFleet->GetShip(target), mHitMod, mDamegMod)) mHits++;
					mShotsFired++;
					mShotsFired++;
				}

				break;
			case Tactics::Targeted:
				//picks a section of the enemy fleet to attack
				//only attacks ship withn a range of that target
				target = mTarget->GetRandomInt(0, mpEnemyFleet->GetSize() - 1);
				int variance=0;
				for (int i = mFleetSectionFiring*mSize / mNumFleetSections; i < (mFleetSectionFiring + 1)*mSize / mNumFleetSections; i++)
				{
					mTarget->SetSeed((float)mHits, (float)mpEnemyFleet->GetShip(0)->GetHealth());
					mShotsFired++;
					if (mpEnemyFleet->GetSize()>1)
					{
						target = mTarget->GetRandomInt(-mTargetedFireVariance, mTargetedFireVariance);
						target = abs(target) % (mpEnemyFleet->GetSize() - 1);
					}
					else target = 0;
					if (mpFleet[i]->Attack(mpEnemyFleet->GetShip(target), mHitMod, mDamegMod)) mHits++;
				}
				break;
		}
		mFleetSectionFiring = (mFleetSectionFiring + 1) % mNumFleetSections;
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
			default:
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
	if (x < -mFleetWidth || x>mFleetWidth)return 0;
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
	int posMod = 1;
	int TransportRowsBack = (mNumSpaceFighter / mFleetRowSize)+1;
	int MotherShipRowsBack = (mNumTransport / mFleetRowSize) + TransportRowsBack + 1;

	if (xPos < 0.0f)
	{
		posMod = -1;
	}
	else
	{
		posMod = 1;
	}

	for (int i = 0; i < mSize; i++)
	{
		//uses intager deviosion to seperate ships into rows of x, where x is the fleet row size, and each row is seperated by a distance of fleet row seperation
		switch (mpFleet[i]->GetPosType())
		{
		case front:
			mpFleet[i]->LoadModel(xPos + (float)((SpaceFighterLoaded / mFleetRowSize) * (mFleetRowSeperation)*posMod),
				((float)SpaceFighterY*mFleetRowSeperation) + mFleetYAdjust, (float)mFleetZAdjust*SpaceFighterY);
			SpaceFighterLoaded++;
			SpaceFighterY = YSwitch(SpaceFighterY);
			break;
		case centre:
			mpFleet[i]->LoadModel(xPos + (float)(((TransportLoaded / mFleetRowSize) + TransportRowsBack + mpFleet[i]->GetUnitSpacing()) * mFleetRowSeperation*posMod),
				((float)TransportY*mFleetRowSeperation) + mFleetYAdjust, (float)mFleetZAdjust*TransportY);
			TransportLoaded++;
			TransportY = YSwitch(TransportY);
			break;
		case back:
			mpFleet[i]->LoadModel(xPos + (float)(((MothershipLoaded / mFleetRowSize) + MotherShipRowsBack + mpFleet[i]->GetUnitSpacing()) * mFleetRowSeperation*posMod),
				((float)MothershipY*mFleetRowSeperation) + mFleetYAdjust, (float)mFleetZAdjust*MothershipY);
			MothershipLoaded++;
			MothershipY = YSwitch(MothershipY);
			break;
		default:
			break;
		}
	}
}

void CFleet::UnloadShieldModels()
{
	for (int i = 0; i < mSize; i++)
	{
		// Unload the shield for this space unit
		CSpaceUnit* mpTemp = (CSpaceUnit*)(mpFleet[i]);
		mpTemp->UnloadFlash();
	}
}

void CFleet::UnloadLazers()
{
	for (int i = 0; i < mSize; i++)
	{
		// Unload the shield for this space unit
		CSpaceUnit* mpTemp = (CSpaceUnit*)(mpFleet[i]);
		mpTemp->UnloadLazer();
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

int CFleet::GetRows()
{
	return (mNumSpaceFighter / mFleetRowSize) + (mNumTransport / mFleetRowSize) + 1;
}

string CFleet::GetTacticsName()
{
	switch (mFleetTactics)
	{
	case Tactics::None:
		return "None";
	case Tactics::Rapid:
		return "Rapid";
	case Tactics::Targeted:
		return "Targeted";
	}
	return "Null";
}

float CFleet::GetFleetTotalHealth()
{
	float totalHp = 0;
	for (int i = 0; i < mSize; i++)
	{
		totalHp += mpFleet[i]->GetHealth();
	}
	return totalHp;
}

float CFleet::GetFleetAvargeHealth()
{
	if (GetFleetTotalHealth()!=0) return GetFleetTotalHealth() / (float)mSize;
	else return 0;
}

//-----------------------------------------------------
// FLEET CLASS MUTATORS
//-----------------------------------------------------

void CFleet::SetEnemy(CFleet* myEnemy)
{
	mpEnemyFleet = myEnemy;
}

void CFleet::ManFleet(vector <CGameAgent*> &UnitsToLoad)
{
	for (int i = 0; i < mpFleet.size(); i++)
	{
		CSpaceUnit* mpTemp = (CSpaceUnit*)(mpFleet[i]);
		for (int j = 0; j < UnitsToLoad.size(); j++)
		{
			if (!mpTemp->StoreUnits(UnitsToLoad[j])) break;
		}
	}
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
			default:
				break;
			}
			mpFleet.push_back((*possibleShips)[i]);
			possibleShips->pop_back();
			mSize++;
		}
	}
	return possibleShips;
}

void CFleet::SetTactic(Tactics tactics)//changes the tactics the fleet will use in battle, e.g rapid, targated
{
	mFleetTactics = tactics;
	//adds the fleet modifiers
	switch (mFleetTactics)
	{
	case Tactics::None:
		mDamegMod = 1.0f;
		mHitMod = 1.0f;

		break;
	case Tactics::Rapid:
		mDamegMod = 1.3f;
		mHitMod = 0.45f;

		break;
	case Tactics::Targeted:
		mDamegMod = 0.7f;
		mHitMod = 1.4f;

		break;
	}
	return;

}

//returns the fleet to the player
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
	mShotsFired = 0;
	mHits = 0;
}

