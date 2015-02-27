//-----------------------------------------------------
// FILE: WorldState.h
//-----------------------------------------------------

#ifndef _WORLD_STATE_H_
#define _WORLD_STATE_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "BaseGameState.h"
#include "Grid.h"
#include "AdvancedButton.h"


//-----------------------------------------------------
// WORLD STATE CLASS
//-----------------------------------------------------
class CWorldState : public CGameState
{
private:
	// CAMERA DATA
	//--------------------------- 
	ICamera* mpCamEarth;
	ICamera* mpCamMars;
	ICamera* mpCamCurrent;

	DX::XMFLOAT4X4 mCamWorldMatrix;
	DX::XMFLOAT4X4 mCamInvViewProj;


	// GROUND PLANE
	//--------------------------- 
	DX::XMFLOAT3 mYPlane;


	// MESHES
	//---------------------------
	IMesh* mpMshSkybox;
	IMesh* mpMshGridArea;
	IMesh* mpMshGrassArea;


	// MODELS
	//---------------------------
	IModel* mpMdlSkybox;
	IModel* mpMdlEarthGridArea;
	IModel* mpMdlEarthGrassArea;
	IModel* mpMdlMarsGridArea;
	IModel* mpMdlMarsGrassArea;


	// GRIDS
	//---------------------------
	CGrid* mpEarthGrid;
	CGrid* mpMarsGrid;
	CTile* mpNullTile;
	CTile* mpCurTile;


	// USER INTERFACE
	//---------------------------
	IFont* mFntDebug;
	ISprite* mpMainUI;
	std::vector<CAdvancedButton<CWorldState, void>*> mpGenericButtonList;
	std::vector<CAdvancedButton<CWorldState, void>*>::iterator miterGenericButtons;

	std::vector<CAdvancedButton<CWorldState, void, int>*> mpUnitsButtonList;
	std::vector<CAdvancedButton<CWorldState, void, int>*>::iterator miterUnitsButtons;

	SStructureButtons<CWorldState>* mpBarracksButtons;
	SStructureButtons<CWorldState>* mpHellipadButtons;
	SStructureButtons<CWorldState>* mpSpaceCentreButtons;
	SStructureButtons<CWorldState>* mpComCentreButtons;

	CAdvancedButton<CWorldState, void>* mpButtonDelete;

	CAdvancedButton<CWorldState, void>* mpButtonBarracks;
	CAdvancedButton<CWorldState, void>* mpButtonHellipad;
	CAdvancedButton<CWorldState, void>* mpButtonSpaceCentre;


	// MOUSE DATA
	//---------------------------
	SPointData*  mpMouseScreenPos;
	SPointData   mMouseGridPos;
	SPointData	 mMousePrevGridPos;

	DX::XMFLOAT3 mMouseDirection;	// Direction of ray for mouse
	DX::XMFLOAT3 mMouseOrigin;

	DX::XMFLOAT3 mMouseWorldPos;	// Position of mouse in 3D space at y = 0
	DX::XMFLOAT3 mCurGridPos;		// The start position of the grid currently being interacted with
	RECT         mBaseClip;		    // The rectangle of the window (for undoing mouse clip)
	RECT         mWindowClip;	    // Limit the mouse to stay within the window
	bool mMouseClicked;
	
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


	// PLAYERS
	//---------------------------
	CPlayerManager* mpPlayerManager;
	CRTSPlayer* mpHumanPlayer;
	CRTSPlayer* mpAIPlayer;


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

	void DisplaySelectedBuildingInfo();

	EMouseStates UpdateMouseState();


	// BUTTON METHODS
	//---------------------------
	void QueueUnit(int);
	void CreateBarracks();
	void CreateHellipad();
	void CreateSpaceCentre();
	void CreateHouse();
	void DeleteStructure();


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
