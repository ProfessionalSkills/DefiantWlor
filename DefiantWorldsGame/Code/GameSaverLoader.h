//-----------------------------------------------------
// FILE: GameSaverLoader.h
//-----------------------------------------------------

#ifndef _GAME_SAVER_LOADER_H_
#define _GAME_SAVER_LOADER_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include <string>
#include <fstream>
#include <sstream>

#include "GameStateControl.h"


//-----------------------------------------------------
// GAME SAVER & LOADER CLASS
//-----------------------------------------------------
class CGameSaverLoader
{
public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CGameSaverLoader();
	~CGameSaverLoader();


	// METHODS
	//---------------------------
	void LoadGame(std::string& fileName);
	void SaveGame(std::string& fileName);


private:


};


#endif /* _GAME_SAVER_LOADER_H_ */
