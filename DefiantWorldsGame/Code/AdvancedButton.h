//-----------------------------------------------------
// FILE: AdvancedButton.h
//-----------------------------------------------------
#ifndef _ADVANCED_BUTTON_H_
#define _ADVANCED_BUTTON_H_

#include "MovingUI.h"


//-----------------------------------------------------
// ADVANCED BUTTON CLASS : CHILD OF MOVING UI CLASS
//-----------------------------------------------------
template <class C, class CRt, class... CArgs>	// C = class type for button
												// CRt = return type
												// CArgs = parameters for pointer function
class CAdvancedButton : public CMovingUI
{
public:
	// CONSTRUCTOR & DESTRUCTOR
	//---------------------------
	typedef CRt(C::*ButtonMethod)(CArgs...);
	
	CAdvancedButton(std::string defaultTex, std::string selectedTex, SPointData pos,
		DX::XMFLOAT2 boxDimensions, C& targetClass, ButtonMethod targetMethod,
		ETransitionTypes transitionType = TR_NONE, bool active = true, float transitionTime = 0.5f) :
		CMovingUI(pos, boxDimensions, transitionType, active, transitionTime), mTargetClass(targetClass),
		mTargetMethod(targetMethod), mIsHidden(false), mMouseIsOver(false)
	{
		mIsHidden = !active;
		mpSprBasic = gpEngine->CreateSprite(defaultTex, mCurPosition.x, mCurPosition.y, 0.7f);
		mpSprMO = gpEngine->CreateSprite(selectedTex, mCurPosition.x, mCurPosition.y, -1.0f);
	}

	CAdvancedButton(SPointData pos, DX::XMFLOAT2 boxDimensions, C& targetClass, ButtonMethod targetMethod,
		ETransitionTypes transitionType = TR_NONE, bool active = true, float transitionTime = 0.5f) :
		CMovingUI(pos, boxDimensions, transitionType, active, transitionTime), mTargetClass(targetClass),
		mTargetMethod(targetMethod), mIsHidden(false), mMouseIsOver(false)
	{
		mIsHidden = !active;
		mpSprBasic = nullptr;
		mpSprMO = nullptr;
	}

	~CAdvancedButton()
	{
		if (mpSprBasic)
		{
			gpEngine->RemoveSprite(mpSprBasic);
		}
		
		if (mpSprMO)
		{
			gpEngine->RemoveSprite(mpSprMO);
		}
	}


	// METHODS
	//---------------------------
	void Execute(CArgs&... parameters)
	{
		(mTargetClass.*(mTargetMethod))(parameters...);
	}

	void Update()
	{
		// If it is transitioning in or out, update the button's position
		if (mToTransitionIn || mToTransitionOut)
		{
			// This is a moving user interface element - call the parent function first to update its position
			UpdateTransition();

			// Calculate bounding box based on provided dimensions and position
			mBoundingBox = { mCurPosition.y + mBoxDimensions.y, mCurPosition.x + mBoxDimensions.x, mCurPosition.y, mCurPosition.x };

			// If sprite exists
			if (mpSprBasic)
			{
				mpSprBasic->SetX(mCurPosition.x);
				mpSprBasic->SetY(mCurPosition.y);
			}
			if (mpSprMO)
			{
				mpSprMO->SetX(mCurPosition.x);
				mpSprMO->SetY(mCurPosition.y);
			}
		}
		
		// Check if the button is hovered over
		if (mMouseIsOver && !mIsHidden && mpSprBasic)
		{
			// Set mouse over sprite to front & basic to back
			mpSprBasic->SetZ(-1.0f);
			mpSprMO->SetZ(0.7f);
		}
		else if (!mMouseIsOver && !mIsHidden && mpSprBasic)
		{
			mpSprBasic->SetZ(0.7f);
			mpSprMO->SetZ(-1.0f);
		}
	}

	void Show()
	{
		// If already shown, just return
		if (mIsHidden && mpSprBasic)
		{
			mIsHidden = false;

			// If not using transitioning, show the buttons
			if (mTransitionType == TR_NONE)
			{
				mpSprBasic->SetZ(0.75f);
				mpSprMO->SetZ(-1.0f);
			}

			// Set to transition in
			mToTransitionIn = true;
			mIsAtDestination = false;
			mIsOffScreen = false;
		}
	}

	void Hide()
	{
		// If already hidden, just return
		if (!mIsHidden && mpSprBasic)
		{
			// Raise hidden flag
			mIsHidden = true;
			
			// If not using transitioning, hide the buttons
			if (mTransitionType == TR_NONE)
			{
				mpSprBasic->SetZ(-1.0f);
				mpSprMO->SetZ(-1.0f);
			}

			// Set to transition out
			mToTransitionOut = true;
			mIsAtDestination = false;
			mIsOffScreen = false;
		}
	}

	void LoadButtons(std::string defTexture, std::string selTexture)
	{
		if (!mpSprBasic)
		{
			mpSprBasic = gpEngine->CreateSprite(defTexture, mCurPosition.x, mCurPosition.y, 0.7f);
		}

		if (!mpSprMO)
		{
			mpSprMO = gpEngine->CreateSprite(selTexture, mCurPosition.x, mCurPosition.y, -1.0f);
		}
	}

	void UnloadButtons()
	{
		if (mpSprBasic)
		{
			gpEngine->RemoveSprite(mpSprBasic);
			mpSprBasic = nullptr;
		}

		if (mpSprMO)
		{
			gpEngine->RemoveSprite(mpSprMO);
			mpSprMO = nullptr;
		}
	}

	void SetNewButtonSkin(std::string fileName)
	{
		// Delete old basic button & create new one with new skin
		if (mpSprBasic)
		{
			gpEngine->RemoveSprite(mpSprBasic);
			mpSprBasic = nullptr;
		}

		mpSprBasic = gpEngine->CreateSprite(fileName, mCurPosition.x, mCurPosition.y, 0.7f);
	}


	// ACCESSORS
	//---------------------------
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
		if (!mIsHidden && mpSprBasic)
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

	// Returns the index of the button which is being hovered over
	int GetMouseOverIndex()
	{
		for (int i = 0; i < mNumButtons; i++)
		{
			if (mpButtons[i]->GetMouseOver())
			{
				return i;
			}
		}

		return -1;
	}

	// Load all sprite buttons
	void LoadButtons(std::string defTexture, std::string selTexture)
	{
		for (int i = 0; i < mNumButtons; i++)
		{
			mpButtons[i]->LoadButtons(defTexture, selTexture);
		}
	}

	// Unloads all the buttons when no longer required
	void UnloadSprites()
	{
		for (int i = 0; i < mNumButtons; i++)
		{
			mpButtons[i]->UnloadButtons();
		}
	}
};


#endif /* _ADVANCED_BUTTON_H_ */
