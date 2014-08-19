#include "Texture2D.h"
#include <cstdio>
#include <cstdlib>

using namespace AGE;

Texture2D::Texture2D():mTexture(0){}

Texture2D::~Texture2D(){
	if(mTexture)
		glDeleteTextures(1, &mTexture);
}

bool Texture2D::Load(const char *path)
	//OpenGL SuperBible
{
	FILE *pFile;			// File pointer
	TGAHEADER tgaHeader;		// TGA file header
	unsigned long lImageSize;		// Size in bytes of image
	short sDepth;			// Pixel depth;
	GLbyte* pBits;

	// Default/Failed values
	mWidth = 0;
	mHeight = 0;
	mFormat = GL_RGB;
	mComponents = GL_RGB;

	// Attempt to open the file
	pFile = fopen(path, "rb");
	if(pFile == NULL)
		return false;

	// Read in header (binary)
	fread(&tgaHeader, 18/* sizeof(TGAHEADER)*/, 1, pFile);

	// Do byte swap for big vs little endian
#ifdef __APPLE__
	LITTLE_ENDIAN_WORD(&tgaHeader.colorMapStart);
	LITTLE_ENDIAN_WORD(&tgaHeader.colorMapLength);
	LITTLE_ENDIAN_WORD(&tgaHeader.xstart);
	LITTLE_ENDIAN_WORD(&tgaHeader.ystart);
	LITTLE_ENDIAN_WORD(&tgaHeader.width);
	LITTLE_ENDIAN_WORD(&tgaHeader.height);
#endif


	// Get width, height, and depth of texture
	mWidth = tgaHeader.width;
	mHeight = tgaHeader.height;
	sDepth = tgaHeader.bits / 8;

	// Put some validity checks here. Very simply, I only understand
	// or care about 8, 24, or 32 bit targa's.
	if(tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
		return false;

	// Calculate size of image buffer
	lImageSize = tgaHeader.width * tgaHeader.height * sDepth;

	// Allocate memory and check for success
	pBits = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));
	if(pBits == NULL)
		return false;

	// Read in the bits
	// Check for read error. This should catch RLE or other 
	// weird formats that I don't want to recognize
	if(fread(pBits, lImageSize, 1, pFile) != 1)
	{
		free(pBits);
		return false;
	}

	// Set OpenGL format expected
	switch(sDepth)
	{
#ifndef OPENGL_ES
	case 3:     // Most likely case
		mFormat = GL_BGR;
		mComponents = GL_RGB;
		break;
#endif
	case 4:
		mFormat = GL_BGRA;
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
		for(int i = 0; i < lImageSize; i+=3)
		{
			GLbyte temp = pBits[i];
			pBits[i] = pBits[i+2];
			pBits[i+2] = temp;
		}
#endif
		break;
	}



	// Done with File
	fclose(pFile);

	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB, mWidth, mHeight, 0,
				 mFormat, GL_UNSIGNED_BYTE, pBits);

	free(pBits);
	// Return pointer to image data
	return true;
}