//-----------------------------------------------------
// FILE: WorldState.h
//-----------------------------------------------------

#ifndef _WORLD_STATE_H_
#define _WORLD_STATE_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include <time.h>

#include "BaseGameState.h"
#include "Grid.h"
#include "AdvancedButton.h"
#include "NewsTicker.h"
#include "SphericalCamera.h"
#include "TypeBox.h"


//-----------------------------------------------------
// WORLD STATE CLASS
//-----------------------------------------------------
class CWorldState : public CGameState
{
private:
	// CAMERA DATA
	//--------------------------- 
	CSphericalCamera* mpCamEarth;
	CSphericalCamera* mpCamMars;
	CSphericalCamera* mpCamCurrent;

	DX::XMFLOAT3 mMinEarthPos;
	DX::XMFLOAT3 mMaxEarthPos;

	DX::XMFLOAT3 mMinMarsPos;
	DX::XMFLOAT3 mMaxMarsPos;

	DX::XMFLOAT3 mCurCamPrevPos;

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
	IMesh* mpTreeMeshes[2];
	IMesh* mpRockMeshes[11];


	// MODELS
	//---------------------------
	IModel* mpMdlSkybox;
	IModel* mpMdlEarthGridArea;
	IModel* mpMdlEarthGrassArea;
	IModel* mpMdlMarsGridArea;
	IModel* mpMdlMarsGrassArea;
	std::vector<IModel*> mpMdlTreeList;
	std::vector<IModel*> mpMdlRockList;


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
	ISprite* mpSprQProg;
	ISprite* mpSprHealth;
	ISprite* mpSprCursor;

	std::vector<CAdvancedButton<CWorldState, void>*> mpGenericButtonList;
	std::vector<CAdvancedButton<CWorldState, void>*>::iterator miterGenericButtons;

	std::vector<CAdvancedButton<CWorldState, void, int>*> mpUnitsButtonList;
	std::vector<CAdvancedButton<CWorldState, void, int>*>::iterator miterUnitsButtons;

	SStructureButtons<CWorldState>* mpBarracksButtons;
	SStructureButtons<CWorldState>* mpHellipadButtons;
	SStructureButtons<CWorldState>* mpSpaceCentreButtons;
	SStructureButtons<CWorldState>* mpComCentreButtons;
	SStructureButtons<CWorldState>* mpQueueButtons;

	CAdvancedButton<CWorldState, void>* mpSpaceAtaackButtons;
	CAdvancedButton<CWorldState, void>* mpSpaceTacTargetedButton;
	CAdvancedButton<CWorldState, void>* mpSpaceTacRapidButton;
	CAdvancedButton<CWorldState, void>* mpSpaceTacNoneButton;

	CAdvancedButton<CWorldState, void>* mpButtonDelete;

	CAdvancedButton<CWorldState, void>* mpButtonBarracks;
	CAdvancedButton<CWorldState, void>* mpButtonHellipad;

	CAdvancedButton<CWorldState, void>* mpButtonSpaceCentre;

	CTypeBox* mpTypeBox = nullptr;


	// MOUSE DATA
	//---------------------------
	SPointData mpMousePrevScreenPos;
	SPointData* mpMouseScreenPos;
	SPointData mMouseGridPos;
	SPointData mMousePrevGridPos;

	SBoundingCube* mpDragBox;
	IModel* mpMdlDragBox;

	DX::XMFLOAT3 mMouseDirection;	// Direction of ray for mouse
	DX::XMFLOAT3 mMouseOrigin;

	DX::XMFLOAT3 mMouseWorldPos;		// Position of mouse in 3D space at y = 0
	DX::XMFLOAT3 mCurGridPos;			// The start position of the grid currently being interacted with

	DX::XMFLOAT3 mDragStartPos;
	DX::XMFLOAT3 mDragEndPos;

	RECT mBaseClip;			    // The rectangle of the window (for undoing mouse clip)
	RECT mWindowClip;		    // Limit the mouse to stay within the window
	bool mLMouseClicked;
	bool mRMouseClicked;
	float mHoldCount;
	float mClickCoolDown;
	bool mLMouseHeld;

	bool mPaused = false;

	EMouseStates mMouseState;		// Stores whether mouse is within a grid or not - and which grid it is in


	// SOUND
	//---------------------------
	CSound* mMusic; //Music


	// SELECTION DATA
	//---------------------------
	CStructure* mpPlacingStructure;
	CStructure* mpCurSelectedStructure;
	CGameAgent* mpCurSelectedAgent;
	CMinerals* mpCurSelectedResource;

	std::vector<CGameAgent*> mpUnitSelectionList;
	std::vector<CGameAgent*>::iterator miterUnitSelectionList;


	// PAUSED SCREEN
	//---------------------------
	IFont* mpTitleFont = nullptr;
	IFont* mpButtonFont = nullptr;
	ISprite* mpSprBackground = nullptr;
	std::vector<CAdvancedButton<CWorldState, void>*> mpPauseButtonList;
	std::vector<CAdvancedButton<CWorldState, void>*>::iterator miterPauseButtons;


	// ADDITIONAL VARIABLES
	//---------------------------
	std::stringstream strStream;
	int mQueuePrevSize;
	int mQueuePrevProg;				// Stores the previous progress of the object currently being queued
	int mPrevHealth;


	// PLAYERS
	//---------------------------
	CPlayerManager* mpPlayerManager;
	CRTSPlayer* mpHumanPlayer;
	CRTSAIPlayer* mpAIPlayer;


public:
	// STATIC MESHES
	//---------------------------
	// Drag box
	static IMesh* mspMshDrag;


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
	bool CheckForQueueChange();

	void DisplaySelectedBuildingInfo();
	void DisplaySelectedAgentInfo();

	EMouseStates UpdateMouseState();


	// BUTTON METHODS
	//---------------------------
	void QueueUnit(int index);
	void UnqueueUnit(int index);
	void CreateBarracks();
	void CreateHellipad();
	void CreateSpaceCentre();
	void CreateHouse();
	void DeleteStructure();
	void LaunchAttack();
	void ChangeTacNone();
	void ChangeTacRapid();
	void ChangeTacTargated();

	// Paused menu buttons methods
	void Continue();
	void SaveGame();
	void QuitGame();


	// OVERRIDE METHODS
	//---------------------------
	void StateSetup() override;
	void StateUpdate() override;
	void StateCleanup() override;


private:
	// EVENT HANDLERS
	//---------------------------
	void OnPlacingStructureChange(CStructure* selStructure);
	void OnStructureSelectChange(CStructure* pSelStructure);
	void OnStructureQueueProgressChange();
	void OnItemHealthChange();
	void OnPause();
	void OnUnPause();
	void OnChooseSave();
	void OnChooseCancel();
};


#endif /* _WORLD_STATE_H_ */
