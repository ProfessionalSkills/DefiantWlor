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
#include <DirectXMath.h>		// Contains vector, matrices, etc. libraries 
								// Everything here is contained in the DirectX namespace (see below)
#include <string>
#include <sstream>
#include "SoundObject.h"
#include "Particles.h"

// Renamed the DirectX namespace so that DirectX:: is not required all the time. Simply use DX:: instead.
// e.g. Vector3 variable created by doing DX::XMFLOAT3 mPos; just as an example.
namespace DX = DirectX;


//-----------------------------------------------------
// ENUMERATIONS
//-----------------------------------------------------
enum EGameStates
{
	GS_MAIN_MENU, GS_WORLD, GS_SPACE
};

enum EObjectStates
{
	OBJ_CONSTRUCTING, OBJ_BUILT, OBJ_DAMAGED, OBJ_DEAD
};

enum EGameAgentsList
{
	GA_FIGHTER, GA_BOMBER,   GA_MOTHERSHIP, GA_SPACE_FIGHTER, GA_TRANSPORT,
	GA_WORKER,  GA_INFANTRY, GA_TANK,       GA_ARTILLERY
};


//-----------------------------------------------------
// STRUCTURES
//-----------------------------------------------------
struct SPointData
{
	int mPosX;
	int mPosY;

	SPointData() { mPosX = 0; mPosY = 0; }
	SPointData(int x, int y) { mPosX = x; mPosY = y; }
};


//-----------------------------------------------------
// GLOBAL VARIABLES
//-----------------------------------------------------
const float PI = 3.14159f;

const int WINDOW_POS_X = 50;
const int WINDOW_POS_Y = 50;

const int WINDOW_WIDTH  = 1600;
const int WINDOW_HEIGHT = 900;

const int MAX_UNITS = 50;
const int MAX_SPACE_UNITS = 10;

const int GRID_SIZE_X = 25;
const int GRID_SIZE_Y = 25;
const float  GRID_TILE_SIZE = 10.0f;

// Store the current game state
extern EGameStates gCurState;

// Global pointer to the engine. Can be used by anything including this file
extern I3DEngine*   gpEngine;
extern CRandomiser* gpRandomiser;


//-----------------------------------------------------
// TEMPLATE FUNCTIONS
//-----------------------------------------------------
// Function takes any type of pointer, removes it and sets it to null
template<class GenericPtr>
void SafeDelete(GenericPtr* inPtr)
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
