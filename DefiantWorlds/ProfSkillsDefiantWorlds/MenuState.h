//-----------------------------------------------------
// FILE: MenuState.h
//-----------------------------------------------------

#ifndef _MENU_STATE_H_
#define _MENU_STATE_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "BaseGameState.h"


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


	// MODELS
	//--------------------------- 
	IModel* mpMdlSkybox;
	IModel* mpMdlEarth;


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CMenuState();
	virtual ~CMenuState();


	// OVERRIDE METHODS
	//---------------------------
	void StateSetup() override;
	void StateUpdate(const float inDelta) override;
	void StateCleanup() override;
	void StateSave() override;
	void StateLoad() override;
};


#endif /* _MENU_STATE_H_ */
