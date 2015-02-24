//-----------------------------------------------------
// FILE: Button.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Button.h"
#include "MenuState.h"
#include "WorldState.h"
#include "SpaceState.h"


//-----------------------------------------------------
// BUTTON CLASS CONSTRUCTOR & DESTRUCTOR
//-----------------------------------------------------
CButton::CButton(std::string spriteName, std::string spriteMOName, SPointData pos, SAABoundingBox boundingBox,
	ClickResponseMenu callbackFunc)
{
	// Passed values
	mPosition = pos;
	mBoundingBox = boundingBox;
	mMenuCallback = callbackFunc;

	// Default values
	mMouseIsOver = false;
	mClicked = false;
	mState = 0;

	// Create sprite
	mpSprBasic = gpEngine->CreateSprite(spriteName, pos.mPosX, pos.mPosY, 0.0f);
	mpSprMO = gpEngine->CreateSprite(spriteMOName, pos.mPosX, pos.mPosY, -1.0f);
}

CButton::CButton (std::string spriteName, std::string spriteMOName, SPointData pos, SAABoundingBox boundingBox,
	ClickResponseWorld callbackFunc)
{
	// Passed values
	mPosition = pos;
	mBoundingBox = boundingBox;
	mWorldCallback = callbackFunc;

	// Default values
	mMouseIsOver = false;
	mClicked = false;
	mState = 1;

	// Create sprite
	mpSprBasic = gpEngine->CreateSprite(spriteName, pos.mPosX, pos.mPosY, 0.0f);
	mpSprMO = gpEngine->CreateSprite(spriteMOName, pos.mPosX, pos.mPosY, -1.0f);
}

CButton::CButton(std::string spriteName, std::string spriteMOName, SPointData pos, SAABoundingBox boundingBox,
	ClickResponseSpace callbackFunc)
{
	// Passed values
	mPosition = pos;
	mBoundingBox = boundingBox;
	mSpaceCallback = callbackFunc;

	// Default values
	mMouseIsOver = false;
	mClicked = false;
	mState = 2;

	// Create sprite
	mpSprBasic = gpEngine->CreateSprite(spriteName, pos.mPosX, pos.mPosY, 0.0f);
	mpSprMO = gpEngine->CreateSprite(spriteMOName, pos.mPosX, pos.mPosY, -1.0f);
}

CButton::~CButton()
{
	gpEngine->RemoveSprite(mpSprBasic);
	gpEngine->RemoveSprite(mpSprMO);
}


//-----------------------------------------------------
// BUTTON CLASS METHODS
//-----------------------------------------------------
void CButton::Update()
{
	// Check if the button is hovered over
	if (mMouseIsOver)
	{
		// Set mouse over sprite to front & basic to back
		mpSprBasic->SetZ(-1.0f);
		mpSprMO->SetZ(0.0f);
	}
	else
	{
		mpSprBasic->SetZ(0.0f);
		mpSprMO->SetZ(-1.0f);
	}

	// Check for click
	if (mClicked)
	{
		// Call the response function
		if (mState == 0)
		{
			CMenuState tmp;
			(tmp.*mMenuCallback)();
			mClicked = false;
		}
		else if (mState == 1)
		{
			CWorldState tmp;
			(tmp.*mWorldCallback)();
			mClicked = false;
		}
		else if (mState == 2)
		{
			CSpaceState tmp;
			(tmp.*mSpaceCallback)();
			mClicked = false;
		}
	}
}
