//-----------------------------------------------------
// FILE: Button.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Button.h"
#include "MenuState.h"


//-----------------------------------------------------
// BUTTON CLASS CONSTRUCTOR & DESTRUCTOR
//-----------------------------------------------------
CButton::CButton(std::string spriteName, std::string spriteMOName, SPointData pos, SAABoundingBox boundingBox,
	ClickResponseFunc callbackFunc)
{
	// Passed values
	mPosition = pos;
	mBoundingBox = boundingBox;
	mCallback = callbackFunc;

	// Default values
	mMouseIsOver = false;
	mClicked = false;

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
		CMenuState tmp;
		(tmp.*mCallback)();
		mClicked = false;
	}
}
