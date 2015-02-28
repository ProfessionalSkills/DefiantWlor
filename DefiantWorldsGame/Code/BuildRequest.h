//-----------------------------------------------------
// FILE: BuildRequest.h
//-----------------------------------------------------

#ifndef _BUILD_REQUEST_H_
#define _BUILD_REQUEST_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Common.h"


//-----------------------------------------------------
// BUILD REQUEST CLASS
//-----------------------------------------------------
class CBuildRequest
{
public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CBuildRequest(EQueueObjectType obj, int priority);
	~CBuildRequest();


	// ACCESSORS
	//---------------------------
	inline unsigned int GetObjectType()
	{
		return mQueueObj & 0xFF00;
	}

	inline int GetObjectPriority()
	{
		return mPriority;
	}


	// MUTATORS
	//---------------------------


	// METHODS
	//---------------------------


private:
	// DATA
	//---------------------------
	EQueueObjectType mQueueObj;
	int mPriority;
};


#endif /* _BUILD_REQUEST_H_ */
