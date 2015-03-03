
//-----------------------------------------------------
// FILE: Fleet.h
//-----------------------------------------------------
#pragma once
//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Common.h"
#include "SpaceUnit.h"
#include "Randomiser.h"


//-----------------------------------------------------
// FLEET CLASS
//-----------------------------------------------------
class CRTSPlayer;

class CFleet
{
private:
	// DATA
	vector <CGameAgent*> mpFleet;//contains the ships that are being sent to attack
	CFleet* mpEnemyFleet;
	int mSize;
	int mNumSpaceFighter;
	int mNumMothership;
	int mNumTransport;

	CRTSPlayer* mpPlayer;

	// FLEET SHIP MODIFIERS
	//---------------------------
	float mDamegMod;//modifys the ships damage acording to the chosen tactics
	float mHitMod;//modifys the ships hit chance according to the choosen tactics

	// TACTICS
	//---------------------------
	enum tactics {Targeted, Rapid, None};
	tactics mFleetTactics;
	int mTargetedFireVariance;
	CRandomiser* mTarget;

	// POSITIONING
	//---------------------------
	const int mFleetRowSize;//maximum size of a row of ships
	const int mFleetRowSeperation;//distance between each row of ships
	int mFleetWidth;//number of ships that can be in a row. specifcally, the number of ships beyond the centre ship in a given direction
	int YSwitch(int x);

public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CFleet();
	~CFleet();

	// METHODS
	//---------------------------
	void UpdateCondition();//gets rid of dead ships and checks to see if the fleet has been defeated
	void Fight();//attacks another fleet
	void LoadShipModels(float xPos);

	// ACCESSORS
	//---------------------------
	CGameAgent*GetShip(int i);//returns ship at the choosen postion
	int GetSize();//returns the size of the fleet

	// MUTATORS
	//---------------------------
	void SetEnemy(CFleet* myEnemy);//sets the fleet that this one will attack
	vector <CGameAgent*>* LaunchFleet(vector <CGameAgent*>* possibleShips);//gets ships a player has, then adds a number of them to the fleet to attack
	void ReturnFleet(CRTSPlayer* Player);
	void SetTactic(string tactics);
};

