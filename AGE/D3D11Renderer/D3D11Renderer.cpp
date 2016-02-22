#include "D3D11Renderer.h"
#include "D3D11RenderObject.h"
#include "HLSLShader.h"
#include "D3D11Texture.h"
#include "../RenderQueue.h"
#include "../Engine.h"
#include "../RtInformation.h"
#include "../ResourceManager.h"
#include "../LevelManager.h"
#include "D3D11ForwardRendering.h"
//#pragma comment(lib, "d3d11.lib")
//#pragma comment(lib, "d3d11d.lib")


using namespace AGE;

int D3D11Renderer::StartUp()
{
	Window& window = Engine::GetInstance()->GetMainWindow();

	UINT createDeviceFlags = 0;
#if 0//defined(DEBUG) || defined(_DEBUG)  
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr = D3D11CreateDevice(
		0,                 // default adapter
		D3D_DRIVER_TYPE_HARDWARE,
		0,                 // no software device
		createDeviceFlags,
		0, 0,              // default feature level array
		D3D11_SDK_VERSION,
		&mD3DDevice,
		&featureLevel,
		&mD3DImmediateContext);

	if (FAILED(hr)) {
		//MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
		return 0;
	}

	////if (featureLevel != D3D_FEATURE_LEVEL_11_0) {
	//	MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
	//	return 0;
	//}


	// Check 4X MSAA quality support for our back buffer format.
	// All Direct3D 11 capable devices support 4X MSAA for all render 
	// target formats, so we only need to check quality support.
	UINT m4xMsaaQuality;
	(mD3DDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality));
	//assert(m4xMsaaQuality > 0);
	
	
	// Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = window.Width;
	sd.BufferDesc.Height = window.Height;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Use 4X MSAA? 
	/*if (mEnable4xMsaa) {
	sd.SampleDesc.Count = 4;
	sd.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
	// No MSAA
	else {*/
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	//}


	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = window.hWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	// To correctly create the swap chain, we must use the IDXGIFactory that was
	// used to create the device.  If we tried to use a different IDXGIFactory instance
	// (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
	// This function is being called with a device from a different IDXGIFactory."

	IDXGIDevice* dxgiDevice = 0;
	(mD3DDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

	IDXGIAdapter* dxgiAdapter = 0;
	(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	IDXGIFactory* dxgiFactory = 0;
	(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

	(dxgiFactory->CreateSwapChain(mD3DDevice, &sd, &mSwapChain));

	SafeRelease(dxgiDevice);
	SafeRelease(dxgiAdapter);
	SafeRelease(dxgiFactory);

	//HR(mSwapChain->ResizeBuffers(1, mClientWidth, mClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));

	mRenderPath = new D3D11ForwardRendering(mD3DDevice, mD3DImmediateContext, mSwapChain, window);


	HLSLShader* shader = (HLSLShader*)ResourceManager::GetInstance()->LoadShader("DefaultLight");
	mD3DImmediateContext->IASetInputLayout(shader->InputLayout);
	return 1;
}

void D3D11Renderer::ShutDown()
{
	if (mRenderPath)
	{
		delete mRenderPath;
		mRenderPath = nullptr;
	}

	SafeRelease(mSwapChain);
	//SafeRelease(mDepthStencilBuffer);

	// Restore all default settings.
	if (mD3DImmediateContext)
		mD3DImmediateContext->ClearState();

	SafeRelease(mD3DImmediateContext);
	SafeRelease(mD3DDevice);
}


RenderObject* D3D11Renderer::CreateRenderObject(Renderable* renderable, Mesh* mesh, Material* material, Shader* shader, bool is_static)
{
	D3D11RenderObject *object = new D3D11RenderObject();
	object->Parent = renderable;
	object->Material = material;
	object->Mesh = mesh;
	object->Shader = shader;
	object->IsStatic = is_static;

	D3D11_BUFFER_DESC vertexBufferDesc;
	if (is_static)
	{
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.CPUAccessFlags = 0;
	}
	else
	{
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	vertexBufferDesc.ByteWidth = sizeof(Mesh::Vertex) * mesh->GetNumberOfVertex();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = mesh->GetVertexData();
	(mD3DDevice->CreateBuffer(&vertexBufferDesc, &vinitData, &object->VertexBuffer));

	return object;
}

Shader* D3D11Renderer::CreateShader(const char* shaderName)
{
	return new HLSLShader(shaderName);
}

Texture* D3D11Renderer::CreateTexture(const char* path, TextureType type)
{
	D3D11Texture* pTex;

	switch (type) {
	case TextureType::Texture2D:
		pTex = new D3D11Texture2D(mD3DDevice);
		break;
	case TextureType::CubeTexture:
		pTex = new D3D11CubeTexture(mD3DDevice);
	}

	if (!pTex->Load(path)) {
		delete pTex;
		pTex = nullptr;
	}

	return pTex;
}

void D3D11Renderer::Render()
{
	mRenderPath->ExecuteRendering();
}

void D3D11Renderer::ResizeToFit()
{
	Window& window = Engine::GetInstance()->GetMainWindow();
	window.Recalc();

	SafeRelease(mSwapChain);

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = window.Width;
	sd.BufferDesc.Height = window.Height;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Use 4X MSAA? 
	/*if (mEnable4xMsaa) {
	sd.SampleDesc.Count = 4;
	sd.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
	// No MSAA
	else {*/
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	//}

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = window.hWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	// To correctly create the swap chain, we must use the IDXGIFactory that was
	// used to create the device.  If we tried to use a different IDXGIFactory instance
	// (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
	// This function is being called with a device from a different IDXGIFactory."

	IDXGIDevice* dxgiDevice = 0;
	(mD3DDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

	IDXGIAdapter* dxgiAdapter = 0;
	(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	IDXGIFactory* dxgiFactory = 0;
	(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

	(dxgiFactory->CreateSwapChain(mD3DDevice, &sd, &mSwapChain));

	SafeRelease(dxgiDevice);
	SafeRelease(dxgiAdapter);
	SafeRelease(dxgiFactory);

	//printf("resize: %X\n", h);
	delete mRenderPath;
	mRenderPath = new D3D11ForwardRendering(mD3DDevice, mD3DImmediateContext, mSwapChain, window);
	if (GetScene()) {
		GetScene()->UpdateCameraAspectRatio();
	}
}
