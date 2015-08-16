#ifndef __AGE_RENDERABLESCENEOBJECT_HEADER__
#define __AGE_RENDERABLESCENEOBJECT_HEADER__
#include "SceneObject.h"

namespace AGE
{
	class AGE_EXPORT RenderableSceneObject : public SceneObject
	{
	public:
		RenderableSceneObject()
		{
		}

		Renderable* GetRenderable() override { return mRenderable; }

	protected:

		Renderable* mRenderable = 0;
	};
}


#endif
