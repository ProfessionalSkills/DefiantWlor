//-----------------------------------------------------
// FILE: MenuState.h
//-----------------------------------------------------

#ifndef _SETTINGS_SCREEN_STATE_H_
#define _SETTINGS_SCREEN_STATE_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "BaseGameState.h"
#include "AdvancedButton.h"
#include "SliderTool.h"


//-----------------------------------------------------
// MENU STATE CLASS
//-----------------------------------------------------
class CSettingsScreenState : public CGameState
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
	ISprite* mpSprCursor;
	std::vector<CAdvancedButton<CSettingsScreenState, void>*> mpButtonList;
	std::vector<CAdvancedButton<CSettingsScreenState, void>*>::iterator miterButtons;

	std::vector<CAdvancedButton<CSettingsScreenState, void, int>*> mpAIDButtonList;					// List of buttons for AI difficulty
	std::vector<CAdvancedButton<CSettingsScreenState, void, int>*>::iterator miterAIDButtons;


	// FONTS
	//--------------------------- 
	IFont* mpButtonFont = nullptr;
	IFont* mpIncDecFont = nullptr;
	IFont* mpTitleFont = nullptr;


	// OTHER UI ITEMS
	//--------------------------- 
	CSliderTool* mpMusicSlider = nullptr;
	CSliderTool* mpEffectsSlider = nullptr;


	// TEMPORARY SETTINGS VARIABLES
	//--------------------------- 
	int mCurAIDifficulty = 0;			// Current index for AI difficulty
	std::stringstream mStrStream;


	// ADDITIONAL VARIABLES
	//--------------------------- 
	DX::XMFLOAT2 mMousePos;			// Position of mouse on screen

	DX::XMFLOAT3 mOrbitCentre;		// The position in which the planets orbit
	float mEarthDistance;			// Distance of Earth from centre point
	float mMarsDistance;			// Distance of Mars from centre point
	float mEarthAngle;				// Angle to calculate position on orbital circle
	float mMarsAngle;				// Angle to calculate position on orbital circle

	float mMaxAngle;				// Maximum angle a planet can have before wrapping
	float mMinAngle;				// Angle at which planet wraps to

	CSound* mMusic;


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CSettingsScreenState();
	virtual ~CSettingsScreenState();


	// METHODS
	//---------------------------
	void SaveSettings();
	void Cancel();
	void IncrementMusic();
	void DecrementMusic();
	void IncrementEffects();
	void DecrementEffects();
	void SetAIDifficulty(int difficulty);


	// OVERRIDE METHODS
	//---------------------------
	void StateSetup() override;
	void StateUpdate() override;
	void StateCleanup() override;
	void StateSave() override;
	void StateLoad() override;
};


#endif /* _SETTINGS_SCREEN_STATE_H_ */