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

		bool IsWindow;

		void Set(HWND h, bool isWindow)
		{
			hWnd = h;
			hDC = GetWindowDC(hWnd);
			RECT rc;
			GetClientRect(hWnd, &rc);

			Width = rc.right - rc.left;
			Height = rc.bottom - rc.top;

			IsWindow = isWindow;
		}
	};

	typedef WindowsWindow Window;
}

#endif