//-----------------------------------------------------
// FILE: Common.h
//-----------------------------------------------------

#ifndef _COMMON_H_
#define _COMMON_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include <vector>
#include <list>
#include <queue>

#include <string>
#include <sstream>
#include "SoundObject.h"
#include "Particles.h"
#include <iostream>


//-----------------------------------------------------
// ENUMERATIONS
//-----------------------------------------------------
enum EGameStates
{
	GS_MAIN_MENU, GS_WORLD, GS_SPACE
};

enum EMouseStates
{
	MS_OUT_OF_GRID, MS_EARTH_GRID, MS_MARS_GRID, MS_UI
};

enum EObjectStates
{
	OBJ_CONSTRUCTING, OBJ_BUILT, OBJ_DAMAGED, OBJ_WARNING, OBJ_DEAD
};

enum EGameAgentVariations
{
	GAV_FIGHTER, GAV_BOMBER,   GAV_MOTHERSHIP, GAV_SPACE_FIGHTER, GAV_TRANSPORT,
	GAV_WORKER,  GAV_INFANTRY, GAV_TANK,       GAV_ARTILLERY
};

enum EGameStructureTypes
{
	STR_HOUSE, STR_COM_CENTRE, STR_SPACE_CENTRE, STR_BARRACKS,
	STR_HELLIPAD
};

enum EFactions
{
	FAC_EARTH_DEFENSE_FORCE, FAC_THE_CRIMSON_LEGION
};


//-----------------------------------------------------
// GLOBAL VARIABLES
//-----------------------------------------------------
const int WINDOW_POS_X = 50;
const int WINDOW_POS_Y = 50;

const int WINDOW_WIDTH = 1600;
const int WINDOW_HEIGHT = 900;

const int MAX_UNITS = 50;
const int MAX_SPACE_UNITS = 10;

const int GRID_SIZE_X = 60;
const int GRID_SIZE_Y = 60;
const float GRID_TILE_SIZE = 10.0f;

const float NEAR_CLIP = 1.0f;
const float FAR_CLIP = 10000.0f;

const float CAM_MOVE_SPEED = 100.0f;
const float CAM_SCROLL_SPEED = 5000.0f;
const float CAM_MAX_HEIGHT = 250.0f;
const float CAM_MIN_HEIGHT = 30.0f;

const int MAX_AI_NUM = 1;
const int MAX_STRUCTURE_BUTTONS = 5;

// Store the current game state
extern EGameStates gCurState;

// Global pointer to the engine. Can be used by anything including this file
extern CRandomiser* gpRandomiser;

// Global frametime & mouse wheel delta
extern float gFrameTime;
extern float gMouseWheelDelta;


//-----------------------------------------------------
// STRUCTURES
//-----------------------------------------------------
struct SPointData
{
	int mPosX;
	int mPosY;

	SPointData() 
	{
		mPosX = 0;
		mPosY = 0; 
	}

	SPointData(int x, int y)
	{ 
		mPosX = x;
		mPosY = y;
	}
};

struct SBoundingCube
{
	DX::BoundingBox mBox;

	SBoundingCube(){};

	SBoundingCube(DX::XMFLOAT3 point1, DX::XMFLOAT3 point2)
	{
		//mBox.CreateFromPoints(mBox, numPoints, pPoints, 12);
		mBox.CreateFromPoints(mBox, DX::XMLoadFloat3(&point1), DX::XMLoadFloat3(&point2));
	}

	bool GetCollisionDistance(DX::XMVECTOR origin, DX::XMVECTOR direction,
		float& distance)
	{
		return mBox.Intersects(origin, direction, distance);
	}
};

struct SAABoundingBox		// Axis aligned bounding box
{
	float mTop;			// Max Z
	float mRight;		// Max X
	float mBottom;		// Min Z
	float mLeft;		// Min X

	SAABoundingBox()
	{ 
		mTop = 0.0f; 
		mRight = 0.0f;
		mBottom = 0.0f;
		mLeft = 0.0f; 
	}

	SAABoundingBox(float top, float right, float bottom, float left)
	{
		mTop = top;
		mRight = right;
		mBottom = bottom;
		mLeft = left;
	}

	bool IsColliding(DX::XMFLOAT3 pos)
	{
		if (pos.x >= mLeft && pos.x <= mRight && pos.z >= mBottom
			&& pos.z <= mTop)
		{
			// Point within boundary
			return true;
		}

		// No collision
		return false;
	}
};


//-----------------------------------------------------
// TEMPLATE FUNCTIONS
//-----------------------------------------------------
// Function takes any type of pointer, removes it and sets it to null
template<class GenericPtr>
void SafeDelete(GenericPtr*& inPtr)
{
	// Check if the pointer exists
	if (inPtr)
	{
		// Delete the pointer
		delete inPtr;
		// Set it to null
		inPtr = nullptr;
	}
}


//-----------------------------------------------------
// HELPER MATHS FUNCTIONS
//-----------------------------------------------------
float Clampf(float inMin, float inMax, float inVal);
float ToRadians(float inDegrees);
float ToDegrees(float inRadians);


#endif /* _COMMON_H_ */
