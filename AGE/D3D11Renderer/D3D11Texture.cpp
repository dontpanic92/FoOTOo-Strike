#include <D3DX11tex.h>
#include "D3D11Texture.h"

using namespace AGE;


void D3D11Texture::SetTextureSRV(ID3D11ShaderResourceView* view) 
{
	SafeRelease(mTextureSRV);
	mTextureSRV = view; 
	mTextureSRV->AddRef();
}

D3D11Texture2D::D3D11Texture2D(ID3D11Device* device) :D3D11Texture(device, TextureType::Texture2D)
{
}

bool D3D11Texture2D::Load(const char* path)
{
	SafeRelease(mTextureSRV);
	char* pBits = LoadTexture(path);

	if (!pBits)
		return false;

	D3D11_TEXTURE2D_DESC desc;
	memset(&desc, 0, sizeof(desc));

	switch (mDepth) {
	case 4:
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; //DXGI_FORMAT_R8G8B8A8_UNORM; //
		break;
	}
	ID3D11Texture2D* texture2d;
	desc.Height = mHeight;
	desc.Width = mWidth;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = pBits;
	data.SysMemPitch = mDepth * mWidth;

	mDevice->CreateTexture2D(&desc, &data, &texture2d);
	mDevice->CreateShaderResourceView(texture2d, 0, &mTextureSRV);

	texture2d->Release();

	delete[] pBits;
	return true;

}

D3D11CubeTexture::D3D11CubeTexture(ID3D11Device* device) :D3D11Texture(device, TextureType::CubeTexture)
{
}

bool D3D11CubeTexture::Load(const char* path)
{
	SafeRelease(mTextureSRV);
	if (FAILED(D3DX11CreateShaderResourceViewFromFileA(mDevice, path, 0, 0, &mTextureSRV, 0))) {
		return false;
	}
	return true;
}
