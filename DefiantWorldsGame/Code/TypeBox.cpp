//-----------------------------------------------------
// FILE: TypeBox.cpp
//-----------------------------------------------------

#include "TypeBox.h"


//-----------------------------------------------------
// TYPE BOX CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CTypeBox::CTypeBox(SPointData position)
{
	// Set the slider's properties
	mPosition = position;

	// Load the sprites & Fonts
	mpSprBox = gpEngine->CreateSprite("SliderBack.png", (float)position.mPosX, (float)position.mPosY, 0.8f);
	mpFntText = gpEngine->LoadFont("font2.bmp", 25U);

	// Define the bounding box around the slider
	mBoundingBox = SAABoundingBox(position.mPosY + mBoxDimensions.y, position.mPosX + mBoxDimensions.x, position.mPosY, position.mPosX);
}

CTypeBox::~CTypeBox()
{
	// Remove the sprites
	if (mpSprBox)
	{
		gpEngine->RemoveSprite(mpSprBox);
	}
	if (mpFntText)
	{
		gpEngine->RemoveFont(mpFntText);
	}
}


//-----------------------------------------------------
// SLIDER TOOL CLASS METHODS
//-----------------------------------------------------
void CTypeBox::Update()
{
	// Draw the current text
	mpFntText->Draw(mTypeStream.str(), mPosition.mPosX, mPosition.mPosY, kWhite, kLeft, kVCentre);

	// If it is not selected, do not do the rest
	if (!mSelected) return;

	// In the update area, all this is doing is looking for key presses to add to the current text
	// Use the delete button to clear the contents
	if (gpEngine->KeyHit(Key_Delete))
	{
		mTypeStream.str("");
		mCurLength = 0;
	}

	// Check for shift being held
	bool shifted = false;
	if (gpEngine->KeyHeld(Key_Shift))
	{
		shifted = true;
	}

	// Check for removing last character
	if (gpEngine->KeyHit(Key_Back))
	{
		// Convert to a string
		if (mCurLength > 0)
		{
			std::string tmp = mTypeStream.str();
			tmp.erase(tmp.end() - 1);
			mTypeStream.str("");
			mTypeStream << tmp;

			// Decrement number of characters
			mCurLength--;
		}
	}

	// LETTERS
	if (gpEngine->KeyHit(Key_A))
	{
		// If shift is being held, capitalise letter
		if (shifted)
		{
			mTypeStream << "A";
		}
		else
		{
			mTypeStream << "a";
		}

		// Increment number of characers
		mCurLength++;
	}
}

