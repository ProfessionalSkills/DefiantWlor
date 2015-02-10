//-----------------------------------------------------
// FILE: Transport.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Transport.h"


//-----------------------------------------------------
// TRANSPORT CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CTransport::CTransport()
{

}

CTransport::~CTransport()
{

}


//-----------------------------------------------------
// TRANSPORT CLASS OVERRIDE METHODS
//-----------------------------------------------------
bool CTransport::Attack(CGameAgent* target)
{
	return false;
}

//CTransport::void MoveTo(CTile* dest)


bool CTransport::Move()
{
	return false;
}

bool CTransport::Destroy()
{
	return false;
}
