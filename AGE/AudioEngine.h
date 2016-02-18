#ifndef __AGE_AUDIOENGINE_HEADER__
#define __AGE_AUDIOENGINE_HEADER__

#include "Singleton.h"

#include "../3rd/include/yse/yse.hpp"
#include "SceneNode.h"
#include "Def.h"

namespace AGE{

	class AGE_EXPORT Sound3D
	{
	public:
		Sound3D();
		//Sound3D(const char* filename, bool loop, Attachable* parent);

		void SetParent(SceneNode* parent);
		void Load(const char* filename, bool loop);

		void Play() { mSound.play(); }
		void Stop() { mSound.stop(); }
		void Restart() { mSound.restart(); }

		bool IsPlaying() { return mSound.isPlaying(); }

		void Update();
	private:
		SceneNode* mAttachable;
		YSE::sound mSound;
	};

	class AGE_EXPORT AudioEngine :public Singleton < AudioEngine >
	{
	public:
		void StartUp();

		void ShutDown();

		void Update();

		void SetListenerPosition(const Vector3f &position);
		//Sound3D* CreateSound3D()
	};
}

#endif
