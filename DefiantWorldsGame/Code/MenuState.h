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
#include "SliderTool.h"


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

	std::vector<CAdvancedButton<CMenuState, void, int>*> mpAIDButtonList;					// List of buttons for AI difficulty
	std::vector<CAdvancedButton<CMenuState, void, int>*> mpStartingResButtonList;			// List of buttons for Starting Resources
	std::vector<CAdvancedButton<CMenuState, void, int>*> mpDepositsButtonList;				// List of buttons for number of starting Mineral Deposits
	std::vector<CAdvancedButton<CMenuState, void, int>*>::iterator miterIntButtons;

	CTypeBox* mpTypeBox = nullptr;

	CSliderTool* mpMusicSlider = nullptr;
	CSliderTool* mpEffectsSlider = nullptr;


	// FONTS
	//--------------------------- 
	IFont* mpButtonFont = nullptr;
	IFont* mpIncDecFont = nullptr;
	IFont* mpTitleFont = nullptr;


	// TEMPORARY SETTINGS VARIABLES
	//--------------------------- 
	int mCurAIDifficulty = 0;			// Current index for AI difficulty
	int mCurStartingResources = 0;		// Current index for Starting Resources
	int mCurMineralDeposits = 0;		// Current index for number of Mineral Deposits which are spawned
	std::stringstream mStrStream;


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

	void SetAIDifficulty(int difficulty);
	void SetStartingResources(int amount);
	void SetMineralDeposits(int amount);
	void OnChooseLoadGame();

	void SaveSettings();
	void IncrementMusic();
	void DecrementMusic();
	void IncrementEffects();
	void DecrementEffects();

	void OnChooseCancel();


	// OVERRIDE METHODS
	//---------------------------
	void StateSetup() override;
	void StateUpdate() override;
	void StateCleanup() override;
};


#endif /* _MENU_STATE_H_ */
