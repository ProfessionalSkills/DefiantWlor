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

	inline void UnloadUnits(bool Victory, int StartX, int StartZ)
	{
		DX::XMFLOAT3 Pos;
		for (int i = 0; i < mpSpaceUnitsList.size(); i++)
		{
			// Store easy access pointer of unit
			CGameAgent* pAgent = mpSpaceUnitsList[i];
			
			// Get the faction of the unit
			if (pAgent->GetFaction() == FAC_EARTH_DEFENSE_FORCE)
			{
				// Check if it was a victory or not
				if (Victory)
				{
					Pos = { 4400.0f-(10*StartX), 0.0f, (100.0f*StartZ)+(7*i) };
				}
				else
				{
					Pos = { -2100.0f - (10 * StartX), 0.0f, (100.0f*StartZ) + (7 * i) };
				}
			}
			else
			{
				// Check if it was a victory or not
				if (Victory)
				{
					Pos = { -2100.0f - (10 * StartX), 0.0f, (100.0f*StartZ) + (7 * i) };
				}
				else
				{
					Pos = { 4400.0f - (10 * StartX), 0.0f, (100.0f*StartZ) + (7 * i) };
				}
			}

			// Check to see if the unit is an air unit
			EGameAgentVariations agentType = pAgent->GetAgentData()->mAgentType;
			if (agentType == GAV_BOMBER || agentType == GAV_FIGHTER)
			{
				// Set the height
				Pos.y = 30.0f;
			}

			// Set the unit to the new position
			pAgent->SetState(OBJ_BUILT);
			pAgent->SetWorldPos(Pos);
			pAgent->UpdateBoundingSphere();
		}

		mpSpaceUnitsList.clear();
	}

	// OVERRIDE METHODS
	//---------------------------
	bool Attack(CGameObject* target, float hitMod, float damageMod);
	void Spawn(CGrid* pGrid, SPointData pCentre);
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