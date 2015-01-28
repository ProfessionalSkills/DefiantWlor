//-----------------------------------------------------
// FILE: Common.h
//-----------------------------------------------------

#ifndef _COMMON_H_
#define _COMMON_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include <TL-Engine.h>			// TL-Engine stuff
#include <DirectXMath.h>		// Contains vector, matrices, etc. libraries 
								// Everything here is contained in the DirectX namespace (see below)

// Renamed the DirectX namespace so that DirectX:: is not required all the time. Simply use DX:: instead.
// e.g. Vector3 variable created by doing DX::XMFLOAT3 mPos; just as an example.
namespace DX = DirectX;

// If this is okay with you guys, I don't really like using this 'using namespace' as it's not
// professional coding conventions/standards. So, if possible, please no 'using namespace std'.
// if you do not like this I guess I can make an exception :P I will leave tle though as, to
// be honest, I have no idea what falls under this namespace.
using namespace tle;


//-----------------------------------------------------
// MACROS
//-----------------------------------------------------
// SAFE_DELETE macro
// Takes a pointer to an object, checks if exists, deallocates memory, sets to 0;
#define SAFE_DELETE(ptr) \
{ \
	if (ptr) \
	{ \
		DELETE(ptr); \
		(ptr) = nullptr; \
	} \
}


//-----------------------------------------------------
// GLOBAL VARIABLES
//-----------------------------------------------------
// Global pointer to the engine. Can be used by anything including this file
extern I3DEngine* gpEngine;



#endif /* _COMMON_H_ */
