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
	CTile*       mpGridArea[GRID_SIZE_X][GRID_SIZE_Y];


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CGrid(DX::XMFLOAT3 gridStartPos, IMesh* msh);
	~CGrid();


	// METHODS
	//--------------------------- 
	CTile* GetTileData(SPointData gridPos);
};

#endif /* _GRID_H_ */
