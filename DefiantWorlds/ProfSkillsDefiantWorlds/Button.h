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
class CButton
{
private:
	// BUTTON PROPERTIES
	//---------------------------
	SPointData mPosition;
	SAABoundingBox mBoundingBox;

	bool mMouseIsOver;
	bool mClicked;

	void (*mClickResponseFunc)(void);


	// BUTTON VISUALS
	//---------------------------
	ISprite* mpSprBasic;			// basic sprite
	ISprite* mpSprMO;				// mouse over sprite
	

public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CButton(std::string spriteName, std::string spriteMOName, SPointData pos, SAABoundingBox boundingBox,
		void (*ClickResponseFunc)(void));
	~CButton();


	// ACCESSORS
	//---------------------------
	inline SAABoundingBox GetBoundingBox()
	{
		return mBoundingBox;
	}


	// MUTATORS
	//---------------------------
	inline void SetMouseOver(bool mouseOver)
	{
		mMouseIsOver = mouseOver;
	}

	inline void SetClick(bool isClicked)
	{
		mClicked = isClicked;
	}


	// METHODS
	//---------------------------
	void Update();
};


#endif /* _BUTTON_H_ */
