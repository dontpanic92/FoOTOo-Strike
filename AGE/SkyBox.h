#ifndef __AGE_SKYBOX_HEADER__
#define __AGE_SKYBOX_HEADER__

#include "Def.h"
#include "Renderable.h"

namespace AGE
{
	class AGE_EXPORT SkyBox
	{
	public:
		SkyBox();
		~SkyBox();

		Renderable* GetRenderable() { return mRenderable; }

	private:
		Renderable* mRenderable;
	};
}
#endif