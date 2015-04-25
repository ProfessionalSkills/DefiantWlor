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
	int textSize = mpFont->MeasureTextHeight("T") + 1.0f;

	// Loop through news items & draw their text
	for (mriterNews = mNews.rbegin(); mriterNews != mNews.rend(); mriterNews++)
	{
		// If it's an error, draw it red. Otherwise draw it blue
		if ((*mriterNews)->mError)
		{
			mpFont->Draw((*mriterNews)->mNewsText, leftPos, drawHeight, 0xCCCC0000, kRight, kTop);
		}
		else
		{
			mpFont->Draw((*mriterNews)->mNewsText, leftPos, drawHeight, kCyan, kRight, kTop);
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
		(*miterNews)->mLifetime -= gFrameTime;
		if ((*miterNews)->mLifetime <= 0.0f)
		{
			delete (*miterNews);
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
		SafeDelete(pTmp);
		mNews.erase(miterNews);
	}

	// Create a new ticker item
	SNewsItem* pNewItem = new SNewsItem();
	pNewItem->mNewsText = elementText;
	pNewItem->mLifetime = mStartLifetime;
	pNewItem->mError = error;

	mNews.push_back(pNewItem);
}
