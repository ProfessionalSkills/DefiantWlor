//-----------------------------------------------------
// FILE: SpaceState.h
//-----------------------------------------------------

#ifndef _SPACE_STATE_H_
#define _SPACE_STATE_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "PlayState.h"


//-----------------------------------------------------
// SPACE STATE CLASS
//-----------------------------------------------------
class CSpaceState : public CPlayState
{
private:
	// CAMERAS
	//--------------------------- 
	ICamera* mpCamMain;


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
	float mDisplacement;//distance from the centre of the screen. used when loading the ship models

	// SOUND
	//---------------------------
	CSound* mMusic;


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
