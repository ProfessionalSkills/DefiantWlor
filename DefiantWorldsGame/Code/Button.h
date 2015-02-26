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
	bool mIsHidden;

	std::string mPurpose;


	// BUTTON VISUALS
	//---------------------------
	ISprite* mpSprBasic;			// basic sprite
	ISprite* mpSprMO;				// mouse over sprite
	

public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CButton(std::string spriteName, std::string spriteMOName, SPointData pos, SAABoundingBox boundingBox,
		std::string purpose);
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

	inline std::string* GetPurpose()
	{
		return &mPurpose;
	}

	inline bool IsHidden()
	{
		return mIsHidden;
	}


	// MUTATORS
	//---------------------------
	void SetMouseOver(bool mouseOver);


	// METHODS
	//---------------------------
	void Update();
	void Show();
	void Hide();
};


struct SStructureButtons
{
	CButton* mpButtons[MAX_STRUCTURE_BUTTONS];
	int mNumButtons;

	SStructureButtons(int num)
	{
		// Limit maximum number of buttons
		if (num > MAX_STRUCTURE_BUTTONS)
		{
			num = MAX_STRUCTURE_BUTTONS;
		}
		mNumButtons = num;

		// Initialise buttons
		for (int i = 0; i < mNumButtons; i++)
		{
			mpButtons[i] = nullptr;
		}
	}

	void Hide()
	{
		for (int i = 0; i < mNumButtons; i++)
		{
			if (mpButtons[i])
			{
				mpButtons[i]->Hide();
			}
		}
	}

	void Show()
	{
		for (int i = 0; i < mNumButtons; i++)
		{
			if (mpButtons[i])
			{
				mpButtons[i]->Show();
			}
		}
	}

	void Update()
	{
		for (int i = 0; i < mNumButtons; i++)
		{
			if (mpButtons[i])
			{
				mpButtons[i]->Update();
			}
		}
	}
};


#endif /* _BUTTON_H_ */
