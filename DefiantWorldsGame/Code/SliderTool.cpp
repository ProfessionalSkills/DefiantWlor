//-----------------------------------------------------
// FILE: SliderTool.cpp
//-----------------------------------------------------

#include "SliderTool.h"


//-----------------------------------------------------
// SLIDER TOOL CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CSliderTool::CSliderTool(SPointData position, int numSettings, int curSetting)
{
	// Set the slider's properties
	mPosition = position;
	mNumSettings = numSettings;
	mCurSetting = curSetting;

	// Load the sprites
	mpSprSlider = gpEngine->CreateSprite("SliderBack.png", (float)position.mPosX, (float)position.mPosY, 0.9f);
	mpSprPull = gpEngine->CreateSprite("DefSliderPull.png", (float)position.mPosX - 5.0f, (float)position.mPosY - 5.0f, 0.8f);
}

CSliderTool::~CSliderTool()
{
	// Remove the sprites
	if (mpSprPull)
	{
		gpEngine->RemoveSprite(mpSprPull);
	}
	if (mpSprSlider)
	{
		gpEngine->RemoveSprite(mpSprSlider);
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
	else if (setting > mNumSettings)
	{
		mCurSetting = mNumSettings;
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
	if (mCurSetting > mNumSettings)
	{
		mCurSetting = mNumSettings;
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


//-----------------------------------------------------
// SLIDER TOOL CLASS METHODS
//-----------------------------------------------------
void CSliderTool::Update()
{

}
