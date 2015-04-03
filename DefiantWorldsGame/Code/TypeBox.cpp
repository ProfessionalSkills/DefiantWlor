//-----------------------------------------------------
// FILE: TypeBox.cpp
//-----------------------------------------------------

#include "TypeBox.h"


//-----------------------------------------------------
// TYPE BOX CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CTypeBox::CTypeBox(SPointData position, DX::XMFLOAT2 boxDimensions, ETransitionTypes transitionType,
	bool active, float transitionTime) : CMovingUI(position, boxDimensions, transitionType, active, transitionTime)
{
	// Load the sprites & Fonts
	mpSprBox = gpEngine->CreateSprite("TypeBoxBack.png", mCurPosition.x, mCurPosition.y, 0.6f);
	mpFntText = gpEngine->LoadFont("font2.bmp", 25U);
	mIsHidden = !active;
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
// TYPE BOX CLASS METHODS
//-----------------------------------------------------
void CTypeBox::Show()
{
	// If already shown, just return
	if (mIsHidden && mpSprBox)
	{
		mIsHidden = false;

		// If not using transitioning, show the buttons
		if (mTransitionType == TR_NONE)
		{
			mpSprBox->SetZ(0.6f);
		}

		// Set to transition in
		mToTransitionIn = true;
		mIsAtDestination = false;
		mIsOffScreen = false;
	}
}

void CTypeBox::Hide()
{
	// If already hidden, just return
	if (!mIsHidden && mpSprBox)
	{
		// Raise hidden flag
		mIsHidden = true;

		// If not using transitioning, hide the buttons
		if (mTransitionType == TR_NONE)
		{
			mpSprBox->SetZ(-1.0f);
		}

		// Set to transition out
		mToTransitionOut = true;
		mIsAtDestination = false;
		mIsOffScreen = false;
	}
}

void CTypeBox::Update()
{
	// If it is transitioning in or out, update the button's position
	if (mToTransitionIn || mToTransitionOut)
	{
		// This is a moving user interface element - call the parent function first to update its position
		UpdateTransition();

		// Calculate bounding box based on provided dimensions and position
		mBoundingBox = { mCurPosition.y + mBoxDimensions.y, mCurPosition.x + mBoxDimensions.x, mCurPosition.y, mCurPosition.x };

		// If sprite exists
		if (mpSprBox)
		{
			mpSprBox->SetX(mCurPosition.x);
			mpSprBox->SetY(mCurPosition.y);
		}
	}
	
	// Draw the current text
	mpFntText->Draw(mTypeStream.str(), mCurPosition.x + 10, mCurPosition.y + 20, kBlack, kLeft, kVCentre);

	// If the element is hidden, do not do the rest of the function
	if (mIsHidden) return;

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


	// Check to see if too many characters have been entered
	if (mCurLength > 40) return;


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

	if (gpEngine->KeyHit(Key_B))
	{
		// If shift is being held, capitalise letter
		if (shifted)
		{
			mTypeStream << "B";
		}
		else
		{
			mTypeStream << "b";
		}

		// Increment number of characers
		mCurLength++;
	}

	if (gpEngine->KeyHit(Key_C))
	{
		// If shift is being held, capitalise letter
		if (shifted)
		{
			mTypeStream << "C";
		}
		else
		{
			mTypeStream << "c";
		}

		// Increment number of characers
		mCurLength++;
	}

	if (gpEngine->KeyHit(Key_D))
	{
		// If shift is being held, capitalise letter
		if (shifted)
		{
			mTypeStream << "D";
		}
		else
		{
			mTypeStream << "d";
		}

		// Increment number of characers
		mCurLength++;
	}

	if (gpEngine->KeyHit(Key_E))
	{
		// If shift is being held, capitalise letter
		if (shifted)
		{
			mTypeStream << "E";
		}
		else
		{
			mTypeStream << "e";
		}

		// Increment number of characers
		mCurLength++;
	}

	if (gpEngine->KeyHit(Key_F))
	{
		// If shift is being held, capitalise letter
		if (shifted)
		{
			mTypeStream << "F";
		}
		else
		{
			mTypeStream << "f";
		}

		// Increment number of characers
		mCurLength++;
	}

	if (gpEngine->KeyHit(Key_G))
	{
		// If shift is being held, capitalise letter
		if (shifted)
		{
			mTypeStream << "G";
		}
		else
		{
			mTypeStream << "g";
		}

		// Increment number of characers
		mCurLength++;
	}

	if (gpEngine->KeyHit(Key_H))
	{
		// If shift is being held, capitalise letter
		if (shifted)
		{
			mTypeStream << "H";
		}
		else
		{
			mTypeStream << "h";
		}

		// Increment number of characers
		mCurLength++;
	}

	if (gpEngine->KeyHit(Key_I))
	{
		// If shift is being held, capitalise letter
		if (shifted)
		{
			mTypeStream << "I";
		}
		else
		{
			mTypeStream << "i";
		}

		// Increment number of characers
		mCurLength++;
	}

	if (gpEngine->KeyHit(Key_J))
	{
		// If shift is being held, capitalise letter
		if (shifted)
		{
			mTypeStream << "J";
		}
		else
		{
			mTypeStream << "j";
		}

		// Increment number of characers
		mCurLength++;
	}

	if (gpEngine->KeyHit(Key_K))
	{
		// If shift is being held, capitalise letter
		if (shifted)
		{
			mTypeStream << "K";
		}
		else
		{
			mTypeStream << "k";
		}

		// Increment number of characers
		mCurLength++;
	}

	if (gpEngine->KeyHit(Key_L))
	{
		// If shift is being held, capitalise letter
		if (shifted)
		{
			mTypeStream << "L";
		}
		else
		{
			mTypeStream << "l";
		}

		// Increment number of characers
		mCurLength++;
	}

	if (gpEngine->KeyHit(Key_M))
	{
		// If shift is being held, capitalise letter
		if (shifted)
		{
			mTypeStream << "M";
		}
		else
		{
			mTypeStream << "m";
		}

		// Increment number of characers
		mCurLength++;
	}

	if (gpEngine->KeyHit(Key_N))
	{
		// If shift is being held, capitalise letter
		if (shifted)
		{
			mTypeStream << "N";
		}
		else
		{
			mTypeStream << "n";
		}

		// Increment number of characers
		mCurLength++;
	}

	if (gpEngine->KeyHit(Key_O))
	{
		// If shift is being held, capitalise letter
		if (shifted)
		{
			mTypeStream << "O";
		}
		else
		{
			mTypeStream << "o";
		}

		// Increment number of characers
		mCurLength++;
	}

	if (gpEngine->KeyHit(Key_P))
	{
		// If shift is being held, capitalise letter
		if (shifted)
		{
			mTypeStream << "P";
		}
		else
		{
			mTypeStream << "p";
		}

		// Increment number of characers
		mCurLength++;
	}

	if (gpEngine->KeyHit(Key_Q))
	{
		// If shift is being held, capitalise letter
		if (shifted)
		{
			mTypeStream << "Q";
		}
		else
		{
			mTypeStream << "q";
		}

		// Increment number of characers
		mCurLength++;
	}

	if (gpEngine->KeyHit(Key_R))
	{
		// If shift is being held, capitalise letter
		if (shifted)
		{
			mTypeStream << "R";
		}
		else
		{
			mTypeStream << "r";
		}

		// Increment number of characers
		mCurLength++;
	}

	if (gpEngine->KeyHit(Key_S))
	{
		// If shift is being held, capitalise letter
		if (shifted)
		{
			mTypeStream << "S";
		}
		else
		{
			mTypeStream << "s";
		}

		// Increment number of characers
		mCurLength++;
	}

	if (gpEngine->KeyHit(Key_T))
	{
		// If shift is being held, capitalise letter
		if (shifted)
		{
			mTypeStream << "T";
		}
		else
		{
			mTypeStream << "t";
		}

		// Increment number of characers
		mCurLength++;
	}

	if (gpEngine->KeyHit(Key_U))
	{
		// If shift is being held, capitalise letter
		if (shifted)
		{
			mTypeStream << "U";
		}
		else
		{
			mTypeStream << "u";
		}

		// Increment number of characers
		mCurLength++;
	}

	if (gpEngine->KeyHit(Key_V))
	{
		// If shift is being held, capitalise letter
		if (shifted)
		{
			mTypeStream << "V";
		}
		else
		{
			mTypeStream << "v";
		}

		// Increment number of characers
		mCurLength++;
	}

	if (gpEngine->KeyHit(Key_W))
	{
		// If shift is being held, capitalise letter
		if (shifted)
		{
			mTypeStream << "W";
		}
		else
		{
			mTypeStream << "w";
		}

		// Increment number of characers
		mCurLength++;
	}

	if (gpEngine->KeyHit(Key_X))
	{
		// If shift is being held, capitalise letter
		if (shifted)
		{
			mTypeStream << "X";
		}
		else
		{
			mTypeStream << "x";
		}

		// Increment number of characers
		mCurLength++;
	}

	if (gpEngine->KeyHit(Key_Y))
	{
		// If shift is being held, capitalise letter
		if (shifted)
		{
			mTypeStream << "Y";
		}
		else
		{
			mTypeStream << "y";
		}

		// Increment number of characers
		mCurLength++;
	}

	if (gpEngine->KeyHit(Key_Z))
	{
		// If shift is being held, capitalise letter
		if (shifted)
		{
			mTypeStream << "Z";
		}
		else
		{
			mTypeStream << "z";
		}

		// Increment number of characers
		mCurLength++;
	}


	// SPACE
	if (gpEngine->KeyHit(Key_Space))
	{
		// If shift is being held, capitalise letter
		mTypeStream << " ";

		// Increment number of characers
		mCurLength++;
	}


	// NUMBERS
	if (gpEngine->KeyHit(Key_0))
	{
		// If shift is being held, capitalise letter
		mTypeStream << 0;

		// Increment number of characers
		mCurLength++;
	}

	if (gpEngine->KeyHit(Key_1))
	{
		// If shift is being held, capitalise letter
		mTypeStream << 1;

		// Increment number of characers
		mCurLength++;
	}

	if (gpEngine->KeyHit(Key_2))
	{
		// If shift is being held, capitalise letter
		mTypeStream << 2;

		// Increment number of characers
		mCurLength++;
	}

	if (gpEngine->KeyHit(Key_3))
	{
		// If shift is being held, capitalise letter
		mTypeStream << 3;

		// Increment number of characers
		mCurLength++;
	}

	if (gpEngine->KeyHit(Key_4))
	{
		// If shift is being held, capitalise letter
		mTypeStream << 4;

		// Increment number of characers
		mCurLength++;
	}

	if (gpEngine->KeyHit(Key_5))
	{
		// If shift is being held, capitalise letter
		mTypeStream << 5;

		// Increment number of characers
		mCurLength++;
	}

	if (gpEngine->KeyHit(Key_6))
	{
		// If shift is being held, capitalise letter
		mTypeStream << 6;

		// Increment number of characers
		mCurLength++;
	}

	if (gpEngine->KeyHit(Key_7))
	{
		// If shift is being held, capitalise letter
		mTypeStream << 7;

		// Increment number of characers
		mCurLength++;
	}

	if (gpEngine->KeyHit(Key_8))
	{
		// If shift is being held, capitalise letter
		mTypeStream << 8;

		// Increment number of characers
		mCurLength++;
	}

	if (gpEngine->KeyHit(Key_9))
	{
		// If shift is being held, capitalise letter
		mTypeStream << 9;

		// Increment number of characers
		mCurLength++;
	}
}

