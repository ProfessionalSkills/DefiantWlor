//-----------------------------------------------------
// FILE: BuildRequest.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "BuildRequest.h"


//-----------------------------------------------------
// BUILD REQUEST CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CBuildRequest::CBuildRequest(EQueueObjectType obj, int priority)
{
	mQueueObj = obj;
	mPriority = priority;
}

CBuildRequest::~CBuildRequest()
{

}
