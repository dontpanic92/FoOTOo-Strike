#ifndef __AGE_D3D11DEF_HEADER__
#define __AGE_D3D11DEF_HEADER__

#define SafeRelease(x) { if(x){ x->Release(); x = 0; } }

#endif
