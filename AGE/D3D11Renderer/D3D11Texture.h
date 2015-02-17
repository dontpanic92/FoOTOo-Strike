#ifndef __AGE_D3D11TEXTURE_HEADER__
#define __AGE_D3D11TEXTURE_HEADER__

#include <D3D11.h>
#include "../Texture.h"

namespace AGE {

	class D3D11Texture : public Texture2D
	{
	public:

		ID3D11ShaderResourceView* TextureSRV;

		D3D11Texture(ID3D11Device* device) : mDevice(device) {}

		D3D11Texture(ID3D11Device* device, const char* path) : mDevice(device) { Load(path); }

		bool Load(const char* path) override;

	protected:
		ID3D11Device* mDevice;

	};
}

#endif