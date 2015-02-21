//-----------------------------------------------------
// FILE: HumanPlayer.h
//-----------------------------------------------------

#ifndef _HUMAN_PLAYER_H_
#define _HUMAN_PLAYER_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "BasePlayer.h"


//-----------------------------------------------------
// HUMAN PLAYER CLASS
//-----------------------------------------------------
class CHumanPlayer : public CPlayer
{
private:


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CHumanPlayer();
	virtual ~CHumanPlayer();


	// METHODS
	//---------------------------
	CStructure* CheckStructureSelection(DX::XMFLOAT3 pos);


	// OVERRIDE METHODS
	//---------------------------
	void Update() override;
};


#endif /* _HUMAN_PLAYER_H_ */
