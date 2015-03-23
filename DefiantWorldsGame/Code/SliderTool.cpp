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
	SetSliderSetting(curSetting);

	// Load the sprites
	DX::XMFLOAT2 pullStartPos{ (float)position.mPosX - 15.0f, (float)position.mPosY - 5.0f };
	mpSprSlider = gpEngine->CreateSprite("SliderBack.png", (float)position.mPosX, (float)position.mPosY, 0.8f);
	mpSprPullBasic = gpEngine->CreateSprite("DefSliderPull.png", pullStartPos.x, pullStartPos.y, 0.7f);
	mpSprPullMO = gpEngine->CreateSprite("SelSliderPull.png", pullStartPos.x, pullStartPos.y, -1.0f);

	// Determine each stop position
	float step = mBarDimensions.x / (float)numSettings;
	for (int i = 0; i < numSettings; i++)
	{
		mSettingPositions.push_back(pullStartPos);
		pullStartPos.x += step;
	}

	// Set the pull sprite to the position of its index
	mpSprPullBasic->SetX(mSettingPositions[mCurSetting].x);
	mpSprPullMO->SetX(mSettingPositions[mCurSetting].x);

	// Define the bounding box around the slider
	mBoundingBox = SAABoundingBox(position.mPosY + mBarDimensions.y, position.mPosX + mBarDimensions.x, position.mPosY, position.mPosX);
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
void CSliderTool::Update()
{
	// Set the pull sprite to the position of its index
	mpSprPullBasic->SetX(mSettingPositions[mCurSetting].x);
	mpSprPullMO->SetX(mSettingPositions[mCurSetting].x);

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
