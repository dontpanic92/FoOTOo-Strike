#ifndef __AGE_APPLICATION_HEADER__
#define __AGE_APPLICATION_HEADER__

#include "Window.h"
#include "Singleton.h"
#include <Windows.h>
#include "Def.h"

namespace AGE
{

	class AGE_EXPORT WindowsApplication
	{
	public:
		int MainLoop();

		int StartUp(HWND hWnd = 0);

		void ShutDown();

		Window& GetMainWindow();

	protected:
		bool CreateApplicationWindow();

	protected:
		Window mWindow;
	};

	typedef WindowsApplication Application;
}

#endif