#ifndef __AGE_DEF_HEADER__
#define __AGE_DEF_HEADER__

#ifdef AGE_SOURCE
#	define AGE_EXPORT  __declspec(dllexport)
#else
#	define AGE_EXPORT  __declspec(dllimport)
#endif

namespace AGE
{
	typedef unsigned int uint;
	typedef unsigned long ulong;
	typedef unsigned short ushort;
}

#endif
