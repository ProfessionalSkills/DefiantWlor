//-----------------------------------------------------
// FILE: AdvancedButton.h
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
												// CRt = return type
												// CArgs = parameters for pointer function
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
		mpSprBasic = gpEngine->CreateSprite(defaultTex, (float)pos.mPosX, (float)pos.mPosY, 0.7f);
		mpSprMO = gpEngine->CreateSprite(selectedTex, (float)pos.mPosX, (float)pos.mPosY, -1.0f);
	}

	CAdvancedButton(SPointData pos, SAABoundingBox boundingBox, C& targetClass, ButtonMethod targetMethod) :
		mPosition(pos), mBoundingBox(boundingBox), mTargetClass(targetClass), mTargetMethod(targetMethod),
		mIsHidden(false), mMouseIsOver(false)
	{
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
		if (mIsHidden  && mpSprBasic)
		{
			mIsHidden = false;
			mpSprBasic->SetZ(0.75f);
			mpSprMO->SetZ(-1.0f);
		}
	}

	void Hide()
	{
		// If already hidden, just return
		if (!mIsHidden && mpSprBasic)
		{
			mIsHidden = true;
			mpSprBasic->SetZ(-1.0f);
			mpSprMO->SetZ(-1.0f);
		}
	}

	void LoadButtons(std::string defTexture, std::string selTexture)
	{
		if (!mpSprBasic)
		{
			mpSprBasic = gpEngine->CreateSprite(defTexture, (float)mPosition.mPosX, (float)mPosition.mPosY, 0.7f);
		}

		if (!mpSprMO)
		{
			mpSprMO = gpEngine->CreateSprite(selTexture, (float)mPosition.mPosX, (float)mPosition.mPosY, -1.0f);
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

		mpSprBasic = gpEngine->CreateSprite(fileName, (float)mPosition.mPosX, (float)mPosition.mPosY, 0.7f);
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
