//-----------------------------------------------------
// FILE: NewsTicker.h
//-----------------------------------------------------

#ifndef _NEWS_TICKER_H_
#define _NEWS_TICKER_H_


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include <string>
#include <vector>
#include "Common.h"


//-----------------------------------------------------
// RANDOMISER CLASS
//-----------------------------------------------------
class CNewsTicker
{
private:
	// TEXTUAL ARRAYS
	//---------------------------
	std::vector<std::string> mNews;


	// FONT DATA
	//---------------------------
	IFont* mpFont;


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CNewsTicker();
	~CNewsTicker();



};


#endif /* _NEWS_TICKER_H_ */
