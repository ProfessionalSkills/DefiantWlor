//-----------------------------------------------------
// FILE: Common.h
//-----------------------------------------------------

#ifndef _COMMON_H_
#define _COMMON_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include <TL-Engine.h>			// TL-Engine stuff
#include <vector>
#include <list>
#include <DirectXMath.h>		// Contains vector, matrices, etc. libraries 
								// Everything here is contained in the DirectX namespace (see below)
#include <string>

// Renamed the DirectX namespace so that DirectX:: is not required all the time. Simply use DX:: instead.
// e.g. Vector3 variable created by doing DX::XMFLOAT3 mPos; just as an example.
namespace DX = DirectX;

// If this is okay with you guys, I don't really like using this 'using namespace' as it's not
// professional coding conventions/standards. So, if possible, please no 'using namespace std'.
// if you do not like this I guess I can make an exception :P I will leave tle though as, to
// be honest, I have no idea what actually falls under this namespace.
using namespace tle;


//-----------------------------------------------------
// ENUMERATIONS
//-----------------------------------------------------
enum EGameStates
{
	GS_MAIN_MENU, GS_WORLD, GS_SPACE
};


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
// GLOBAL VARIABLES
//-----------------------------------------------------
// Global pointer to the engine. Can be used by anything including this file
extern I3DEngine* gpEngine;


#endif /* _COMMON_H_ */
