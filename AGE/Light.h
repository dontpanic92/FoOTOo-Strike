#ifndef __AGE_LIGHT_HEADER__
#define __AGE_LIGHT_HEADER__

#include "SceneNode.h"

namespace AGE {

	class Light : public SceneNode
	{
	public:
		enum class LightType { Directional };
		LightType	Type;
		Vector3f	Direction;
	};

}

#endif
