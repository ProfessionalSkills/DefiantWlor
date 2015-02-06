//-----------------------------------------------------
// FILE: WorldState.h
//-----------------------------------------------------

#ifndef _WORLD_STATE_H_
#define _WORLD_STATE_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "PlayState.h"


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

	const float CAM_MOVE_SPEED = 50.0f;


	// MESHES
	//---------------------------
	IMesh* mpMshSkybox;


	// MODELS
	//---------------------------
	IModel* mpMdlSkybox;


	// ADDITIONAL VARIABLES
	//---------------------------
	const int EDGE_THRESHOLD = 50; // How far from the edge the mouse must be for edge scrolling

	SPointData* mpMousePos;
	RECT        mBaseClip;		   // The rectangle of the window (for undoing mouse clip)
	RECT        mWindowClip;	   // Limit the mouse to stay within the window


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
