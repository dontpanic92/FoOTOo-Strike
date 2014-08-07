#ifndef __AGE_WINDOW_HEADER__
#define __AGE_WINDOW_HEADER__
#include <Windows.h>

namespace AGE
{
	struct WindowsWindow
	{
		HWND hWnd;
		HDC hDC;
	};

	typedef WindowsWindow Window;
}

#endif