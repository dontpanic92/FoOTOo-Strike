#ifndef __AGE_PRIMITIVE_HEADER__
#define __AGE_PRIMITIVE_HEADER__

#include "Singleton.h"
#include "Renderable.h"

namespace AGE
{

	class Primitive : public Singleton<Primitive>
	{
	public:

		static Renderable* CreateTorusUnmanage();

		static Renderable* CreatePlaneUnmanage(Vector3f points[4]);
	};
}

#endif