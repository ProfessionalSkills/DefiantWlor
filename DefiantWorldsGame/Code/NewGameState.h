//-----------------------------------------------------
// FILE: NewGameState.h
//-----------------------------------------------------

#ifndef _NEW_GAME_STATE_H_
#define _NEW_GAME_STATE_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "BaseGameState.h"
#include "AdvancedButton.h"


//-----------------------------------------------------
// MENU STATE CLASS
//-----------------------------------------------------
class CNewGameState : public CGameState
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
	std::vector<CAdvancedButton<CNewGameState, void>*> mpButtonList;
	std::vector<CAdvancedButton<CNewGameState, void>*>::iterator miterButtons;

	std::vector<CAdvancedButton<CNewGameState, void, int>*> mpAIDButtonList;					// List of buttons for AI difficulty
	std::vector<CAdvancedButton<CNewGameState, void, int>*> mpStartingResButtonList;			// List of buttons for Starting Resources
	std::vector<CAdvancedButton<CNewGameState, void, int>*>::iterator miterIntButtons;


	// FONTS
	//--------------------------- 
	IFont* mpButtonFont = nullptr;
	IFont* mpIncDecFont = nullptr;
	IFont* mpTitleFont = nullptr;


	// TEMPORARY SETTINGS VARIABLES
	//--------------------------- 
	int mCurAIDifficulty = 0;			// Current index for AI difficulty
	int mCurStartingResources = 0;		// Current index for Starting Resources
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


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CNewGameState();
	virtual ~CNewGameState();


	// METHODS
	//---------------------------
	void StartNewGame();
	void Cancel();
	void SetAIDifficulty(int difficulty);
	void SetStartingResources(int amount);


	// OVERRIDE METHODS
	//---------------------------
	void StateSetup() override;
	void StateUpdate() override;
	void StateCleanup() override;
	void StateSave() override;
	void StateLoad() override;
};


#endif /* _NEW_GAME_STATE_H_ */