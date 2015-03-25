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
	CWall(bool horizontal);
	virtual ~CWall();


	// METHODS
	//---------------------------
	CWall* CreateWall(DX::XMFLOAT3 mPosition, bool horizontal, CRTSPlayer* pPlayer) override;
	void UnloadIModel() override;
	void LoadIModel() override;


private:


};

#endif /* _WALL_H_ */
