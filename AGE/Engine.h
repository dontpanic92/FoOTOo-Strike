#ifndef __AGE_ENGINE_HEADER__
#define __AGE_ENGINE_HEADER__
#include "Application.h"
#include "OpenGLRenderer.h"
#include "Scene.h"
#include "GameLogic.h"

namespace AGE{
	class Engine : public Singleton<Engine>
	{
	public:
		~Engine();

		int StartUp();

		int Run();

		int Update();

		Scene* GetScene(){ return mScene; }
		Window GetMainWindow() { return mApp.GetMainWindow(); }

	private:
		Engine();

	private:
		Application mApp;
		OpenGLRenderer* mRenderer;
		Scene * mScene;
		GameLogicImp* mGameLogic;

		float mLastTimeUpdate;

		friend class Singleton<Engine>;
	};
}
#endif