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

	DX::XMFLOAT4X4 mCamWorldMatrix;
	DX::XMFLOAT4X4 mCamInvViewProj;

	DX::XMFLOAT3 mCamRayOrigin;
	DX::XMFLOAT3 mCamRayEnd;


	// GROUND PLANE
	//--------------------------- 
	DX::XMFLOAT3 mYPlane;


	// MESHES
	//---------------------------
	IMesh* mpMshSkybox;


	// MODELS
	//---------------------------
	IModel* mpMdlSkybox;
	IModel* testModel;

	CGrid* mpEarthGrid;
	CGrid* mpMarsGrid;


	// ADDITIONAL VARIABLES
	//---------------------------
	const int EDGE_THRESHOLD = 50; // How far from the edge the mouse must be for edge scrolling

	SPointData*  mpMouseScreenPos;
	SPointData*  mpMouseGridPos;
	DX::XMFLOAT3 mMouseWorldPos;	// Position of mouse in 3D space at y = 0
	RECT        mBaseClip;		    // The rectangle of the window (for undoing mouse clip)
	RECT        mWindowClip;	    // Limit the mouse to stay within the window

	CSound* mMusic; //Music

	IMesh* test;


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CWorldState();
	virtual ~CWorldState();


	// METHODS
	//---------------------------
	void UpdateMatrices();
	void CalculateMouseGridPos();


	// OVERRIDE METHODS
	//---------------------------
	void StateSetup() override;
	void StateUpdate(const float inDelta) override;
	void StateCleanup() override;
	void StateSave() override;
	void StateLoad() override;
};


#endif /* _WORLD_STATE_H_ */
