//-----------------------------------------------------
// FILE: TypeBox.h
//-----------------------------------------------------

#ifndef _TYPE_BOX_H_
#define _TYPE_BOX_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Common.h"


//-----------------------------------------------------
// TYPE BOX CLASS
//-----------------------------------------------------
class CTypeBox
{
public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CTypeBox(SPointData position);
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

	inline void SetSelected(bool selected)
	{
		mSelected = selected;
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
	SPointData mPosition{ 0, 0 };
	DX::XMFLOAT2 mBoxDimensions{ 500.0f, 20.0f };		// Dimensions of the input box: x = width, y = height
	SAABoundingBox mBoundingBox;
	std::stringstream mTypeStream;


	// MISC
	//---------------------------
	bool mMouseOver = false;
	bool mSelected = false;
	int mCurLength = 0;			// Tracks the length of the entered string
};

#endif /* _TYPE_BOX_H_ */
