//-----------------------------------------------------
// FILE: Barracks.h
//-----------------------------------------------------

#ifndef _BARRACKS_H_
#define _BARRACKS_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "ProductionStructure.h"


//-----------------------------------------------------
// BARRACKS CLASS - CHILD OF PRODUCTION STRUCTURE
//-----------------------------------------------------
class CBarracks : public CProductionStructure
{
private:
	// DATA
	//---------------------------


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CBarracks();
	virtual ~CBarracks();


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

#endif /* _BARRACKS_H_ */
