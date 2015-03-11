//-----------------------------------------------------
// FILE: Transport.h
//-----------------------------------------------------

#ifndef _TRANSPORT_H_
#define _TRANSPORT_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "SpaceUnit.h"


//-----------------------------------------------------
// TRANSPORTER CLASS - CHILD OF SPACE UNIT
//-----------------------------------------------------
class CTransport : public CSpaceUnit
{
private:
	// DATA
	//---------------------------
	std::vector<CGameAgent*> mpSpaceUnitsList;
	int mUnitCapacity;
	int mNumUnitsHeld;
public:
	static IMesh* mspMshTransport;
	static IMesh* mspMshSheild;

	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CTransport();
	virtual ~CTransport();


	// ACCESSORS
	//---------------------------


	// MUTATORS
	//---------------------------



	// METHODS
	//---------------------------
	inline std::vector < CGameAgent* > GetUnitList()
	{
		return mpSpaceUnitsList;
	}

	inline bool AddUnit(CGameAgent* newUnit)
	{
		if (mNumUnitsHeld < mUnitCapacity)
		{
			mNumUnitsHeld++;
			mpSpaceUnitsList.push_back(newUnit);
		}
	}

	inline std::vector < CGameAgent* > ReturnUnitList()
	{
		vector < CGameAgent* > temp = mpSpaceUnitsList;
		for (int i = 0; i < mNumUnitsHeld; i++)
		{
			mpSpaceUnitsList.pop_back();
		}
		return temp;
	}

	// OVERRIDE METHODS
	//---------------------------
	bool Attack(CGameAgent* target, float hitMod, float damageMod);
	void Spawn(CGrid* pGrid, SPointData pCentre);
	//virtual void MoveTo(CTile* dest);
	bool Move();
	void LoadModel(float x, float y, float z);
	void UnloadIModel();
	void HitFlash();
	void UnloadFlash(); 

private:
	bool Destroy();
};

#endif /* _TRANSPORT_H_ */