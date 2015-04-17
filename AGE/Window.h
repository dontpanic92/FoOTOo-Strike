#ifndef __AGE_WINDOW_HEADER__
#define __AGE_WINDOW_HEADER__
#include <Windows.h>
#include "LinearMath.h"

namespace AGE
{
	struct WindowsWindow
	{
		HWND hWnd;
		HDC hDC;
		uint Height;
		uint Width;
	};

	typedef WindowsWindow Window;
}

#endif