//-----------------------------------------------------
// FILE: SpaceCentre.h
//-----------------------------------------------------

#ifndef _SPACE_CENTRE_H_
#define _SPACE_CENTRE_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "ProductionStructure.h"


//-----------------------------------------------------
// SPACE CENTRE CLASS - CHILD OF PRODUCTION STRUCTURE
//-----------------------------------------------------
class CSpaceCentre : public CProductionStructure
{
private:
	// DATA
	//---------------------------


public:
	static IMesh* mspMshStructure;


	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CSpaceCentre();
	virtual ~CSpaceCentre();


	// ACCESSORS
	//---------------------------



	// MUTATORS
	//---------------------------



	// METHODS
	//---------------------------


	// OVERRIDE METHODS
	//---------------------------
	void UnloadIModel() override;
	bool Destroy() override;
};

#endif /* _SPACE_CENTRE_H_ */
