//-----------------------------------------------------
// FILE: MovingUI.cpp
//-----------------------------------------------------

#include "MovingUI.h"


//-----------------------------------------------------
// MOVING UI CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CMovingUI::CMovingUI(SPointData destination, SAABoundingBox boundingBox, ETransitionTypes transitionType, bool active, float transitionTime)
{
	mDestination = destination;
	mTransitionType = transitionType;
	mBoundingBox = boundingBox;
	mToTransitionIn = active;

	// Calculate the start position based on its transition
	switch (transitionType)
	{
	case TR_NONE:
		// There is no transition, so the starting position is the same as the destination
		mStartPosition = destination;
		break;
	case TR_LEFT:
		// Object is transitioning in from the right of the screen - x is different y is the same
		mStartPosition.mPosX = 1600;
		mStartPosition.mPosY = destination.mPosY;
		// Calculate the transitioning speed (s = d/t)
		mTransitionSpeed = ((float)(mStartPosition.mPosX - destination.mPosX)) / transitionTime;
		break;
	case TR_RIGHT:
		{
			// Object is transitioning in from the left of the screen - x is different y is the same
			// Use the bounding box dimensions to determine how far offscreen to the left it should be placed
			int offset = mBoundingBox.mLeft - mBoundingBox.mRight;
			mStartPosition.mPosX = offset;
			mStartPosition.mPosY = destination.mPosY;
		}
		break;
	case TR_UP:
		// Object is transitioning in from the bottom of the screen - x is the same y is different
		mStartPosition.mPosX = destination.mPosX;
		mStartPosition.mPosY = 900;
		break;
	case TR_DOWN:
		{
			// Object is transitioning in from the top of the screen - x is the same y is different
			// Use the bounding box dimensions to determine how far offscreen to the top it should be placed
			int offset = mBoundingBox.mLeft - mBoundingBox.mRight;
			mStartPosition.mPosX = destination.mPosX;
			mStartPosition.mPosY = offset;
		}
		break;
	}

	// Store the current position as the start position
	mCurPosition.x = (float)mStartPosition.mPosX;
	mCurPosition.y = (float)mStartPosition.mPosY;
}

CMovingUI::~CMovingUI()
{

}


//-----------------------------------------------------
// MOVING UI CLASS METHODS
//-----------------------------------------------------
void CMovingUI::UpdateTransition()
{
	// Determine what type of transitioning is being used
	switch (mTransitionType)
	{
	case TR_NONE:
		// If the transition is nothing, lower/raise relevant flags and leave function
		mToTransitionIn = false;
		mToTransitionOut = false;
		mIsAtDestination = true;
		mIsOffScreen = false;
		break;
	case TR_LEFT:
		// Determine whether or not the item is transitioning in or out
		if (mToTransitionIn)
		{
			// With transition type left, the item will be coming in from the right - a negative x direction
			// Check to see if it has overtaken the destination
			if (mCurPosition.x <= mDestination.mPosX)
			{
				// Destination reached - no longer transitioning
				mCurPosition.x = (float)mDestination.mPosX;
				mCurPosition.y = (float)mDestination.mPosY;
				mToTransitionIn = false;
				mIsAtDestination = true;
			}
			else
			{
				// Not yet where it should be positioned - move it closer
				mCurPosition.x -= mTransitionSpeed * gFrameTime;
			}
		}
		else if (mToTransitionOut)
		{
			// Transition out to the right - positive x
			// Check to see if it has overtaken the destination
			if (mCurPosition.x >= mStartPosition.mPosX)
			{
				// Destination reached - no longer transitioning
				mCurPosition.x = (float)mStartPosition.mPosX;
				mCurPosition.y = (float)mStartPosition.mPosY;
				mToTransitionOut = false;
				mIsAtDestination = false;
				mIsOffScreen = true;
			}
			else
			{
				// Not yet where it should be positioned - move it closer
				mCurPosition.x += mTransitionSpeed * gFrameTime;
			}
		}
		break;
	case TR_RIGHT:
		// Determine whether or not the item is transitioning in or out
		if (mToTransitionIn)
		{
			// With transition type right, the item will be coming in from the left - a positive x direction
		}
		else if (mToTransitionOut)
		{
			// Transition out to the left - negative x
		}
		break;
	case TR_UP:
		// Determine whether or not the item is transitioning in or out
		if (mToTransitionIn)
		{
			// With transition type up, the item will be coming in from the bottom - a negative y direction
		}
		else if (mToTransitionOut)
		{
			// Transition out to the bottom - positive y
		}
		break;
	case TR_DOWN:
		// Determine whether or not the item is transitioning in or out
		if (mToTransitionIn)
		{
			// With transition type down, the item will be coming in from the top - a positive y direction

		}
		else if (mToTransitionOut)
		{
			// Transition out to the top - negative y
		}
		break;
	}
}
