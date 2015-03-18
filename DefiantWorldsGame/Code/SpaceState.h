//-----------------------------------------------------
// FILE: SpaceState.h
//-----------------------------------------------------

#ifndef _SPACE_STATE_H_
#define _SPACE_STATE_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "BaseGameState.h"


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

	// MESHES
	//--------------------------- 
	IMesh* mpMshSkybox;

	// MODELS
	//--------------------------- 
	IModel* mpMdlSkybox;

	// FLEET
	//---------------------------
	CFleet* mpPlayerOneFleet;
	CFleet* mpPlayerTwoFleet;
	const float mDisplacement;//distance from the centre of the screen. used when loading the ship models

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

	void DrawFontData();

	// MISC
	//---------------------------
	const float mTimeToUpdate;
	const float mTimeToUpdateEffects;
	const float mCamRotSpeed;
	float mTimeSinceUpdate;
	float mTimeSinceEffectsUpdate = 0.0f;
	CRandomiser mNewRandom;

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
	void StateSave() override;
	void StateLoad() override;
};


#endif /* _SPACE_STATE_H_ */
