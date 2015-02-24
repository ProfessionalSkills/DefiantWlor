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
	static IMesh* mspMshStructureBuilt;
	static IMesh* mspMshStructurePlacing;


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
	void SetBuiltModel() override;
	void UnloadIModel() override;
	void LoadIModel()override;
};

#endif /* _SPACE_CENTRE_H_ */
