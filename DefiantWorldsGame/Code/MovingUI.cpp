//-----------------------------------------------------
// FILE: MovingUI.cpp
//-----------------------------------------------------

#include "MovingUI.h"


//-----------------------------------------------------
// MOVING UI CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CMovingUI::CMovingUI(SPointData destination, DX::XMFLOAT2 boxDimensions, ETransitionTypes transitionType, bool active, float transitionTime)
{
	mDestination = destination;
	mTransitionType = transitionType;
	mBoxDimensions = boxDimensions;
	mToTransitionIn = active;

	// Calculate the start position based on its transition
	switch (transitionType)
	{
	case TR_NONE:
		// There is no transition, so the starting position is the same as the destination
		mStartPosition = destination;
		mCurPosition.x = (float)destination.mPosX;
		mCurPosition.y = (float)destination.mPosY;

		// Calculate bounding box based on provided dimensions and position
		mBoundingBox = { mCurPosition.y + mBoxDimensions.y, mCurPosition.x + mBoxDimensions.x, mCurPosition.y, mCurPosition.x };
		break;
	case TR_LEFT:
		// Object is transitioning in from the right of the screen - x is different y is the same
		mStartPosition.mPosX = 1600;
		mStartPosition.mPosY = destination.mPosY;
		
		mTransitionTime = transitionTime;
		mTransitionDistance = (float)(mStartPosition.mPosX - destination.mPosX);
		break;
	case TR_RIGHT:
		{
			// Object is transitioning in from the left of the screen - x is different y is the same
			// Use the bounding box dimensions to determine how far offscreen to the left it should be placed
			int offset = mBoxDimensions.x;
			mStartPosition.mPosX = -offset;
			mStartPosition.mPosY = destination.mPosY;

			mTransitionTime = transitionTime;
			mTransitionDistance = (float)(destination.mPosX - mStartPosition.mPosX);
		}
		break;
	case TR_UP:
		// Object is transitioning in from the bottom of the screen - x is the same y is different
		mStartPosition.mPosX = destination.mPosX;
		mStartPosition.mPosY = 900;

		mTransitionTime = transitionTime;
		mTransitionDistance = (float)(mStartPosition.mPosY - destination.mPosY);
		break;
	case TR_DOWN:
		{
			// Object is transitioning in from the top of the screen - x is the same y is different
			// Use the bounding box dimensions to determine how far offscreen to the top it should be placed
			int offset = mBoxDimensions.y;
			mStartPosition.mPosX = destination.mPosX;
			mStartPosition.mPosY = -offset;

			mTransitionTime = transitionTime;
			mTransitionDistance = (float)(destination.mPosY - mStartPosition.mPosY);
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
			if (mTimer >= mTransitionTime)
			{
				// Destination reached - no longer transitioning
				mCurPosition.x = (float)mDestination.mPosX;
				mCurPosition.y = (float)mDestination.mPosY;
				mToTransitionIn = false;
				mIsAtDestination = true;

				// Reset timer
				mTimer = 0.0f;
			}
			else
			{
				// Increment timer
				mTimer += gFrameTime;

				// Use the timer to calculate percentage
				float percentage = mTimer / mTransitionTime;

				// Use percentage to determine new position based on distance to travel and starting position
				mCurPosition.x = (float)mStartPosition.mPosX - (percentage * mTransitionDistance);
			}
		}
		else if (mToTransitionOut)
		{
			// Transition out to the right - positive x
			// Check to see if it has overtaken the destination
			if (mTimer >= mTransitionTime)
			{
				// Destination reached - no longer transitioning
				mCurPosition.x = (float)mStartPosition.mPosX;
				mCurPosition.y = (float)mStartPosition.mPosY;
				mToTransitionOut = false;
				mIsAtDestination = false;
				mIsOffScreen = true;

				// reset timer
				mTimer = 0.0f;
			}
			else
			{
				// Increment timer
				mTimer += gFrameTime;

				// Use the timer to calculate percentage
				float percentage = mTimer / mTransitionTime;

				// Use percentage to determine new position based on distance to travel and starting position
				mCurPosition.x = (float)mDestination.mPosX + (percentage * mTransitionDistance);
			}
		}
		break;
	case TR_RIGHT:
		// Determine whether or not the item is transitioning in or out
		if (mToTransitionIn)
		{
			// With transition type left, the item will be coming in from the right - a negative x direction
			// Check to see if it has overtaken the destination
			if (mTimer >= mTransitionTime)
			{
				// Destination reached - no longer transitioning
				mCurPosition.x = (float)mDestination.mPosX;
				mCurPosition.y = (float)mDestination.mPosY;
				mToTransitionIn = false;
				mIsAtDestination = true;

				// Reset timer
				mTimer = 0.0f;
			}
			else
			{
				// Increment timer
				mTimer += gFrameTime;

				// Use the timer to calculate percentage
				float percentage = mTimer / mTransitionTime;

				// Use percentage to determine new position based on distance to travel and starting position
				mCurPosition.x = (float)mStartPosition.mPosX + (percentage * mTransitionDistance);
			}
		}
		else if (mToTransitionOut)
		{
			// Transition out to the right - positive x
			// Check to see if it has overtaken the destination
			if (mTimer >= mTransitionTime)
			{
				// Destination reached - no longer transitioning
				mCurPosition.x = (float)mStartPosition.mPosX;
				mCurPosition.y = (float)mStartPosition.mPosY;
				mToTransitionOut = false;
				mIsAtDestination = false;
				mIsOffScreen = true;

				// reset timer
				mTimer = 0.0f;
			}
			else
			{
				// Increment timer
				mTimer += gFrameTime;

				// Use the timer to calculate percentage
				float percentage = mTimer / mTransitionTime;

				// Use percentage to determine new position based on distance to travel and starting position
				mCurPosition.x = (float)mDestination.mPosX - (percentage * mTransitionDistance);
			}
		}
		break;
	case TR_UP:
		// Determine whether or not the item is transitioning in or out
		if (mToTransitionIn)
		{
			// With transition type left, the item will be coming in from the right - a negative x direction
			// Check to see if it has overtaken the destination
			if (mTimer >= mTransitionTime)
			{
				// Destination reached - no longer transitioning
				mCurPosition.x = (float)mDestination.mPosX;
				mCurPosition.y = (float)mDestination.mPosY;
				mToTransitionIn = false;
				mIsAtDestination = true;

				// Reset timer
				mTimer = 0.0f;
			}
			else
			{
				// Increment timer
				mTimer += gFrameTime;

				// Use the timer to calculate percentage
				float percentage = mTimer / mTransitionTime;

				// Use percentage to determine new position based on distance to travel and starting position
				mCurPosition.y = (float)mStartPosition.mPosY - (percentage * mTransitionDistance);
			}
		}
		else if (mToTransitionOut)
		{
			// Transition out to the right - positive x
			// Check to see if it has overtaken the destination
			if (mTimer >= mTransitionTime)
			{
				// Destination reached - no longer transitioning
				mCurPosition.x = (float)mStartPosition.mPosX;
				mCurPosition.y = (float)mStartPosition.mPosY;
				mToTransitionOut = false;
				mIsAtDestination = false;
				mIsOffScreen = true;

				// reset timer
				mTimer = 0.0f;
			}
			else
			{
				// Increment timer
				mTimer += gFrameTime;

				// Use the timer to calculate percentage
				float percentage = mTimer / mTransitionTime;

				// Use percentage to determine new position based on distance to travel and starting position
				mCurPosition.y = (float)mDestination.mPosY + (percentage * mTransitionDistance);
			}
		}
		break;
	case TR_DOWN:
		// Determine whether or not the item is transitioning in or out
		if (mToTransitionIn)
		{
			// With transition type left, the item will be coming in from the right - a negative x direction
			// Check to see if it has overtaken the destination
			if (mTimer >= mTransitionTime)
			{
				// Destination reached - no longer transitioning
				mCurPosition.x = (float)mDestination.mPosX;
				mCurPosition.y = (float)mDestination.mPosY;
				mToTransitionIn = false;
				mIsAtDestination = true;

				// Reset timer
				mTimer = 0.0f;
			}
			else
			{
				// Increment timer
				mTimer += gFrameTime;

				// Use the timer to calculate percentage
				float percentage = mTimer / mTransitionTime;

				// Use percentage to determine new position based on distance to travel and starting position
				mCurPosition.y = (float)mStartPosition.mPosY + (percentage * mTransitionDistance);
			}
		}
		else if (mToTransitionOut)
		{
			// Transition out to the right - positive x
			// Check to see if it has overtaken the destination
			if (mTimer >= mTransitionTime)
			{
				// Destination reached - no longer transitioning
				mCurPosition.x = (float)mStartPosition.mPosX;
				mCurPosition.y = (float)mStartPosition.mPosY;
				mToTransitionOut = false;
				mIsAtDestination = false;
				mIsOffScreen = true;

				// reset timer
				mTimer = 0.0f;
			}
			else
			{
				// Increment timer
				mTimer += gFrameTime;

				// Use the timer to calculate percentage
				float percentage = mTimer / mTransitionTime;

				// Use percentage to determine new position based on distance to travel and starting position
				mCurPosition.y = (float)mDestination.mPosY - (percentage * mTransitionDistance);
			}
		}
		break;
	}
}
