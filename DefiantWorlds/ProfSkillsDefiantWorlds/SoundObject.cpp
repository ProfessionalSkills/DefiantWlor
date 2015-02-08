//-----------------------------------------------------
// FILE: SoundObject.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "SoundObject.h"

using namespace std;
//-----------------------------------------------------
// SOUND CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------

CSound::CSound(string mSoundFile, ALfloat mSourcePos[3], ALfloat mSourceVel[3])
{
	//Start up OpenAL
	alutInit(0, 0);


	//Obtains sound file 
	mBuffer = alutCreateBufferFromFile("Intro.wav");

	//Generates sounds source
	alGenSources(1, &mSource);

	// Set the properties of the source

	alSourcei(mSource, AL_BUFFER, mBuffer); //Attach buffer to source
	alSourcef(mSource, AL_PITCH, 1.0f);  //Set pitch
	alSourcef(mSource, AL_GAIN, 0.1f);   //
	alSourcefv(mSource, AL_POSITION, mSourcePos); // Position of sound relative to listener 
	alSourcefv(mSource, AL_VELOCITY, mSourceVel); // Velocity of sound relative to listener
	alSourcei(mSource, AL_LOOPING, AL_TRUE);  // Whether to loop the sound or not


	//****************
	// Listener

	// Set the properties of the listener.
	// Position, velocity and orientation of listener
	alListenerfv(AL_POSITION, listenerPos); 
	alListenerfv(AL_VELOCITY, listenerVel); 
	alListenerfv(AL_ORIENTATION, listenerOri);
	alListenerf(AL_GAIN, 1.0f); //Master volume/gain

	bool mIsPlaying = false;

}

CSound::~CSound()
{
	if (mIsPlaying)
	{
		alSourceStop(mSource);
	}
}

void CSound::PlaySound()
{
	alSourcePlay(mSource);
}