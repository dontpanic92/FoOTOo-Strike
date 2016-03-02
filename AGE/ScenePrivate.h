#ifndef __AGE_SCENEPRIVATE_HEADER__
#define __AGE_SCENEPRIVATE_HEADER__
#include "Scene.h"
namespace AGE
{
	struct ScenePrivate
	{
		SceneNode root;
		vector<Light*> lights;

		Camera camera;
		SkyBox* skyBox = nullptr;
		std::wstring name;
	};
}

#endif
