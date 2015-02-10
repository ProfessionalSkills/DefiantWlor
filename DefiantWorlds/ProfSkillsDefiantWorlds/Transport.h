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
	virtual bool Attack(CGameAgent* target);
	//virtual void MoveTo(CTile* dest);
	virtual bool Move();

private:
	virtual bool Destroy();
};

#endif /* _TRANSPORT_H_ */