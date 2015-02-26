//-----------------------------------------------------
// FILE: Button.h
//-----------------------------------------------------
#ifndef _ADVANCED_BUTTON_H_
#define _ADVANCED_BUTTON_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Common.h"


//-----------------------------------------------------
// ADVANCED BUTTON CLASS
//-----------------------------------------------------
template <class C, class CRt, class... CArgs>	// C = class type for button
												// Rt = return type
												// Args = parameters for pointer function
class CAdvancedButton
{
public:
	// CONSTRUCTOR & DESTRUCTOR
	//---------------------------
	typedef CRt(C::*ButtonMethod)(CArgs...);
	
	CAdvancedButton(std::string defaultTex, std::string selectedTex, SPointData pos,
		SAABoundingBox boundingBox, C& targetClass, ButtonMethod targetMethod) :
		mPosition(pos), mBoundingBox(boundingBox), mTargetClass(targetClass), mTargetMethod(targetMethod),
		mIsHidden(false), mMouseIsOver(false)
	{
		mpSprBasic = gpEngine->CreateSprite(defaultTex, (float)pos.mPosX, (float)pos.mPosY, 0.0f);
		mpSprMO = gpEngine->CreateSprite(selectedTex, (float)pos.mPosX, (float)pos.mPosY, -1.0f);
	}

	~CAdvancedButton()
	{
		gpEngine->RemoveSprite(mpSprBasic);
		gpEngine->RemoveSprite(mpSprMO);
	}


	// METHODS
	//---------------------------
	void Execute(CArgs&... parameters)
	{
		(mTargetClass.*(mTargetMethod))(parameters...);
	}

	void Update()
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

	void Show()
	{
		// If already shown, just return
		if (mIsHidden)
		{
			mIsHidden = false;
			mpSprBasic->SetZ(0.0f);
			mpSprMO->SetZ(-1.0f);
		}
	}

	void Hide()
	{
		// If already hidden, just return
		if (!mIsHidden)
		{
			mIsHidden = true;
			mpSprBasic->SetZ(-1.0f);
			mpSprMO->SetZ(-1.0f);
		}
	}


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

	inline bool IsHidden()
	{
		return mIsHidden;
	}


	// MUTATORS
	//---------------------------
	void SetMouseOver(bool mouseOver)
	{
		if (!mIsHidden)
		{
			mMouseIsOver = mouseOver;
		}
		else
		{
			mMouseIsOver = false;
		}
	}


private:
	// FUNCTION POINTER
	//---------------------------
	ButtonMethod mTargetMethod;
	C& mTargetClass;


	// BUTTON POSITIONING
	//---------------------------
	SPointData mPosition;
	SAABoundingBox mBoundingBox;


	// BUTTON PROPERTIES
	//---------------------------
	bool mMouseIsOver;
	bool mIsHidden;


	// BUTTON VISUALS
	//---------------------------
	ISprite* mpSprBasic;			// basic sprite
	ISprite* mpSprMO;				// mouse over sprite
};


template<class C>

struct SStructureButtons
{
	CAdvancedButton<C, void, int>* mpButtons[MAX_STRUCTURE_BUTTONS];
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


#endif /* _ADVANCED_BUTTON_H_ */
