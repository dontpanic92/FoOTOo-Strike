#ifndef __AGE_LIGHT_HEADER__
#define __AGE_LIGHT_HEADER__

#include "SceneObject.h"

namespace AGE {

	class Light : public SceneObject
	{
	public:
		enum class LightType { Directional };
		LightType	Type;
		Vector3f	Direction;
	};

}

#endif
