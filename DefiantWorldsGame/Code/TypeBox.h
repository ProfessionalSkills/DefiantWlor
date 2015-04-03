//-----------------------------------------------------
// FILE: TypeBox.h
//-----------------------------------------------------

#ifndef _TYPE_BOX_H_
#define _TYPE_BOX_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "MovingUI.h"


//-----------------------------------------------------
// TYPE BOX CLASS : CHILD OF MOVING UI BASE CLASS
//-----------------------------------------------------
class CTypeBox : public CMovingUI
{
public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CTypeBox(SPointData position, DX::XMFLOAT2 boxDimensions, ETransitionTypes transitionType = TR_NONE,
		bool active = true, float transitionTime = 0.5f);
	~CTypeBox();


	// ACCESSORS
	//---------------------------
	inline SAABoundingBox GetBoundingBox()
	{
		return mBoundingBox;
	}

	inline bool IsMouseOver()
	{
		return mMouseOver;
	}

	inline std::string GetText()
	{
		return mTypeStream.str();
	}


	// MUTATORS
	//---------------------------
	inline void SetMouseOver(bool mouseOver)
	{
		mMouseOver = mouseOver;
	}

	inline void SetText(std::string& text)
	{
		mTypeStream.str("");
		mTypeStream << text;
		mCurLength = text.length();
	}


	// METHODS
	//---------------------------
	void Update();


private:
	// SPRITES & FONTS
	//---------------------------
	ISprite* mpSprBox = nullptr;
	IFont* mpFntText = nullptr;


	// TYPE BOX PROPERTIES
	//---------------------------
	//DX::XMFLOAT2 mBoxDimensions{ 500.0f, 40.0f };		// Dimensions of the input box: x = width, y = height
	std::stringstream mTypeStream;


	// MISC
	//---------------------------
	bool mMouseOver = false;
	int mCurLength = 0;			// Tracks the length of the entered string
};

#endif /* _TYPE_BOX_H_ */
