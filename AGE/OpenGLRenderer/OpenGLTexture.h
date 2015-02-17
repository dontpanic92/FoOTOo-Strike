#ifndef __AGE_OPENGLTEXTURE_HEADER__
#define __AGE_OPENGLTEXTURE_HEADER__

#include <GL/glew.h>
#include "../Texture.h"

namespace AGE {

	class OpenGLTexture : public Texture2D
	{
	public:
		GLuint mTexture;

		GLint mComponents, mFormat;

		OpenGLTexture() : mTexture(0) {}

		OpenGLTexture(const char* path) { Load(path); }

		~OpenGLTexture()
		{
			Destroy();
		}

		void Destroy()
		{
			if (mTexture)
				glDeleteTextures(1, &mTexture);
			mTexture = 0;
		}

		//void Use() const {  } 

		bool Load(const char* path) override;
	};
}

#endif