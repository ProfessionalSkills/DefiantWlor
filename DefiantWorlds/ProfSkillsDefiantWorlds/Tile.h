//-----------------------------------------------------
// FILE: Tile.h
//-----------------------------------------------------

#ifndef _TILE_H_
#define _TILE_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Common.h"
#include "GameObject.h"


//-----------------------------------------------------
// TILE CLASS
//-----------------------------------------------------
class CTile
{
private:
	SPointData   mTileGridPos;
	DX::XMFLOAT3 mTileWorldPos;
	bool         mTileUsed;
	CGameObject* mpObject;				// Pointer to the object taking up the tile space
	CTile*       mpParent;				// Only really used for pathfinding

	IModel* mpMdlTest;
	
public:
	static IMesh* mspMshTile;


	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CTile();
	~CTile();


	// ACCESSORS
	//---------------------------
	inline SPointData GetGridPos()
	{
		return mTileGridPos;
	}

	inline DX::XMFLOAT3 GetWorldPos()
	{
		return mTileWorldPos;
	}

	inline bool IsTileUsed()
	{
		return mTileUsed;
	}

	inline CTile* GetParent()
	{
		return mpParent;
	}


	// MUTATORS
	//---------------------------
	inline void SetGridPos(SPointData& gridPos)
	{
		mTileGridPos = gridPos;
	}

	inline void SetWorldPos(DX::XMFLOAT3& worldPos)
	{
		mTileWorldPos = worldPos;
	}

	inline void SetTileUsage(bool isUsed)
	{
		mTileUsed = isUsed;
	}

	inline void SetParent(CTile* pTile)
	{
		mpParent = pTile;
	}


	// METHODS
	//--------------------------- 
	void CreateTestModel();
};

#endif /* _TILE_H_ */
