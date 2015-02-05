//-----------------------------------------------------
// FILE: SoundObject.cpp
//-----------------------------------------------------

#ifndef SOUND_H
#define SOUND_H

//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Common.h"
#include <al.h>      // Main OpenAL functions
#include <alc.h>     // OpenAL "context" functions (also part of main OpenAL API)
#include <AL/alut.h> // OpenAL utility library - helper functions


//-----------------------------------------------------
//  CLASS FOR SOUNDS
//-----------------------------------------------------
class CSound
{
private:
	//Sound data
	//--------------------------- 
	ALuint mBuffer;

	//Sound source 
	//---------------------------
	ALuint mSource;

	//Source Position and Velocity 
	//---------------------------

	ALfloat sourcesPos[3];
	ALfloat sourceVel[3];

	//Listener Position, Velocity and Orientation
	//---------------------------

	ALfloat listenerPos[3];
	ALfloat listenerVel[3];
	ALfloat listenerOri[6];

	//Whether the sound is playing or not
	//---------------------------
	bool mIsPlaying;


public:

	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CSound(char* SoundFile, ALfloat mSourcePos[3], ALfloat mSourceVel[3]); 
	~CSound();


	void PlaySound();

};

#endif