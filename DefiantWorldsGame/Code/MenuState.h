//-----------------------------------------------------
// FILE: MenuState.h
//-----------------------------------------------------

#ifndef _MENU_STATE_H_
#define _MENU_STATE_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include <fstream>

#include "BaseGameState.h"
#include "AdvancedButton.h"
#include "TypeBox.h"


//-----------------------------------------------------
// MAIN MENU STATE ENUMERATIONS
//-----------------------------------------------------
enum EMenuState
{
	MENU_MAIN, MENU_NEW_GAME, MENU_LOAD, MENU_SETTINGS
};


//-----------------------------------------------------
// MENU STATE CLASS
//-----------------------------------------------------
class CMenuState : public CGameState
{
private:
	// CAMERAS
	//--------------------------- 
	ICamera* mpCamMain;


	// MESHES
	//--------------------------- 
	IMesh* mpMshSkybox;
	IMesh* mpMshPlanet;
	IMesh* mpMshAtmosphere;


	// MODELS
	//--------------------------- 
	IModel* mpMdlSkybox;
	IModel* mpMdlAtmosphere;
	IModel* mpMdlEarth;
	IModel* mpMdlMars;


	// SPRITES & BUTTONS
	//--------------------------- 
	ISprite* mpSprBackground;
	ISprite* mpSprLogo;
	ISprite* mpSprCursor;
	std::vector<CAdvancedButton<CMenuState, void>*> mpButtonList;
	std::vector<CAdvancedButton<CMenuState, void>*>::iterator miterButtons;

	CTypeBox* mpTypeBox = nullptr;


	// FONTS
	//--------------------------- 
	IFont* mpButtonFont = nullptr;
	IFont* mpIncDecFont = nullptr;


	// ADDITIONAL VARIABLES
	//--------------------------- 
	EMenuState mMenuState = MENU_MAIN;		// The current state which the menu is in

	DX::XMFLOAT2 mMousePos;			// Position of mouse on screen

	DX::XMFLOAT3 mOrbitCentre;		// The position in which the planets orbit
	float mEarthDistance;			// Distance of Earth from centre point
	float mMarsDistance;			// Distance of Mars from centre point
	float mEarthAngle;				// Angle to calculate position on orbital circle
	float mMarsAngle;				// Angle to calculate position on orbital circle

	float mMaxAngle;				// Maximum angle a planet can have before wrapping
	float mMinAngle;				// Angle at which planet wraps to


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CMenuState();
	virtual ~CMenuState();


	// METHODS
	//---------------------------
	void NewGame();
	void StartNewGame();
	void LoadGame();
	void ChangeSettings();
	void Quit();

	void OnChooseLoadGame();
	void OnChooseCancel();


	// OVERRIDE METHODS
	//---------------------------
	void StateSetup() override;
	void StateUpdate() override;
	void StateCleanup() override;
};


#endif /* _MENU_STATE_H_ */
