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
	IFont* mpButtonFont = nullptr;
	IFont* mpTitleFont = nullptr;
	std::stringstream strStream;
	bool mTacticChoosen;
	bool leftClicked;
	void DrawFontData();

	// BUTTON
	//---------------------------
	std::vector<CAdvancedButton<CSpaceState, void>*> mpButtonListAll;
	std::vector<CAdvancedButton<CSpaceState, void>*> mpButtonListTactics;
	std::vector<CAdvancedButton<CSpaceState, void>*> mpButtonListPause;
	std::vector<CAdvancedButton<CSpaceState, void>*> mpButtonListVictory;
	std::vector<CAdvancedButton<CSpaceState, void>*> mpButtonListDefeat;
	std::vector<CAdvancedButton<CSpaceState, void>*>::iterator miterButtons;

	//tactic button functions
	void ChangeTacNone();
	void ChangeTacRapid();
	void ChangeTacTargated();

	//other button functions
	void GoToMainMenu();
	void ReturnToEarth();
	void Resume();


	void RemoveButtons();

	inline void UpdateButtons()
	{

		for (miterButtons = mpButtonListAll.begin(); miterButtons != mpButtonListAll.end(); miterButtons++)
		{
			CAdvancedButton<CSpaceState, void>* pButton = (*miterButtons);
			// Check if the mouse is colliding with the object
			if (pButton->GetBoundingBox().IsColliding(DX::XMFLOAT3(mMousePos.x, 0.0f, mMousePos.y)))
			{
				pButton->SetMouseOver(true);
			}
			else
			{
				pButton->SetMouseOver(false);
			}

			// Check for click 
			if (pButton->GetMouseOver())
			{
				// Check if the mouse is over the button
				if (leftClicked)
				{
					// Raise click flag
					pButton->Execute();
					leftClicked = false;
					// Remove self from for loop
					break;
				}
			}

			// Update the button
			pButton->Update();
		}
	}

	inline void HideButtonsTactics()
	{
		for (miterButtons = mpButtonListTactics.begin(); miterButtons != mpButtonListTactics.end(); miterButtons++)
		{
			CAdvancedButton<CSpaceState, void>* pButton = (*miterButtons);
			if (pButton)
			{
				pButton->Hide();
			}
		}
	}

	inline void HideButtonsPaused()
	{
		for (miterButtons = mpButtonListPause.begin(); miterButtons != mpButtonListPause.end(); miterButtons++)
		{
			CAdvancedButton<CSpaceState, void>* pButton = (*miterButtons);
			if (pButton)
			{
				pButton->Hide();
				pButton->Update();
			}
		}
	}

	inline void ShowButtonsPaused()
	{
		for (miterButtons = mpButtonListPause.begin(); miterButtons != mpButtonListPause.end(); miterButtons++)
		{
			CAdvancedButton<CSpaceState, void>* pButton = (*miterButtons);
			if (pButton)
			{
				pButton->Show();
				pButton->Update();
			}
		}
	}

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
	bool mPaused;
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
