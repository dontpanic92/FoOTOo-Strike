#ifndef __AGE_APPLICATION_HEADER__
#define __AGE_APPLICATION_HEADER__

#include "Window.h"
#include "Singleton.h"
#include <Windows.h>

namespace AGE
{

	class WindowsApplication
	{
	public:
		int MainLoop();

		int StartUp();

		Window GetMainWindow();

	protected:
		bool CreateApplicationWindow();

	protected:
		Window mWindow;
	};

	typedef WindowsApplication Application;
}

#endif