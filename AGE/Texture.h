#ifndef __AGE_TEXTURE2D_HEADER__
#define __AGE_TEXTURE2D_HEADER__

#include <GL/glew.h>
#include <cstdint>
#include "Def.h"

namespace AGE
{

	class AGE_EXPORT Texture2D
	{
	public:

		Texture2D();

		virtual ~Texture2D();

		virtual bool Load(const char* path) = 0;

	protected:

		char* LoadTexture(const char* path);

		int mWidth, mHeight, mDepth;
	};
}

#endif