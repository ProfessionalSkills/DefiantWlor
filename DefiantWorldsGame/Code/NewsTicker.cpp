//-----------------------------------------------------
// FILE: NewsTicker.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "NewsTicker.h"
#include "Common.h"


//-----------------------------------------------------
// NEWS TICKER CLASS CONSTRUCTORS & DESTRUCTOR
//-----------------------------------------------------
CNewsTicker::CNewsTicker() : mStartLifetime(8.0f), mMaxNumEls(10)
{
	// Create font
	mpFont = gpEngine->LoadFont("Calibri", 21U);
}

CNewsTicker::~CNewsTicker()
{
	// Remove all news items
	while (!mNews.empty())
	{
		SNewsItem* pItem = mNews.back();
		SafeDelete(pItem);
		mNews.pop_back();
	}

	// Unload font
	gpEngine->RemoveFont(mpFont);
}


//-----------------------------------------------------
// NEWS TICKER CLASS METHODS
//-----------------------------------------------------
void CNewsTicker::Display()
{
	// Variables for deciding where to draw next element
	int drawHeight = 10;
	int leftPos = 1580;
	int textSize = 30.0f;

	// Loop through news items & draw their text
	for (mriterNews = mNews.rbegin(); mriterNews != mNews.rend(); mriterNews++)
	{
		// Cache news item
		SNewsItem* pNews = (*mriterNews);
		
		// Set position of the sprite
		pNews->mpSprBG->SetX(1150.0f);
		pNews->mpSprBG->SetY(drawHeight);

		// If it's an error, draw it red. Otherwise draw it blue
		if (pNews->mError)
		{
			mpFont->Draw(pNews->mNewsText, leftPos, drawHeight + 4, 0xCCCC0000, kRight, kTop);
		}
		else
		{
			mpFont->Draw(pNews->mNewsText, leftPos, drawHeight + 4, kCyan, kRight, kTop);
		}

		// Decrement draw height by text size amount
		drawHeight += textSize;
	}
}

void CNewsTicker::UpdateTimers()
{
	// Loop through news items and decrement their lifetime
	for (miterNews = mNews.begin(); miterNews != mNews.end(); miterNews++)
	{
		// Cache pointer instead of working it out all the time
		SNewsItem* pNews = (*miterNews);
		pNews->mLifetime -= gFrameTime;
		if (pNews->mLifetime <= 0.0f)
		{
			// Unload sprite & delete news item
			gpEngine->RemoveSprite(pNews->mpSprBG);
			delete pNews;
			mNews.erase(miterNews);
			break;
		}
	}

	// Display remaining news items
	Display();
}

void CNewsTicker::AddNewElement(std::string elementText, bool error)
{
	// Check if the max has already been reached
	if (mNews.size() >= 10)
	{
		// Remove the oldest element
		miterNews = mNews.begin();
		SNewsItem* pTmp = mNews.front();
		gpEngine->RemoveSprite(pTmp->mpSprBG);
		SafeDelete(pTmp);
		mNews.erase(miterNews);
	}

	// Create a new ticker item
	SNewsItem* pNewItem = new SNewsItem();
	pNewItem->mNewsText = elementText;
	pNewItem->mLifetime = mStartLifetime;
	pNewItem->mError = error;

	// Check whether it is an erro
	if (error)
	{
		pNewItem->mpSprBG = gpEngine->CreateSprite("NewsTickerUIError.png");
	}
	else
	{
		pNewItem->mpSprBG = gpEngine->CreateSprite("NewsTickerUI.png");
	}

	mNews.push_back(pNewItem);
}
