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
	static IMesh* mspMshStructure;


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
	void UnloadIModel() override;
	bool Destroy() override;
};

#endif /* _BARRACKS_H_ */
