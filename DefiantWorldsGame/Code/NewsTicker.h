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
#include <TL-Engine.h>
using namespace tle;


//-----------------------------------------------------
// NEWS ITEM STRUCT
//-----------------------------------------------------
struct SNewsItem
{
	std::string mNewsText;
	float mLifetime;
	bool mError;
	ISprite* mpSprBG = nullptr;
};


//-----------------------------------------------------
// NEWS TICKER CLASS CLASS
//-----------------------------------------------------
class CNewsTicker
{
private:
	// TEXTUAL ARRAYS
	//---------------------------
	std::vector<SNewsItem*> mNews;
	std::vector<SNewsItem*>::iterator miterNews;
	std::vector<SNewsItem*>::reverse_iterator mriterNews;


	// NEWS ITEM DATA
	//---------------------------
	float mStartLifetime;
	int mMaxNumEls;
	IFont* mpFont;


public:
	// CONSTRUCTORS & DESTRUCTOR
	//---------------------------
	CNewsTicker();
	~CNewsTicker();


	// METHODS
	//---------------------------
	void Display();
	void UpdateTimers();
	void AddNewElement(std::string elementText, bool error);
};


#endif /* _NEWS_TICKER_H_ */
