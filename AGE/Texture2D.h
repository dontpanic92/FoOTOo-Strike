#ifndef __AGE_TEXTURE2D_HEADER__
#define __AGE_TEXTURE2D_HEADER__

#include <GL\glew.h>
#include <cstdint>

namespace AGE
{

#pragma pack(1)
	typedef struct
	{
		int8_t	identsize;              // Size of ID field that follows header (0)
		int8_t	colorMapType;           // 0 = None, 1 = paletted
		int8_t	imageType;              // 0 = none, 1 = indexed, 2 = rgb, 3 = grey, +8=rle
		int16_t	colorMapStart;          // First colour map entry
		int16_t	colorMapLength;         // Number of colors
		int8_t	colorMapBits;   // bits per palette entry
		int16_t	xstart;                 // image x origin
		int16_t	ystart;                 // image y origin
		int16_t	width;                  // width in pixels
		int16_t	height;                 // height in pixels
		int8_t	bits;                   // bits per pixel (8 16, 24, 32)
		int8_t	descriptor;             // image descriptor
	} TGAHEADER;
#pragma pack()

	class Texture2D
	{
	public:

		Texture2D();
		~Texture2D();

		bool Load(const char* path);

		GLuint GetTexture(){ return mTexture; }

	private:
		GLuint mTexture;

		GLint mWidth, mHeight, mComponents, mFormat;
	};
}

#endif