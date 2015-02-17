#include "OpenGLTexture.h"

using namespace AGE;

bool OpenGLTexture::Load(const char* path)
{
	Destroy();

	char* pBits = LoadTexture(path);

	// Set OpenGL format expected
	switch (mDepth) {
#ifndef OPENGL_ES
	case 3:     // Most likely case
		mFormat = GL_BGR;
		mComponents = GL_RGB;
		break;
#endif
	case 4:
		mFormat = GL_RGBA;
		mComponents = GL_RGBA;
		break;
	case 1:
		mFormat = GL_LUMINANCE;
		mComponents = GL_LUMINANCE;
		break;
	default:        // RGB
		// If on the iPhone, TGA's are BGR, and the iPhone does not 
		// support BGR without alpha, but it does support RGB,
		// so a simple swizzle of the red and blue bytes will suffice.
		// For faster iPhone loads however, save your TGA's with an Alpha!
#ifdef OPENGL_ES
		for (int i = 0; i < lImageSize; i += 3) {
			GLbyte temp = pBits[i];
			pBits[i] = pBits[i + 2];
			pBits[i + 2] = temp;
		}
#endif
		mFormat = GL_RGBA;
		mComponents = GL_RGBA;
		break;
	}

	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA, mWidth, mHeight, 0,
				 mFormat, GL_UNSIGNED_BYTE, pBits);

	delete[] pBits;

	return true;
}