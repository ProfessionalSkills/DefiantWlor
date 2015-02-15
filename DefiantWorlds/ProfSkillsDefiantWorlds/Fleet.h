
//-----------------------------------------------------
// FILE: Fleet.h
//-----------------------------------------------------
#pragma once
//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Common.h"
#include "SpaceUnit.h"
//-----------------------------------------------------
// FLEET CLASS
//-----------------------------------------------------
class CFleet
{
private:
	vector <CSpaceUnit*> mpFleet;//contains the ships that are being sent to attack
	float mDamegMod;//modifys the ships damage acording to the chosen tactics
	float mHitMod;//modifys the ships hit chance according to the choosen tactics
	CFleet* mpEnemyFleet;
	
	int mSize;
public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CFleet();
	~CFleet();

	// METHODS
	//---------------------------
	void UpdateCondition();//gets rid of dead ships and checks to see if the fleet has been defeated
	void Fight();//attacks another fleet

	// ACCESSORS
	//---------------------------
	CSpaceUnit*GetShip(int i);//returns ship at the choosen postion
	int GetSize();//returns the size of the fleet

	// MUTATORS
	//---------------------------
	void SetEnemy(CFleet* myEnemy);//sets the fleet that this one will attack
};

