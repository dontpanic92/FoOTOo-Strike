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

	D3D11_TEXTURE2D_DESC texDesc;
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

	mDepthMap = 0;
	TESTRESULT(device->CreateTexture2D(&texDesc, 0, &mDepthMap));

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = 0;
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;
	TESTRESULT(device->CreateDepthStencilView(mDepthMap, &dsvDesc, &mDepthMapDSV));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	TESTRESULT(device->CreateShaderResourceView(mDepthMap, &srvDesc, &mDepthMapSRV));

	// View saves a reference to the texture so we can release our reference.
	
}

D3D11ShadowMapTarget::~D3D11ShadowMapTarget()
{
	SafeRelease(mDepthMap);
	SafeRelease(mDepthMapSRV);
	SafeRelease(mDepthMapDSV);
}

void D3D11ShadowMapTarget::SetRenderTarget(ID3D11DeviceContext* dc)
{
	dc->RSSetViewports(1, &mViewport);
	ID3D11RenderTargetView* renderTargets[1] = { 0 };
	
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
	static float BackColor[] = { 0.8, 0.8, 0.8, 1.0 };

	dc->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(BackColor));
	dc->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
