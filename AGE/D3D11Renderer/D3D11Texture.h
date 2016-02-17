#ifndef __AGE_D3D11TEXTURE_HEADER__
#define __AGE_D3D11TEXTURE_HEADER__

#include <D3D11.h>
#include "../Texture.h"
#include "D3D11Def.h"

namespace AGE {

	class D3D11Texture : public Texture
	{
	public:

		D3D11Texture(ID3D11Device* device, TextureType type) : Texture(type), mDevice(device) {
		}
		~D3D11Texture() { SafeRelease(mTextureSRV); }

		ID3D11ShaderResourceView* GetTextureSRV() { return mTextureSRV; }
		void SetTextureSRV(ID3D11ShaderResourceView* view);

	protected:
		ID3D11Device* mDevice = nullptr;
		ID3D11ShaderResourceView* mTextureSRV = nullptr;
	};

	class D3D11Texture2D : public D3D11Texture
	{
	public:
		D3D11Texture2D(ID3D11Device* device);
		bool Load(const char* path) override;
	};

	class D3D11CubeTexture : public D3D11Texture
	{
	public:
		D3D11CubeTexture(ID3D11Device* device);
		bool Load(const char* path) override;
	};
}

#endif