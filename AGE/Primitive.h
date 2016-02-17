#ifndef __AGE_PRIMITIVE_HEADER__
#define __AGE_PRIMITIVE_HEADER__

#include "Singleton.h"
#include "Renderable.h"

namespace AGE
{

	class AGE_EXPORT Primitive : public Singleton<Primitive>
	{
	public:

		static Mesh* CreateTorusUnmanage();

		static Mesh* CreateRectangleUnmanage(float width, float height);
	};
}

#endif