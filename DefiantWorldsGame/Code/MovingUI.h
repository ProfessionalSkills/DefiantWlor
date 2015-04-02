//-----------------------------------------------------
// FILE: MovingUI.h
//-----------------------------------------------------

#ifndef _MOVING_UI_H_
#define _MOVING_UI_H_

#include "Common.h"


//-----------------------------------------------------
// TRANSITION ENUMERATION
//-----------------------------------------------------
enum ETransitionTypes
{
	TR_NONE, TR_LEFT, TR_RIGHT, TR_UP, TR_DOWN
};


//-----------------------------------------------------
// MOVING UI BASE CLASS CLASS
//-----------------------------------------------------
class CMovingUI
{
public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CMovingUI(SPointData destination, SAABoundingBox boundingBox, ETransitionTypes transitionType, bool active, float transitionTime);
	~CMovingUI();


	// ACCESSORS
	//---------------------------
	inline SAABoundingBox GetBoundingBox()
	{
		return mBoundingBox;
	}

	inline bool IsInPlace()
	{
		return mIsAtDestination;
	}

	inline bool IsOffScreen()
	{
		return mIsOffScreen;
	}


	// METHODS
	//---------------------------
	void UpdateTransition();


protected:
	// PROPERTIES
	//---------------------------
	SPointData mStartPosition;
	SPointData mCurPosition;
	SPointData mDestination;
	ETransitionTypes mTransitionType;
	float mTransitionSpeed;
	SAABoundingBox mBoundingBox;

	// Determines what the destination is
	bool mToTransitionIn = true;
	bool mToTransitionOut = false;

	// Whether the button is at its destination
	bool mIsAtDestination = false;
	bool mIsOffScreen = true;				// 
};


#endif /* _MOVING_UI_H_ */
