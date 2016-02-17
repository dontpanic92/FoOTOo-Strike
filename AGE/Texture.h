#ifndef __AGE_TEXTURE2D_HEADER__
#define __AGE_TEXTURE2D_HEADER__

#include <GL/glew.h>
#include <cstdint>
#include "Def.h"

namespace AGE
{
	enum class TextureType
	{
		Texture2D, CubeTexture
	};

	class AGE_EXPORT Texture
	{
	public:

		Texture(TextureType type);

		virtual ~Texture();

		virtual bool Load(const char* path) = 0;

		TextureType GetType() { return mType; }

	protected:

		char* LoadTexture(const char* path);

		int mWidth, mHeight, mDepth;
		TextureType mType;
	};
}

#endif