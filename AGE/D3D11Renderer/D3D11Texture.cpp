#include "D3D11Texture.h"

using namespace AGE;

bool D3D11Texture::Load(const char* path)
{
	char* pBits = LoadTexture(path);

	D3D11_TEXTURE2D_DESC desc;
	memset(&desc, 0, sizeof(desc));

	switch (mDepth) {
	case 4:
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
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
	mDevice->CreateShaderResourceView(texture2d, 0, &TextureSRV);

	texture2d->Release();

	return true;

}


