#include "Texture.h"
#include "Math.h"
#include <cstdio>
#include <cstdlib>
#include <ijg/jpeglib.h>
#include "Log.h"

using namespace AGE;

Texture2D::Texture2D(){}

Texture2D::~Texture2D() 
{
}


char* Texture2D::LoadTGA(const char* path)
//OpenGL SuperBible
{
	FILE *pFile;			// File pointer
	TGAHEADER tgaHeader;		// TGA file header
	unsigned long lImageSize;		// Size in bytes of image
	char* pBits;
	//short sDepth;			// Pixel depth;

	// Default/Failed values
	mWidth = 0;
	mHeight = 0;
	//mFormat = GL_RGB;
	//mComponents = GL_RGB;

	// Attempt to open the file
	pFile = fopen(path, "rb");
	if (pFile == NULL){
		Log::Error("Cannot Load TGA File");
		return NULL;
	}

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
	mDepth = tgaHeader.bits / 8;

	// Put some validity checks here. Very simply, I only understand
	// or care about 8, 24, or 32 bit targa's.
	if (tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
		return false;

	// Calculate size of image buffer
	lImageSize = tgaHeader.width * tgaHeader.height * mDepth;

	// Allocate memory and check for success
	pBits = new char[lImageSize];
	if (pBits == NULL)
		return NULL;

	// Read in the bits
	// Check for read error. This should catch RLE or other 
	// weird formats that I don't want to recognize
	if (fread(pBits, lImageSize, 1, pFile) != 1)
	{
		free(pBits);
		return NULL;
	}

	fclose(pFile);
	return pBits;
}

char* Texture2D::LoadJPEG(const char* path)
//ijg example
{
	struct jpeg_decompress_struct cinfo;

	FILE * infile;		/* source file */
	JSAMPARRAY buffer;		/* Output row buffer */
	int row_stride;		/* physical row width in output buffer */
	char* pBits;

	if ((infile = fopen(path, "rb")) == NULL) {
		fprintf(stderr, "can't open %s\n", path);
		return 0;
	}
	struct jpeg_error_mgr err;           //the error handler

	cinfo.err = jpeg_std_error(&err);
	jpeg_create_decompress(&cinfo);



	jpeg_stdio_src(&cinfo, infile);

	(void)jpeg_read_header(&cinfo, TRUE);


	(void)jpeg_start_decompress(&cinfo);

	mDepth = cinfo.num_components;
	uint x = cinfo.output_width;
	uint y = cinfo.output_height;


	mWidth = x;
	mHeight = y;


	ulong size = x * y * mDepth;

	pBits = new char[size];
	//row_stride = cinfo.output_width * cinfo.output_components;

	//buffer = (*cinfo.mem->alloc_sarray)
		//((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

	uint line = 0;
	char *p = pBits;
	char** p2 = &p;

	while (cinfo.output_scanline < cinfo.output_height) {
		/* jpeg_read_scanlines expects an array of pointers to scanlines.
		* Here the array is only one element long, but you could ask for
		* more than one scanline at a time if that's more convenient.
		*/
		line = jpeg_read_scanlines(&cinfo, (unsigned char**)p2, 1);
		/* Assume put_scanline_someplace wants a pointer and sample count. */
		p += line * x * mDepth;
	}

	(void)jpeg_finish_decompress(&cinfo);

	jpeg_destroy_decompress(&cinfo);

	fclose(infile);

	if (mDepth == 3) {
		mDepth = 4;
		unsigned int size = x * y * mDepth;
		char* pBits4 = new char[size];
		for (int i = 0; i < size / 4; i++) {
			pBits4[i * 4] = pBits[i * 3];
			pBits4[i * 4 + 1] = pBits[i * 3 + 1];
			pBits4[i * 4 + 2] = pBits[i * 3 + 2];
			pBits4[i * 4 + 3] = 1;
		}
		delete[] pBits;
		return pBits4;
	}
	return pBits;
}

char* Texture2D::LoadTexture(const char *path, Type fileType)
{
	char* pBits;
	if (fileType == AUTO)
	{
		uint len = strlen(path);
		if ((path[len - 1] == 'a' || path[len - 1] == 'A') 
			&& (path[len - 2] == 'g' || path[len - 2] == 'G') 
			&& (path[len - 3] == 't' || path[len - 3] == 'T'))
			fileType = TGA;
		else
			fileType = JPEG;
	}

	if (fileType == TGA)
	{
		pBits = LoadTGA(path);
	}
	else if (fileType == JPEG)
	{
		pBits = LoadJPEG(path);
	}

	return pBits;
}