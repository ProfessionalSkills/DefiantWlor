
#ifndef _EXPLOSION_H
#define _EXPLOSION_H

#include "ParticleEmitter.h"

class CExplosion : public CParticleEmitter
{
protected:
	// EXPLOSION DATA
	//---------------------------

	//EMITTER MODEL SOURCE

public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CExplosion();
	~CExplosion();


	// VIRTUAL METHODS
	//---------------------------
	virtual void UpdateSystem();
};

#endif
