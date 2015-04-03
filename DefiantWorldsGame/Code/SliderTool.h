//-----------------------------------------------------
// FILE: SliderTool.h
//-----------------------------------------------------
#ifndef _SLIDER_TOOL_H_
#define _SLIDER_TOOL_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "MovingUI.h"


//-----------------------------------------------------
// BUILD REQUEST CLASS : CHILD OF MOVING UI BASE CLASS
//-----------------------------------------------------
class CSliderTool : public CMovingUI
{
public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CSliderTool(SPointData position, int numSettings, int curSetting, DX::XMFLOAT2 boxDimensions, 
		ETransitionTypes trnsitionType = TR_NONE, bool active = true, float transitionTime = 0.5f);
	~CSliderTool();


	// ACCESSORS
	//---------------------------
	inline int GetSliderSetting()
	{
		return mCurSetting + 1;
	}

	inline SAABoundingBox GetBoundingBox()
	{
		return mBoundingBox;
	}

	inline bool IsMouseOver()
	{
		return mMouseOver;
	}


	// MUTATORS
	//---------------------------
	// Set the slider to a specific setting on the bar
	void SetSliderSetting(int setting);
	// Increment the slider by 1 point
	void IncrementSlider();
	// Decrement the slider by 1 point
	void DecrementSlider();
	// Run this function when the slider is clicked on
	void OnClick(int mousePosX);

	inline void SetMouseOver(bool mouseOver)
	{
		mMouseOver = mouseOver;
	}


	// METHODS
	//---------------------------
	void Show();
	void Hide();
	void Update();


private:
	// SPRITES
	//---------------------------
	ISprite* mpSprSlider = nullptr;
	ISprite* mpSprPullBasic = nullptr;
	ISprite* mpSprPullMO = nullptr;


	// SLIDER BAR PROPERTIES
	//---------------------------
	int mCurSetting = 0;
	int mNumSettings = 1;

	//DX::XMFLOAT2 mBarDimensions{500.0f, 20.0f};		// Dimensions of the slider bar: x = width, y = height

	std::vector<DX::XMFLOAT2> mSettingPositions;	// A setting position for each stop which spans length of bar
													// Using mCurSetting as an index, the stop position can be accessed


	// SLIDER BAR STATES
	//---------------------------
	bool mIsHidden = false;
	bool mMouseOver = false;
};

#endif /* _SLIDER_TOOL_H_ */
