//-----------------------------------------------------
// FILE: WorldState.h
//-----------------------------------------------------

#ifndef _WORLD_STATE_H_
#define _WORLD_STATE_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "PlayState.h"
#include "Grid.h"


//-----------------------------------------------------
// WORLD STATE CLASS
//-----------------------------------------------------
class CWorldState : public CPlayState
{
private:
	// CAMERAS
	//--------------------------- 
	ICamera* mpCamEarth;
	ICamera* mpCamMars;
	ICamera* mpCamCurrent;

	const float CAM_MOVE_SPEED = 50.0f;


	// MESHES
	//---------------------------
	IMesh* mpMshSkybox;


	// MODELS
	//---------------------------
	IModel* mpMdlSkybox;


	CGrid* mpEarthGrid;
	CGrid* mpMarsGrid;


	// ADDITIONAL VARIABLES
	//---------------------------
	const int EDGE_THRESHOLD = 50; // How far from the edge the mouse must be for edge scrolling

	SPointData* mpMouseScreenPos;
	SPointData* mpMouseGridPos;
	RECT        mBaseClip;		   // The rectangle of the window (for undoing mouse clip)
	RECT        mWindowClip;	   // Limit the mouse to stay within the window

	IMesh* test;


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CWorldState();
	virtual ~CWorldState();


	// OVERRIDE METHODS
	//---------------------------
	void StateSetup() override;
	void StateUpdate(const float inDelta) override;
	void StateCleanup() override;
	void StateSave() override;
	void StateLoad() override;
};


#endif /* _WORLD_STATE_H_ */
