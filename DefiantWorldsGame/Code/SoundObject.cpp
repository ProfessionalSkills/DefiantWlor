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

CSound::CSound(ALuint buffer, XMFLOAT3 mSourcePos, XMFLOAT3 mSourceVel, bool loop, float gain,
	XMFLOAT3 mListenerPos, XMFLOAT3 mListenerVel)
{
	//Obtains sound file 
	mBuffer = buffer;
	
	//Generates sounds source
	alGenSources(1, &mSource);

	//sets up sound positions
	sourcesPos[0] = mSourcePos.x;
	sourcesPos[1] = mSourcePos.y;
	sourcesPos[2] = mSourcePos.z;

	sourceVel[0] = mSourceVel.x;
	sourceVel[1] = mSourceVel.y;
	sourceVel[2] = mSourceVel.z;

	listenerPos[0] = mListenerPos.x;
	listenerPos[1] = mListenerPos.y;
	listenerPos[2] = mListenerPos.z;

	listenerVel[0] = mListenerVel.x;
	listenerVel[1] = mListenerVel.y;
	listenerVel[2] = mListenerVel.z;



	// Set the properties of the source

	alSourcei(mSource, AL_BUFFER, mBuffer); //Attach buffer to source
	alSourcef(mSource, AL_PITCH, 1.0f);  //Set pitch
	alSourcef(mSource, AL_GAIN, gain);   //
	alSourcefv(mSource, AL_POSITION, sourcesPos); // Position of sound relative to listener 
	alSourcefv(mSource, AL_VELOCITY, sourceVel); // Velocity of sound relative to listener
	alSourcei(mSource, AL_LOOPING, loop);  // Whether to loop the sound or not


	//****************
	// Listener
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

	mIsPlaying = false;
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
	if (mIsPlaying) alSourceStop(mSource);

	alSourcePlay(mSource);
	mIsPlaying = true;
}

void CSound::StopSound()
{
	if (mIsPlaying)
	{
		alSourceStop(mSource);
		mIsPlaying = false;
	}
}