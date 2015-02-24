//-----------------------------------------------------
// FILE: Button.h
//-----------------------------------------------------
#ifndef _BUTTON_H_
#define _BUTTON_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Common.h"


//-----------------------------------------------------
// BUTTON CLASS
//-----------------------------------------------------
class CMenuState;
class CWorldState;
class CSpaceState;

class CButton
{
private:
	// BUTTON PROPERTIES
	//---------------------------
	SPointData mPosition;
	SAABoundingBox mBoundingBox;

	bool mMouseIsOver;
	bool mClicked;
	int  mState;		// 0 = menu, 1 = World, 2 = Space

	typedef void (CMenuState::*ClickResponseMenu)(void);
	typedef void (CWorldState::*ClickResponseWorld)(void);
	typedef void (CSpaceState::*ClickResponseSpace)(void);
	ClickResponseMenu mMenuCallback;
	ClickResponseWorld mWorldCallback;
	ClickResponseSpace mSpaceCallback;


	// BUTTON VISUALS
	//---------------------------
	ISprite* mpSprBasic;			// basic sprite
	ISprite* mpSprMO;				// mouse over sprite
	

public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CButton(std::string spriteName, std::string spriteMOName, SPointData pos, SAABoundingBox boundingBox,
		ClickResponseMenu callbackFunc);
	CButton(std::string spriteName, std::string spriteMOName, SPointData pos, SAABoundingBox boundingBox,
		ClickResponseWorld callbackFunc);
	CButton(std::string spriteName, std::string spriteMOName, SPointData pos, SAABoundingBox boundingBox,
		ClickResponseSpace callbackFunc);
	~CButton();


	// ACCESSORS
	//---------------------------
	inline SAABoundingBox GetBoundingBox()
	{
		return mBoundingBox;
	}

	inline bool GetMouseOver()
	{
		return mMouseIsOver;
	}


	// MUTATORS
	//---------------------------
	inline void SetMouseOver(bool mouseOver)
	{
		mMouseIsOver = mouseOver;
	}

	inline void SetClick(bool isClicked)
	{
		mClicked = isClicked;
	}


	// METHODS
	//---------------------------
	void Update();
};


#endif /* _BUTTON_H_ */
