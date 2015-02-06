//-----------------------------------------------------
// FILE: ComCentre.h
//-----------------------------------------------------

#ifndef _COM_CENTRE_H_
#define _COM_CENTRE_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "ProductionStructure.h"


//-----------------------------------------------------
// COM CENTRE CLASS - CHILD OF PRODUCTION STRUCTURE
//-----------------------------------------------------
class CComCentre : public CProductionStructure
{
private:
	// DATA
	//---------------------------


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CComCentre();
	virtual ~CComCentre();


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

#endif /* _COM_CENTRE_H_ */
