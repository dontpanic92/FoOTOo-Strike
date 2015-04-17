#ifndef __AGE_ENGINE_HEADER__
#define __AGE_ENGINE_HEADER__
#include "Application.h"
#include "RenderEngine.h"
#include "Scene.h"
#include "GameLogic.h"
#include "Def.h"

namespace AGE{
	class AGE_EXPORT Engine : public Singleton < Engine >
	{
	public:
		~Engine();

		int StartUp();

		void ShutDown();

		int Run();

		int Update();

		void SetGameLogic(GameLogic* logic);

		Scene* GetScene() { return mScene; }
		Window GetMainWindow() { return mApp.GetMainWindow(); }

	private:
		Engine();

	private:
		Application mApp;
		Scene * mScene;
		GameLogic* mGameLogic;

		float mLastTimeUpdate;

		friend class Singleton < Engine > ;
	};
}
#endif