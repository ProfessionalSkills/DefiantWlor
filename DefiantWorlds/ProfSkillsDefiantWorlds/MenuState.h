//-----------------------------------------------------
// FILE: MenuState.h
//-----------------------------------------------------

#ifndef _MENU_STATE_H_
#define _MENU_STATE_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "BaseGameState.h"
#include "Button.h"


//-----------------------------------------------------
// MENU STATE CLASS
//-----------------------------------------------------
class CMenuState : public IGameState
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
	std::vector<CButton*> mpButtonList;
	std::vector<CButton*>::iterator miterButtons;


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
	CMenuState();
	virtual ~CMenuState();


	// METHODS
	//---------------------------
	static void NewGame();
	static void LoadGame();
	static void ChangeSettings();
	static void Quit();


	// OVERRIDE METHODS
	//---------------------------
	void StateSetup() override;
	void StateUpdate() override;
	void StateCleanup() override;
	void StateSave() override;
	void StateLoad() override;
};


#endif /* _MENU_STATE_H_ */
