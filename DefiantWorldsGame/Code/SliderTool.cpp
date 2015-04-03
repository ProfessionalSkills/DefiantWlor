//-----------------------------------------------------
// FILE: SliderTool.cpp
//-----------------------------------------------------
#include "SliderTool.h"


//-----------------------------------------------------
// SLIDER TOOL CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CSliderTool::CSliderTool(SPointData position, int numSettings, int curSetting, DX::XMFLOAT2 boxDimensions,
	ETransitionTypes transitionType, bool active, float transitionTime) : CMovingUI(position, boxDimensions, 
	transitionType, active, transitionTime)
{
	// Set the slider's properties
	mNumSettings = numSettings;
	SetSliderSetting(curSetting);
	mIsHidden = !active;

	// Load the sprites
	DX::XMFLOAT2 pullStartPos{ mCurPosition.x - 15.0f, mCurPosition.y - 5.0f };
	mpSprSlider = gpEngine->CreateSprite("SliderBack.png", mCurPosition.x, mCurPosition.y, 0.8f);
	mpSprPullBasic = gpEngine->CreateSprite("DefSliderPull.png", pullStartPos.x, pullStartPos.y, 0.7f);
	mpSprPullMO = gpEngine->CreateSprite("SelSliderPull.png", pullStartPos.x, pullStartPos.y, -1.0f);

	// Determine each stop position
	float step = mBoxDimensions.x / (float)numSettings;
	for (int i = 0; i < numSettings; i++)
	{
		mSettingPositions.push_back(pullStartPos);
		pullStartPos.x += step;
	}

	// Set the pull sprite to the position of its index
	mpSprPullBasic->SetX(mSettingPositions[mCurSetting].x);
	mpSprPullMO->SetX(mSettingPositions[mCurSetting].x);

	// Define the bounding box around the slider
	mBoundingBox = SAABoundingBox(mCurPosition.y + mBoxDimensions.y, mCurPosition.x + mBoxDimensions.x,
		mCurPosition.y, mCurPosition.x);
}

CSliderTool::~CSliderTool()
{
	// Remove the sprites
	if (mpSprPullBasic)
	{
		gpEngine->RemoveSprite(mpSprPullBasic);
	}
	if (mpSprSlider)
	{
		gpEngine->RemoveSprite(mpSprSlider);
	}
	if (mpSprPullMO)
	{
		gpEngine->RemoveSprite(mpSprPullMO);
	}
}


//-----------------------------------------------------
// SLIDER TOOL CLASS MUTATORS
//-----------------------------------------------------
void CSliderTool::SetSliderSetting(int setting)
{
	// When changing the current setting, ensure it is not less than 0 or above the number of maximum setting
	if (setting < 0)
	{
		mCurSetting = 0;
	}
	else if (setting > mNumSettings - 1)
	{
		mCurSetting = mNumSettings - 1;
	}
	else
	{
		mCurSetting = setting;
	}
}

void CSliderTool::IncrementSlider()
{
	// Increment the current setting, but not beyond the number of settings
	mCurSetting++;
	if (mCurSetting >= mNumSettings)
	{
		mCurSetting = mNumSettings - 1;
	}
}

void CSliderTool::DecrementSlider()
{
	// Decrement the current setting, but do not allow it to be less than 0
	mCurSetting--;
	if (mCurSetting < 0)
	{
		mCurSetting = 0;
	}
}

void CSliderTool::OnClick(int mousePosX)
{
	float prevDistance = 9999.0f;
	float curDistance = 999.0f;
	float positionX = (float)mousePosX - 15.0f;

	// Loop through each of the stopping positions to determine which is the closest
	for (int i = 0; i < mNumSettings; i++)
	{
		// Calculate the distance to the next point
		curDistance = fabs(mSettingPositions[i].x - positionX);
		
		// If the new current distance is greater than the previous distance, then we are clearly going away from the closest position found
		if (curDistance > prevDistance)
		{
			// Set the current position to be one less than i
			mCurSetting = i - 1;
			return;
		}

		prevDistance = curDistance;
	}

	// If it got all the way to the end, then the setting must be the last one
	mCurSetting = mNumSettings - 1;
}


//-----------------------------------------------------
// SLIDER TOOL CLASS METHODS
//-----------------------------------------------------
void CSliderTool::Show()
{
	// If already shown, just return
	if (mIsHidden && mpSprSlider)
	{
		mIsHidden = false;

		// If not using transitioning, show the buttons
		if (mTransitionType == TR_NONE)
		{
			mpSprSlider->SetZ(0.8f);
			mpSprPullBasic->SetZ(0.7f);
		}

		// Set to transition in
		mToTransitionIn = true;
		mIsAtDestination = false;
		mIsOffScreen = false;
	}
}

void CSliderTool::Hide()
{
	// If already hidden, just return
	if (!mIsHidden && mpSprSlider)
	{
		// Raise hidden flag
		mIsHidden = true;

		// If not using transitioning, hide the buttons
		if (mTransitionType == TR_NONE)
		{
			mpSprSlider->SetZ(-1.0f);
			mpSprPullBasic->SetZ(-1.0f);
		}

		// Set to transition out
		mToTransitionOut = true;
		mIsAtDestination = false;
		mIsOffScreen = false;
	}
}

void CSliderTool::Update()
{
	// If it is transitioning in or out, update the button's position
	if (mToTransitionIn || mToTransitionOut)
	{
		// This is a moving user interface element - call the parent function first to update its position
		UpdateTransition();

		// Calculate bounding box based on provided dimensions and position
		mBoundingBox = { mCurPosition.y + mBoxDimensions.y, mCurPosition.x + mBoxDimensions.x, mCurPosition.y, mCurPosition.x };

		// If sprite exists
		if (mpSprSlider)
		{
			mpSprSlider->SetX(mCurPosition.x);
			mpSprSlider->SetY(mCurPosition.y);
		}

		// Update draw positions
		DX::XMFLOAT2 pullStartPos{ mCurPosition.x - 15.0f, mCurPosition.y - 5.0f };
		mSettingPositions.clear();
		float step = mBoxDimensions.x / (float)mNumSettings;
		for (int i = 0; i < mNumSettings; i++)
		{
			mSettingPositions.push_back(pullStartPos);
			pullStartPos.x += step;
		}
	}

	// Set the pull sprite to the position of its index
	mpSprPullBasic->SetX(mSettingPositions[mCurSetting].x);
	mpSprPullMO->SetX(mSettingPositions[mCurSetting].x);

	// If the element is hidden, do not do the rest of the function
	if (mIsHidden) return;

	// Check to see if there is a mouse over event taking place
	if (mMouseOver)
	{
		mpSprPullBasic->SetZ(-1.0f);
		mpSprPullMO->SetZ(0.7f);
	}
	else
	{
		mpSprPullMO->SetZ(-1.0f);
		mpSprPullBasic->SetZ(0.7f);
	}
}
