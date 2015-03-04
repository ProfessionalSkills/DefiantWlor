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

CSound::CSound(string mSoundFile, ALfloat mSourcePos[3], ALfloat mSourceVel[3], bool loop,float gain)
{
	//Start up OpenAL
	alutInit(0, 0);

	//Obtains sound file 
	mBuffer = alutCreateBufferFromFile(mSoundFile.c_str());
	
	//Generates sounds source
	alGenSources(1, &mSource);

	// Set the properties of the source

	alSourcei(mSource, AL_BUFFER, mBuffer); //Attach buffer to source
	alSourcef(mSource, AL_PITCH, 1.0f);  //Set pitch
	alSourcef(mSource, AL_GAIN, gain);   //
	alSourcefv(mSource, AL_POSITION, mSourcePos); // Position of sound relative to listener 
	alSourcefv(mSource, AL_VELOCITY, mSourceVel); // Velocity of sound relative to listener
	alSourcei(mSource, AL_LOOPING, loop);  // Whether to loop the sound or not


	//****************
	// Listener
	listenerPos[0] = mSourcePos[0];
	listenerPos[1] = mSourcePos[1];
	listenerPos[2] = mSourcePos[2];

	listenerVel[0] = 0.0f;
	listenerVel[1] = 0.0f;
	listenerVel[2] = 0.0f;

	listenerOri[0] = 0.0f;
	listenerOri[1] = 0.0f;
	listenerOri[2] = 0.0f;
	listenerOri[3] = 0.0f;
	listenerOri[4] = 0.0f;
	listenerOri[5] = 0.0f;


	// Set the properties of the listener.
	// Position, velocity and orientation of listener
	alListenerfv(AL_POSITION, listenerPos); 
	alListenerfv(AL_VELOCITY, listenerVel); 
	alListenerfv(AL_ORIENTATION, listenerOri);
	alListenerf(AL_GAIN, gain); //Master volume/gain

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

void CSound::StopSound()
{
	alSourceStop(mSource);
}