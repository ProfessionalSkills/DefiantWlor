//-----------------------------------------------------
// FILE: TransportBeam.h
//-----------------------------------------------------

#ifndef _TRANSPORT_BEAM_H_
#define _TRANSPORT_BEAM_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Common.h"


//-----------------------------------------------------
// TRANSPORT BEAM CLASS
//-----------------------------------------------------
class CTransportBeam
{
public:
	// STATIC MESHES
	//---------------------------
	static IMesh* mspMshBeam;


	// CONSTRUCTOR & DESTRUCTOR
	//---------------------------
	CTransportBeam(const DX::XMFLOAT3& position);
	~CTransportBeam();


	// METHODS
	//---------------------------
	bool Update();


private:
	// DATA
	//---------------------------
	IModel* mpMdlBeam = nullptr;
	float mCounter = 4.0f;
};


#endif /* _TRANSPORT_BEAM_H_ */
