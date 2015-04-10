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
	int mNumUnitsHeld = 0;
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
			mCargoValue += newUnit->GetPopValue();
			return true;
		}
		return false;
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

	inline void UnloadUnits()
	{
		for (int i = 0; i < mpSpaceUnitsList.size(); i++)
		{
			mpSpaceUnitsList[i]->SetState(OBJ_BUILT);
			DX::XMFLOAT3 Pos = { -2100.0f, 0.0f, 200.0f };
			mpSpaceUnitsList[i]->SetWorldPos(Pos);
		}
		mpSpaceUnitsList.clear();
	}

	// OVERRIDE METHODS
	//---------------------------
	bool Attack(CGameObject* target, float hitMod, float damageMod);
	void Spawn(CGrid* pGrid, SPointData pCentre);
	//virtual void MoveTo(CTile* dest);
	void LoadModel(float x, float y, float z);
	void UnloadIModel();
	void HitFlash();
	void UnloadFlash();
	float GetUnitSpacing();
	void MoveY(float yChange);
	bool StoreUnits(CGameAgent* unit);

	// Save the data for this unit
	void SaveAgent(std::ofstream& outFile) override;
	void LoadAgent(std::ifstream& inFile) override;

private:
	bool Destroy();
};

#endif /* _TRANSPORT_H_ */