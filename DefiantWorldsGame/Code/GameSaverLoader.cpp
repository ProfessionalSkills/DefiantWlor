//-----------------------------------------------------
// FILE: GameSaverLoader.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "GameSaverLoader.h"


//-----------------------------------------------------
// GAME SAVER & LOADER CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CGameSaverLoader::CGameSaverLoader()
{

}

CGameSaverLoader::~CGameSaverLoader()
{

}


//-----------------------------------------------------
// GAME SAVER & LOADER CLASS METHODS
//-----------------------------------------------------
void CGameSaverLoader::LoadGame(std::string& fileName)
{

}

void CGameSaverLoader::SaveGame(std::string& fileName)
{
	// Concatenate file type onto end of file name & open the stream to this file
	stringstream saveFile;
	saveFile << "..\\Saves\\" << fileName << ".dws";
	ofstream saveStream(saveFile.str());

	// SAVE FILE STRUCTURE:
	// <Player>
	//     - Minerals, Faction, num spacefighters, num transport ships, num motherships
	// Grid
	//     - Line by line get state of grid (used or not)  -- ASSESS WHETHER REQUIRED
	// Structures
	//     - Type, Grid Position, Faction, State, World Position, Health, Queue items (Type, Progress), Rotation (currently just for wall data)
	// Ground/Air units
	//     - Type, Faction, State, Position, Health
	// Space Units
	//     - Type, Faction, State, Health, /* cargo (transport ships only) */ <- CHALLENGE
	// Resources
	//     - Position
	// Settings
	//     - Music vol, Effects vol, AI Difficulty, Starting Resources


	// FIRST PLAYER
	// Save player data
	CRTSPlayer* pPlayer = CStateControl::GetInstance()->GetPlayerManager()->GetHumanPlayer();
	pPlayer->SavePlayerData(saveStream);
}
