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
	static IMesh* mspMshStructureBuilt;
	static IMesh* mspMshStructurePlacing;


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
	void SetBuiltModel() override;
	void UnloadIModel() override;
	void LoadIModel() override;
};

#endif /* _BARRACKS_H_ */
