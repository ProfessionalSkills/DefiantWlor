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
#include <unordered_map>

#include <string>
#include <sstream>
#include <iostream>
#include "SoundObject.h"
#include "Particles.h"
#include "NewsTicker.h"


//-----------------------------------------------------
// ENUMERATIONS
//-----------------------------------------------------
enum EGameStates
{
	GS_MAIN_MENU, GS_WORLD, GS_SPACE
};

enum EMouseStates
{
	MS_NO_AREA, MS_EARTH_GRID, MS_EARTH_EDGE, MS_MARS_GRID, MS_MARS_EDGE, MS_UI
};

enum EObjectStates
{
	OBJ_CONSTRUCTING, OBJ_BUILT, OBJ_DEAD, OBJ_INSPACE
};

enum EQueueObjectType
{
	Q_BARRACKS,       Q_HELLIPAD,      Q_SPACE_CENTRE,  Q_HOUSE,      Q_FIGHTER,
	Q_BOMBER,         Q_MOTHERSHIP,    Q_SPACE_FIGHTER, Q_TRANSPORT,  Q_WORKER,
	Q_INFANTRY,       Q_TANK,          Q_ARTILLERY,     Q_MOVE_UNIT,  Q_MOVE_UNIT_GROUP,
	Q_CHANGE_TACTIC
};

enum EGameAgentVariations
{
	GAV_FIGHTER, GAV_BOMBER,   GAV_MOTHERSHIP, GAV_SPACE_FIGHTER, GAV_TRANSPORT,
	GAV_WORKER,	 GAV_INFANTRY, GAV_TANK,       GAV_ARTILLERY
};

enum EGameStructureTypes
{
	STR_HOUSE, STR_COM_CENTRE, STR_SPACE_CENTRE, STR_BARRACKS,
	STR_HELLIPAD, STR_AA, STR_WALL
};

enum EFactions
{
	FAC_EARTH_DEFENSE_FORCE, FAC_THE_CRIMSON_LEGION, FAC_REBELS
};

enum EErrorTypes
{
	ERR_NONE, ERR_NO_SPACE, ERR_NO_MINERALS, ERR_MAX_QUEUE_SIZE, ERR_NOT_READY,
	ERR_INCORRECT_INDEX, ERR_POP_LIMIT,
};

enum EAirspaces
{
	AS_NONE, AS_EARTH, AS_MARS
};


//-----------------------------------------------------
// GLOBAL VARIABLES
//-----------------------------------------------------
const int WINDOW_POS_X = 0;
const int WINDOW_POS_Y = 0;

const int WINDOW_WIDTH = 1600;
const int WINDOW_HEIGHT = 900;

const int MAX_UNITS = 50;
const int MAX_SPACE_UNITS = 10;

const int GRID_SIZE_X = 60;
const int GRID_SIZE_Y = 60;
const float GRID_TILE_SIZE = 10.0f;

const float NEAR_CLIP = 1.0f;
const float FAR_CLIP = 10000.0f;

const float CAM_MOVE_SPEED = 200.0f;
const float CAM_SCROLL_SPEED = 5000.0f;
const float CAM_MAX_HEIGHT = 250.0f;
const float CAM_MIN_HEIGHT = 30.0f;

const int MAX_AI_NUM = 1;
const int MAX_STRUCTURE_BUTTONS = 5;

// Store the current game state
extern EGameStates gCurState;

// Global pointer to the engine. Can be used by anything including this file
extern CRandomiser* gpRandomiser;
extern CNewsTicker* gpNewsTicker;

// Global frametime & mouse wheel delta
extern float gFrameTime;
extern float gMouseWheelDelta;

// Global pointer to the current camera - for lookat functions when doing particles and other various things
extern ICamera* gpCurWorldCamera;


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
		mBox.CreateFromPoints(mBox, DX::XMLoadFloat3(&point1), DX::XMLoadFloat3(&point2));
	}

	bool GetCollisionDistance(DX::XMVECTOR origin, DX::XMVECTOR direction,
		float& distance)
	{
		return mBox.Intersects(origin, direction, distance);
	}

	float GetWidthExtent()
	{
		return mBox.Extents.x;
	}

	float GetLengthExtent()
	{
		return mBox.Extents.z;
	}

	void SetPosition(DX::XMFLOAT3 position)
	{
		mBox.Center = position;
	}
};

struct SBoundingSphere
{
	DX::BoundingSphere mSphere;

	SBoundingSphere(){};

	SBoundingSphere(const DX::XMFLOAT3 &centre, float radius)
	{
		mSphere.Center = centre;
		mSphere.Radius = radius;
	}

	bool GetCollisionDistance(DX::XMVECTOR origin, DX::XMVECTOR direction,
		float& distance)
	{
		return mSphere.Intersects(origin, direction, distance);
	}

	bool CollidingWith(DX::BoundingBox& pBox)
	{
		return mSphere.Intersects(pBox);
	}

	bool CollidingWith(DX::BoundingSphere& sphere)
	{
		return mSphere.Intersects(sphere);
	}

	void MoveTo(DX::XMFLOAT3 pos)
	{
		float scale = 1.0f;
		DX::XMFLOAT3 rotation = { 0.0f, 0.0f, 0.0f };
		mSphere.Transform(mSphere, scale, DX::XMLoadFloat3(&rotation), DX::XMLoadFloat3(&pos));
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

struct SProjectile
{
	IModel* mModel = nullptr;
	DX::XMFLOAT3 mPos;
	DX::XMFLOAT3 mDirection;
	float mSpeed;
	float mLifeTime;
	SBoundingSphere mCollisionSphere;

	void Update()
	{
		if (mModel)
		{
			mModel->Move(mDirection.x * mSpeed * gFrameTime, mDirection.y * mSpeed * gFrameTime, mDirection.z * mSpeed * gFrameTime);
			mPos = { mModel->GetX(), mModel->GetY(), mModel->GetZ() };
			mCollisionSphere.MoveTo(mPos);
			mLifeTime -= gFrameTime;
		}
	}

	~SProjectile()
	{
		if (mModel)
		{
			IMesh* pMesh = mModel->GetMesh();
			pMesh->RemoveModel(mModel);
		}
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

float distance(float x, float y, float z);
float Dot(const DX::XMFLOAT3 v1, const DX::XMFLOAT3 v2);
DX::XMFLOAT3 Cross(const  DX::XMFLOAT3 v1, const  DX::XMFLOAT3 v2);
bool BoxCollision(const DX::XMFLOAT3 v1, const DX::XMFLOAT3 v2, float radius);

#endif /* _COMMON_H_ */
