#include "Texture.h"
#include "LinearMath.h"
#include <cstdio>
#include <cstdlib>
#include "Log.h"
#include <FreeImage/FreeImage.h>

using namespace AGE;

Texture2D::Texture2D(){}

Texture2D::~Texture2D() 
{
}


char* Texture2D::LoadTexture(const char *path)
{
	char* pBits = NULL;
	
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

	fif = FreeImage_GetFileType(path, 0);
	if (fif == FIF_UNKNOWN) {
		fif = FreeImage_GetFIFFromFilename(path);
	}
	if ((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif)) {
		FIBITMAP *dib = FreeImage_Load(fif, path, 0);
		FIBITMAP *temp = FreeImage_ConvertTo32Bits(dib);
		FreeImage_Unload(dib);
		dib = temp;
		
		BITMAPINFOHEADER* header = FreeImage_GetInfoHeader(dib);
		mDepth = header->biBitCount / 8;
		mHeight = header->biHeight;
		mWidth = header->biWidth;
		pBits = new char[FreeImage_GetDIBSize(dib)];
		memcpy(pBits, FreeImage_GetBits(dib), FreeImage_GetDIBSize(dib));

		FreeImage_Unload(dib);
	}
	
	return pBits;
}