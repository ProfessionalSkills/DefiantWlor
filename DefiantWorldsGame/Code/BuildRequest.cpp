//-----------------------------------------------------
// FILE: BuildRequest.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "BuildRequest.h"

CBuildRequest::CBuildRequest(EQueueObjectType obj, int priority)
{
	mQueueObj = obj;
	mPriority = priority;
}

CBuildRequest::~CBuildRequest()
{

}
