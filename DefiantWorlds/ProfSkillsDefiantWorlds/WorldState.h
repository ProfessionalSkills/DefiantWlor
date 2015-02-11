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

	const float CAM_MOVE_SPEED = 80.0f;
	const float CAM_SCROLL_SPEED = 20.0f;
	const float CAM_MAX_HEIGHT = 200.0f;
	const float CAM_MIN_HEIGHT = 30.0f;

	DX::XMFLOAT4X4 mCamWorldMatrix;
	DX::XMFLOAT4X4 mCamInvViewProj;
	


	// GROUND PLANE
	//--------------------------- 
	DX::XMFLOAT3 mYPlane;


	// MESHES
	//---------------------------
	IMesh* mpMshSkybox;
	IMesh* mpMshGridArea;


	// MODELS
	//---------------------------
	IModel* mpMdlSkybox;
	IModel* mpMdlGridArea;


	// GRIDS
	//---------------------------
	CGrid* mpEarthGrid;
	CGrid* mpMarsGrid;
	CTile* mpNullTile;
	CTile* mpCurTile;


	// FONT
	//---------------------------
	IFont* mFntDebug;


	// MOUSE DATA
	//---------------------------
	SPointData*  mpMouseScreenPos;
	SPointData   mMouseGridPos;
	SPointData	 mMousePrevGridPos;
	DX::XMFLOAT3 mMouseWorldPos;	// Position of mouse in 3D space at y = 0
	RECT         mBaseClip;		    // The rectangle of the window (for undoing mouse clip)
	RECT         mWindowClip;	    // Limit the mouse to stay within the window
	
	EMouseStates mMouseState;		// Stores whether mouse is within a grid or not - and which grid it is in


	// SOUND
	//---------------------------
	CSound* mMusic; //Music


	// SELECTION DATA
	//---------------------------
	CStructure* mpPlacingStructure;
	CStructure* mpCurSelectedStructure;
	CGameAgent* mpCurSelectedAgent;
	//CResource* mpCurSelectedResource;


	// ADDITIONAL VARIABLES
	//---------------------------
	const int EDGE_THRESHOLD = 50; // How far from the edge the mouse must be for edge scrolling
	std::stringstream strStream;


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CWorldState();
	virtual ~CWorldState();


	// METHODS
	//---------------------------
	void UpdateMatrices();
	void UpdateHeldStructure();
	void CalculateMouseGridPos();
	void DrawFontData();
	void CheckKeyPresses();

	EMouseStates UpdateMouseState();


	// OVERRIDE METHODS
	//---------------------------
	void StateSetup() override;
	void StateUpdate() override;
	void StateCleanup() override;
	void StateSave() override;
	void StateLoad() override;

private:
	// EVENT HANDLERS
	//---------------------------
	void OnPlacingStructureChange(CStructure* selStructure);
};


#endif /* _WORLD_STATE_H_ */
