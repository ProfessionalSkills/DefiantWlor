//-----------------------------------------------------
// FILE: Randomiser.h
//-----------------------------------------------------

#ifndef _RANDOMISER_H_
#define _RANDOMISER_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include <random>
#include <chrono>


//-----------------------------------------------------
// RANDOMISER CLASS
//-----------------------------------------------------
class CRandomiser
{
private:
	// RANDOMISERS
	//--------------------------- 
	std::default_random_engine mGenerator;


public:
	// CONSTRUCTORS & DESTRUCTOR
	//--------------------------- 
	CRandomiser();
	~CRandomiser();


	// METHODS
	//--------------------------- 
	int GetRandomInt(int inMin, int inMax);
	float GetRandomFloat(double inMin, double inMax);
};


#endif /* _RANDOMISER_H_ */
