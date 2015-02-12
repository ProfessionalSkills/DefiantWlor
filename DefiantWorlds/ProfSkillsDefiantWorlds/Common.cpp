//-----------------------------------------------------
// FILE: Common.cpp
//-----------------------------------------------------


//-----------------------------------------------------
// INCLUDES
//-----------------------------------------------------
#include "Common.h"


//-----------------------------------------------------
// DEFINES
//-----------------------------------------------------
// Actual initialisation of extern pointer variable.
// extern = will be defined elsewhere, but I want anything that includes this to be aware it
// will exist at some point.
I3DEngine*   gpEngine = New3DEngine(kTLX);
CRandomiser* gpRandomiser = new CRandomiser();
EGameStates  gCurState = GS_MAIN_MENU;
float        gFrameTime = 0.0f;

// Mouse scrolling detection variables - from Windows.h
HHOOK gMSScrollHook;
MSLLHOOKSTRUCT* gpMouseHookData;
short gMouseWheelDelta;


//-----------------------------------------------------
// FUNCTION DEFINITIONS
//-----------------------------------------------------
float Clampf(float inMin, float inMax, float inVal)
{
	if (inVal < inMin)
	{
		// Too small - return minimum
		return inMin;
	}
	else if (inVal > inMax)
	{
		// too great - return maximum
		return inMax;
	}

	// Values is fine
	return inVal;
}

float ToRadians(float inDegrees)
{
	return (DX::XM_PI / 180.0f) * inDegrees;
}

float ToDegrees(float inRadians)
{
	return (180.0f / DX::XM_PI) * inRadians;
}


//-----------------------------------------------------
// MOUSE HOOK DEFINITIONS
//-----------------------------------------------------
void SetMouseHook()
{
	// Link the hook with the callback function below
	//gMSScrollHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookCallback, NULL, 0);
}

LRESULT __stdcall MouseHookCallback(int code, WPARAM wParam, LPARAM lParam)
{
	// Check if a code has been entered
	if (code >= 0)
	{
		// Check if the mouse wheel has been used to scroll up
		if (wParam == WM_MOUSEWHEEL)
		{
			// Cast and store data stored within lParam
			gpMouseHookData = (MSLLHOOKSTRUCT*)lParam;
			gMouseWheelDelta = HIWORD(gpMouseHookData->mouseData);
		}
	}

	return CallNextHookEx(gMSScrollHook, code, wParam, lParam);
}

void ReleaseMouseHook()
{
	// Release the mouse hook
	//UnhookWindowsHookEx(gMSScrollHook);
}
