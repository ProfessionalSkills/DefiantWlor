//-----------------------------------------------------
// FILE: Wall.h
//-----------------------------------------------------

#ifndef _WALL_H_
#define _WALL_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "StaticStructure.h"


//-----------------------------------------------------
// WALL CLASS - CHILD OF STATIC STRUCTURE
//-----------------------------------------------------
class CWall : public CStaticStructure
{
public:
	static IMesh* mspMshStructure;


	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CWall();
	CWall(bool horizontal);
	virtual ~CWall();


	// METHODS
	//---------------------------
	void CalculateBoundingBox();
	void UnloadIModel() override;
	void LoadIModel() override;


private:
	bool mIsHorizontal = false;

};

#endif /* _WALL_H_ */
