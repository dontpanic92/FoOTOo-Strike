#include "D3D11RenderTarget.h"

using namespace AGE;

D3D11ShadowMapTarget::D3D11ShadowMapTarget(ID3D11Device* device, uint width, uint height)
	: D3D11RenderTarget(device), mWidth(width), mHeight(height)
{
	mViewport.TopLeftX = 0;
	mViewport.TopLeftY = 0;
	mViewport.Width = width;
	mViewport.Height = height;
	mViewport.MinDepth = 0;
	mViewport.MaxDepth = 1;

	//Create texture for stencil
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = mWidth;
	texDesc.Height = mHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	TESTRESULT(device->CreateTexture2D(&texDesc, 0, &mDepthMap));

	//Create stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(dsvDesc));
	dsvDesc.Flags = 0;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	TESTRESULT(device->CreateDepthStencilView(mDepthMap, &dsvDesc, &mDepthMapDSV));

	//Create srv for stencil texture
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	TESTRESULT(device->CreateShaderResourceView(mDepthMap, &srvDesc, &mDepthMapSRV));

	//Create texture for render target
	D3D11_TEXTURE2D_DESC texDesc2;
	ZeroMemory(&texDesc2, sizeof(texDesc2));
	texDesc2.Width = mWidth;
	texDesc2.Height = mHeight;
	texDesc2.MipLevels = 1;
	texDesc2.ArraySize = 1;
	texDesc2.Format = DXGI_FORMAT_R32G32_FLOAT;
	texDesc2.SampleDesc.Count = 1;
	texDesc2.SampleDesc.Quality = 0;
	texDesc2.Usage = D3D11_USAGE_DEFAULT;
	texDesc2.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc2.CPUAccessFlags = 0;
	texDesc2.MiscFlags = 0;

	TESTRESULT(device->CreateTexture2D(&texDesc2, 0, &mRenderTargetMap));

	
	//Create render target view
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
	renderTargetViewDesc.Format = texDesc2.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	TESTRESULT(device->CreateRenderTargetView(mRenderTargetMap, &renderTargetViewDesc, &mRenderTargetView));

	//Create srv for render target texture
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc2;
	ZeroMemory(&srvDesc2, sizeof(srvDesc2));
	srvDesc2.Format = DXGI_FORMAT_R32G32_FLOAT;
	srvDesc2.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc2.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc2.Texture2D.MostDetailedMip = 0;
	TESTRESULT(device->CreateShaderResourceView(mRenderTargetMap, &srvDesc2, &mRenderTargetMapSRV));

}

D3D11ShadowMapTarget::~D3D11ShadowMapTarget()
{
	SafeRelease(mDepthMap);
	SafeRelease(mDepthMapSRV);
	SafeRelease(mDepthMapDSV);
	SafeRelease(mRenderTargetMap);
	SafeRelease(mRenderTargetMapSRV);
	SafeRelease(mRenderTargetView);
}

void D3D11ShadowMapTarget::SetRenderTarget(ID3D11DeviceContext* dc)
{
	dc->RSSetViewports(1, &mViewport);
	ID3D11RenderTargetView* renderTargets[1] = { mRenderTargetView };
	
	dc->OMSetRenderTargets(1, renderTargets, mDepthMapDSV);
	dc->ClearDepthStencilView(mDepthMapDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

D3D11NormalTarget::D3D11NormalTarget(ID3D11Device* device, IDXGISwapChain* swapChain, uint width, uint height)
	: D3D11RenderTarget(device), mWidth(width), mHeight(height)
{
	ID3D11Texture2D* backBuffer;
	(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	(device->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView));
	SafeRelease(backBuffer);

	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = mWidth;
	depthStencilDesc.Height = mHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// Use 4X MSAA? --must match swap chain MSAA values.
	//if (mEnable4xMsaa) {
	//	depthStencilDesc.SampleDesc.Count = 4;
	//	depthStencilDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
	//}
	// No MSAA
	//else {
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	//}

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	(mDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer));
	(mDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView));

	mViewport.TopLeftX = 0;
	mViewport.TopLeftY = 0;
	mViewport.Width = static_cast<float>(mWidth);
	mViewport.Height = static_cast<float>(mHeight);
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;
}

D3D11NormalTarget::~D3D11NormalTarget()
{
	SafeRelease(mRenderTargetView);
	SafeRelease(mDepthStencilView);
}

void D3D11NormalTarget::SetRenderTarget(ID3D11DeviceContext* dc)
{
	ID3D11RenderTargetView* renderTargets[1] = { mRenderTargetView };
	dc->OMSetRenderTargets(1, renderTargets, mDepthStencilView);
	dc->RSSetViewports(1, &mViewport);
}

void D3D11NormalTarget::ClearView(ID3D11DeviceContext* dc)
{
	static float BackColor[] = { 0.1, 0.1, 0.1, 1.0 };

	dc->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(BackColor));
	dc->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
