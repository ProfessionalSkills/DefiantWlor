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
	inline EQueueObjectType GetObjectType()
	{
		return mQueueObj;
	}

	inline int GetObjectPriority()
	{
		return mPriority;
	}

	inline int GetRejections()
	{
		return mNumRejections;
	}


	// MUTATORS
	//---------------------------
	inline void DecreasePriority()
	{
		mPriority += 5;
		mNumRejections++;
	}


	// METHODS
	//---------------------------


private:
	// DATA
	//---------------------------
	EQueueObjectType mQueueObj;
	int mPriority;
	int mNumRejections = 0;
};


#endif /* _BUILD_REQUEST_H_ */
