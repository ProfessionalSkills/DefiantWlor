//-----------------------------------------------------
// FILE: StaticStructure.h
//-----------------------------------------------------

#ifndef _STATIC_STRUCTURE_H_
#define _STATIC_STRUCTURE_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Structure.h"


//-----------------------------------------------------
// STATIC STRUCTURE CLASS - CHILD OF STRUCTURE
//-----------------------------------------------------
class CStaticStructure : public CStructure
{
private:
	// DATA
	//---------------------------


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CStaticStructure();
	virtual ~CStaticStructure();


	// ACCESSORS
	//---------------------------



	// MUTATORS
	//---------------------------



	// METHODS
	//---------------------------


	// OVERRIDE METHODS
	//---------------------------
	virtual void UnloadIModel();
	virtual void LoadIModel();
	//virtual bool Destroy();
};

#endif /* _STATIC_STRUCTURE_H_ */
