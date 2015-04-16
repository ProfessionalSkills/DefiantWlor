//-----------------------------------------------------
// FILE: SpaceState.h
//-----------------------------------------------------

#ifndef _SPACE_STATE_H_
#define _SPACE_STATE_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "BaseGameState.h"
#include "AdvancedButton.h"


//-----------------------------------------------------
// SPACE STATE CLASS
//-----------------------------------------------------
class CSpaceState : public CGameState
{
private:
	// CAMERAS
	//--------------------------- 
	ICamera* mpCamMain;
	//used to adjust the camera to accomadate for larger fleet sizes
	const float mBaseCamZ;
	const float mCamZAdjust;
	float mCamZ;
	void ChangeCameraPosition();
	int mCamState;
	float mCamZMovement;
	const int mNumCamStates;
	const float mCameraMoveSpeed;

	// MESHES
	//--------------------------- 
	IMesh* mpMshSkybox;
	IMesh* mpMshPlanet;
	IMesh* mpMshAtmosphere;


	// MODELS
	//--------------------------- 
	IModel* mpMdlSkybox;
	ISprite* mpSprCursor;

	//planets
	IModel* mpMdlEarth;
	IModel* mpMdlMars;
	IModel* mpMdlSun;
	IModel* mpMdlMoon;
	IModel* mpMdlJupiter;
	IModel* mpMdlVenus;
	IModel* mpMdlMercury;
	IModel* mpMdlNeptune;

	//atmosphere
	IModel* mpMdlEarthAtmos;



	// FLEET
	//---------------------------
	CFleet* mpPlayerOneFleet;
	CFleet* mpPlayerTwoFleet;
	const float mDisplacement;//distance from the centre of the screen. used when loading the ship models
	float mSpecialAttackCooldownTimer;
	const float mSpecialAttackCooldownTime;
	// SOUND
	//---------------------------
	CSound* mMusic;

	// PLAYERS
	//---------------------------
	CPlayerManager* mpPlayerManager;
	CRTSPlayer* mpHumanPlayer;
	CRTSPlayer* mpAIPlayer;

	// USER INTERFACE
	//---------------------------
	IFont* mFntDebug;
	std::stringstream strStream;
	bool mTacticChoosen;

	void DrawFontData();

	// BUTTON
	//---------------------------
	std::vector<CAdvancedButton<CSpaceState, void>*> mpButtonList;
	std::vector<CAdvancedButton<CSpaceState, void>*>::iterator miterButtons;
	void ChangeTacNone();
	void ChangeTacRapid();
	void ChangeTacTargated();
	void RemoveButtonsTactics();

	// PLANET POSITIONS
	//---------------------------
	DirectX::XMFLOAT4 mEarthPos;
	DirectX::XMFLOAT4 mMarsPos;
	DirectX::XMFLOAT4 mSunPos;
	DirectX::XMFLOAT4 mMoonPos;
	DirectX::XMFLOAT4 mJupiterPos;
	DirectX::XMFLOAT4 mVenusPos;
	DirectX::XMFLOAT4 mMercuryPos;
	DirectX::XMFLOAT4 mNeptunePos;

	void LoadPlanets();

	// MISC
	//---------------------------
	const float mTimeToUpdate;
	const float mTimeToUpdateEffects;
	const float mCamRotSpeed;
	float mTimeSinceUpdate;
	float mTimeSinceEffectsUpdate = 0.0f;
	CRandomiser mNewRandom;
	bool PlayerOneVictory;
	bool PlayerTwoVictory;
	DX::XMFLOAT2 mMousePos;

public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CSpaceState();
	virtual ~CSpaceState();

	// OVERRIDE METHODS
	//---------------------------
	void StateSetup() override;
	void StateUpdate() override;
	void StateCleanup() override;
};


#endif /* _SPACE_STATE_H_ */
