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
	// CAMERA DATA
	//--------------------------- 
	ICamera* mpCamEarth;
	ICamera* mpCamMars;
	ICamera* mpCamCurrent;

	const float CAM_MOVE_SPEED = 50.0f;

	DX::XMFLOAT4X4 mCamWorldMatrix;
	DX::XMFLOAT4X4 mCamInvViewProj;


	// GROUND PLANE
	//--------------------------- 
	DX::XMFLOAT3 mYPlane;


	// MESHES
	//---------------------------
	IMesh* mpMshSkybox;


	// MODELS
	//---------------------------
	IModel* mpMdlSkybox;


	// GRIDS
	//---------------------------
	CGrid* mpEarthGrid;
	CGrid* mpMarsGrid;


	// FONT
	//---------------------------
	IFont* mFntDebug;


	// MOUSE DATA
	//---------------------------
	SPointData*  mpMouseScreenPos;
	SPointData*  mpMouseGridPos;
	DX::XMFLOAT3 mMouseWorldPos;	// Position of mouse in 3D space at y = 0
	RECT         mBaseClip;		    // The rectangle of the window (for undoing mouse clip)
	RECT         mWindowClip;	    // Limit the mouse to stay within the window
	
	EMouseStates mMouseState;		// Stores whether mouse is within a grid or not - and which grid it is in


	// SOUND
	//---------------------------
	CSound* mMusic; //Music


	// ADDITIONAL VARIABLES
	//---------------------------
	const int EDGE_THRESHOLD = 50; // How far from the edge the mouse must be for edge scrolling
	std::stringstream strStream;

	IMesh* test;

	IMesh* buildTest;
	IModel* mdlBuildTest;


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CWorldState();
	virtual ~CWorldState();


	// METHODS
	//---------------------------
	void UpdateMatrices();
	void CalculateMouseGridPos();
	void DrawFontData();

	EMouseStates UpdateMouseState();


	// OVERRIDE METHODS
	//---------------------------
	void StateSetup() override;
	void StateUpdate(const float inDelta) override;
	void StateCleanup() override;
	void StateSave() override;
	void StateLoad() override;
};


#endif /* _WORLD_STATE_H_ */
