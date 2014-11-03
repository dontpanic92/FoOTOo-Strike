#include "Texture2D.h"
#include "Math.h"
#include <cstdio>
#include <cstdlib>
#include <ijg/jpeglib.h>
#include "Log.h"

using namespace AGE;

Texture2D::Texture2D():mTexture(0){}

Texture2D::~Texture2D(){
	if(mTexture)
		glDeleteTextures(1, &mTexture);
}


bool Texture2D::LoadTGA(const char* path)
//OpenGL SuperBible
{
	FILE *pFile;			// File pointer
	TGAHEADER tgaHeader;		// TGA file header
	unsigned long lImageSize;		// Size in bytes of image
	short sDepth;			// Pixel depth;

	// Default/Failed values
	mWidth = 0;
	mHeight = 0;
	mFormat = GL_RGB;
	mComponents = GL_RGB;

	// Attempt to open the file
	pFile = fopen(path, "rb");
	if (pFile == NULL){
		Log::Error("Cannot Load TGA File");
		return false;
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
	sDepth = tgaHeader.bits / 8;

	// Put some validity checks here. Very simply, I only understand
	// or care about 8, 24, or 32 bit targa's.
	if (tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
		return false;

	// Calculate size of image buffer
	lImageSize = tgaHeader.width * tgaHeader.height * sDepth;

	// Allocate memory and check for success
	pBits = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));
	if (pBits == NULL)
		return false;

	// Read in the bits
	// Check for read error. This should catch RLE or other 
	// weird formats that I don't want to recognize
	if (fread(pBits, lImageSize, 1, pFile) != 1)
	{
		free(pBits);
		return false;
	}

	// Set OpenGL format expected
	switch (sDepth)
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
		for (int i = 0; i < lImageSize; i += 3)
		{
			GLbyte temp = pBits[i];
			pBits[i] = pBits[i + 2];
			pBits[i + 2] = temp;
		}
#endif
		break;
	}

	fclose(pFile);

	return true;
}

bool Texture2D::LoadJPEG(const char* path)
//ijg example
{
	struct jpeg_decompress_struct cinfo;

	FILE * infile;		/* source file */
	JSAMPARRAY buffer;		/* Output row buffer */
	int row_stride;		/* physical row width in output buffer */

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


	uint x = cinfo.output_width;
	uint y = cinfo.output_height;
	uint channels = cinfo.num_components;

	mWidth = x;
	mHeight = y;

	//mFormat = GL_BGR;
	mFormat = GL_RGB;
	mComponents = GL_RGB;

	if (channels == 4){
		//mFormat = GL_BGRA;
		mFormat = GL_RGBA;
		mComponents = GL_RGBA;
	}

	ulong size = x * y * channels;

	pBits = new GLbyte[size];

	row_stride = cinfo.output_width * cinfo.output_components;

	//buffer = (*cinfo.mem->alloc_sarray)
		//((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

	uint line = 0;
	GLbyte *p = pBits;
	GLbyte** p2 = &p;

	while (cinfo.output_scanline < cinfo.output_height) {
		/* jpeg_read_scanlines expects an array of pointers to scanlines.
		* Here the array is only one element long, but you could ask for
		* more than one scanline at a time if that's more convenient.
		*/
		line = jpeg_read_scanlines(&cinfo, (unsigned char**)p2, 1);
		/* Assume put_scanline_someplace wants a pointer and sample count. */
		p += line * x * channels;
	}

	(void)jpeg_finish_decompress(&cinfo);

	jpeg_destroy_decompress(&cinfo);

	fclose(infile);

	return true;
}

bool Texture2D::Load(const char *path, Type fileType)
{
	bool ret;
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
		ret = LoadTGA(path);
	}
	else if (fileType == JPEG)
	{
		ret = LoadJPEG(path);
	}

	if (!ret)
		return false;

	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB, mWidth, mHeight, 0,
		mFormat, GL_UNSIGNED_BYTE, pBits);

	free(pBits);

	return true;
}