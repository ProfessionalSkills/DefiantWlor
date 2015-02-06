//-----------------------------------------------------
// FILE: Hellipad.h
//-----------------------------------------------------

#ifndef _HELLIPAD_H_
#define _HELLIPAD_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "ProductionStructure.h"


//-----------------------------------------------------
// HELLIPAD CLASS - CHILD OF PRODUCTION STRUCTURE
//-----------------------------------------------------
class CHellipad : public CProductionStructure
{
private:
	// DATA
	//---------------------------


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CHellipad();
	virtual ~CHellipad();


	// ACCESSORS
	//---------------------------



	// MUTATORS
	//---------------------------



	// METHODS
	//---------------------------


	// OVERRIDE METHODS
	//---------------------------
private:
	bool Destroy();
};

#endif /* _HELLIPAD_H_ */
