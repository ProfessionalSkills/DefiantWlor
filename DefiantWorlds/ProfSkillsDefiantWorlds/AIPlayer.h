//-----------------------------------------------------
// FILE: AIPlayer.h
//-----------------------------------------------------

#ifndef _AI_PLAYER_H_
#define _AI_PLAYER_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "BasePlayer.h"


//-----------------------------------------------------
// AI PLAYER CLASS
//-----------------------------------------------------
class CAIPlayer : public CPlayer
{
private:


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CAIPlayer();
	virtual ~CAIPlayer();	


	// METHODS
	//---------------------------
	CStructure* CheckStructureSelection(DX::XMFLOAT3 pos);


	// OVERRIDE METHODS
	//---------------------------
	void Update() override;
};


#endif /* _AI_PLAYER_H_ */
