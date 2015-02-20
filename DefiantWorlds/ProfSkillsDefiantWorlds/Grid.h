//-----------------------------------------------------
// FILE: Grid.h
//-----------------------------------------------------

#ifndef _GRID_H_
#define _GRID_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Tile.h"


//-----------------------------------------------------
// GRID CLASS
//-----------------------------------------------------
class CGrid
{
private:
	DX::XMFLOAT3 mGridStartPos;							// Where grid 0, 0 is in relation to world 0, 0, 0
	DX::XMFLOAT3 mGridEndPos;
	DX::XMFLOAT3 mGridCentrePos;

	CTile* mpGridArea[GRID_SIZE_X][GRID_SIZE_Y];


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CGrid(DX::XMFLOAT3 gridStartPos);
	~CGrid();


	// ACCESSORS
	//---------------------------
	inline DX::XMFLOAT3 GetGridStartPos()
	{
		return mGridStartPos;
	}

	inline DX::XMFLOAT3 GetGridEndPos()
	{
		return mGridEndPos;
	}

	inline DX::XMFLOAT3 GetGridCentrePos()
	{
		return mGridCentrePos;
	}

	CTile* GetTileData(SPointData gridPos);
	CTile* GetTileToLeft(CTile* pTile);
	CTile* GetTileToRight(CTile* pTile);
	CTile* GetTileAbove(CTile* pTile);
	CTile* GetTileBelow(CTile* pTile);


	// METHODS
	//---------------------------
	void ResetTilesModels();
	bool TurnOnTiles(CTile* gridPos, SPointData pointBL, SPointData pointTR, SPointData pointSpawn);
};

#endif /* _GRID_H_ */
