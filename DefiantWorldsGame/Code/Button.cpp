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
	std::string purpose)
{
	// Passed values
	mPosition = pos;
	mBoundingBox = boundingBox;
	mPurpose = purpose;

	// Default values
	mMouseIsOver = false;
	mIsHidden = false;

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
	if (mMouseIsOver && !mIsHidden)
	{
		// Set mouse over sprite to front & basic to back
		mpSprBasic->SetZ(-1.0f);
		mpSprMO->SetZ(0.0f);
	}
	else if (!mMouseIsOver && !mIsHidden)
	{
		mpSprBasic->SetZ(0.0f);
		mpSprMO->SetZ(-1.0f);
	}
}

void CButton::Show()
{
	// If already shown, just return
	if (mIsHidden)
	{
		mIsHidden = false;
		mpSprBasic->SetZ(0.0f);
		mpSprMO->SetZ(-1.0f);
	}
}

void CButton::Hide()
{
	// If already hidden, just return
	if (!mIsHidden)
	{
		mIsHidden = true;
		mpSprBasic->SetZ(-1.0f);
		mpSprMO->SetZ(-1.0f);
	}
}
