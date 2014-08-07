#ifndef __AGE_PRIMITIVE_HEADER__
#define __AGE_PRIMITIVE_HEADER__

#include "Singleton.h"
#include "Mesh.h"

namespace AGE
{

	class Primitive : public Singleton<Primitive>
	{
	public:
		static Mesh* CreatePlaneUnmanage(Vector3f points[4]);
	};
}

#endif