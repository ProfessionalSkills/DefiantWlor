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
	static IMesh* mspMshStructureBuilt;
	static IMesh* mspMshStructurePlacing;


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
	void SetBuiltModel() override;
	void UnloadIModel() override;
	void LoadIModel()override;
};

#endif /* _COM_CENTRE_H_ */
