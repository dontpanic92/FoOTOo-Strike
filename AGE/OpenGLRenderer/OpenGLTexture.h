#ifndef __AGE_OPENGLTEXTURE_HEADER__
#define __AGE_OPENGLTEXTURE_HEADER__

#include <GL/glew.h>
#include "../Texture.h"

namespace AGE {

	class OpenGLTexture : public Texture
	{
	public:
		GLuint mTexture;

		GLint mComponents, mFormat;

		OpenGLTexture() : Texture(TextureType::Texture2D), mTexture(0) {}

		OpenGLTexture(const char* path) :Texture(TextureType::Texture2D) { Load(path); }

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