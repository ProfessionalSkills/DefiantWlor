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


	// OVERRIDE METHODS
	//---------------------------
	void Update(float delta) override;
};


#endif /* _HUMAN_PLAYER_H_ */
