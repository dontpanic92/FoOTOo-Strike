#ifndef __AGE_LIGHT_HEADER__
#define __AGE_LIGHT_HEADER__

#include "Attachable.h"

namespace AGE {
	class SceneObject : public Attachable
	{
	};


	class Light : public SceneObject
	{
	public:
		enum class LightType { Directional };
		LightType	Type;
		Vector3f	Direction;
	};

}

#endif
