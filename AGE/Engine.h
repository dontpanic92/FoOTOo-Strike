#ifndef __AGE_ENGINE_HEADER__
#define __AGE_ENGINE_HEADER__
#include "Application.h"
#include "RenderEngine.h"
#include "Scene.h"
#include "Def.h"

namespace AGE{
	class AGE_EXPORT Engine : public Singleton < Engine >
	{
	public:
		~Engine();

		int StartUp(HWND hWnd = 0);

		void ShutDown();

		int Run();

		int Update();

		Window& GetMainWindow() { return mApp.GetMainWindow(); }

	private:
		Engine();

	private:
		Application mApp;
		bool mInitialized;

		float mLastTimeUpdate;

		friend class Singleton < Engine > ;
	};
}
#endif