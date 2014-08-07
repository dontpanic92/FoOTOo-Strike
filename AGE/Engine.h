#ifndef __AGE_ENGINE_HEADER__
#define __AGE_ENGINE_HEADER__
#include "Application.h"
#include "OpenGLRenderer.h"
#include "Scene.h"

namespace AGE{
	class Engine : public Singleton<Engine>
	{
	public:
		~Engine();

		int StartUp();

		int Run();

		int Update();

	private:
		Engine();

	private:
		Application mApp;
		OpenGLRenderer* mRenderer;
		Scene * mScene;

		friend class Singleton<Engine>;
	};
}
#endif