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
class CMenuState;
class CWorldState;
class CSpaceState;

class CButton
{
private:
	// BUTTON PROPERTIES
	//---------------------------
	SPointData mPosition;
	SAABoundingBox mBoundingBox;

	bool mMouseIsOver;
	bool mIsHidden;

	std::string mPurpose;


	// BUTTON VISUALS
	//---------------------------
	ISprite* mpSprBasic;			// basic sprite
	ISprite* mpSprMO;				// mouse over sprite
	

public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CButton(std::string spriteName, std::string spriteMOName, SPointData pos, SAABoundingBox boundingBox,
		std::string purpose);
	~CButton();


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

	inline std::string* GetPurpose()
	{
		return &mPurpose;
	}

	inline bool IsHidden()
	{
		return mIsHidden;
	}


	// MUTATORS
	//---------------------------
	void SetMouseOver(bool mouseOver);


	// METHODS
	//---------------------------
	void Update();
	void Show();
	void Hide();
};


#endif /* _BUTTON_H_ */
