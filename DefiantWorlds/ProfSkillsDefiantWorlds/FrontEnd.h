//-----------------------------------------------------
// FILE: FrontEnd.h
//-----------------------------------------------------

#ifndef _FRONT_END_H_
#define _FRONT_END_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Common.h"


//-----------------------------------------------------
// FRONT END CLASS
//-----------------------------------------------------
class CFrontEnd
{
public:
	CFrontEnd();
	~CFrontEnd();

	void FrontEndSetup();
	void FrontEndUpdate();
	void FrontEndCleanup();
};


#endif /* _FRONT_END_H_ */
