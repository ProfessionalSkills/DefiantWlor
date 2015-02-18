//-----------------------------------------------------
// FILE: Transport.h
//-----------------------------------------------------

#ifndef _TRANSPORT_H_
#define _TRANSPORT_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "SpaceUnit.h"


//-----------------------------------------------------
// TRANSPORTER CLASS - CHILD OF SPACE UNIT
//-----------------------------------------------------
class CTransport : public CSpaceUnit
{
private:
	// DATA
	//---------------------------


public:
	static IMesh* mspMshTransport;
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CTransport();
	virtual ~CTransport();


	// ACCESSORS
	//---------------------------



	// MUTATORS
	//---------------------------



	// METHODS
	//---------------------------


	// OVERRIDE METHODS
	//---------------------------
	bool Attack(CGameAgent* target, float hitMod, float damageMod);
	//virtual void MoveTo(CTile* dest);
	bool Move();
	void LoadModel(float x, float y, float z);


private:
	bool Destroy();
};

#endif /* _TRANSPORT_H_ */