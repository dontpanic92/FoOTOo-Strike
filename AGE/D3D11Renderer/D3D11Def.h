#ifndef __AGE_D3D11DEF_HEADER__
#define __AGE_D3D11DEF_HEADER__

#include <dxerr.h>

#define SafeRelease(x) { if(x){ x->Release(); x = 0; } }

#if 1//defined(DEBUG) | defined(_DEBUG)
#ifndef TESTRESULT
#define TESTRESULT(x)                                              \
		{                                                          \
		HRESULT hr = (x);                                      \
		if(FAILED(hr))                                         \
				{                                                      \
			DXTrace(__FILE__, (DWORD)__LINE__, hr, L#x, true); \
				}                                                      \
		}
#endif

#else
#ifndef TESTRESULT
#define TESTRESULT(x) (x)
#endif
#endif 


#endif
