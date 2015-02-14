#include "D3D11Renderer.h"
#include "D3D11RenderObject.h"
#include "HLSLShader.h"
#include "../RenderQueue.h"
#include "../Engine.h"
#include "../RtInfomation.h"
//#pragma comment(lib, "d3d11.lib")
//#pragma comment(lib, "d3d11d.lib")


using namespace AGE;

int D3D11Renderer::StartUp(Window window)
{
	mMainWindow = window;

	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
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

	if (featureLevel != D3D_FEATURE_LEVEL_11_0) {
		//MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		return 0;
	}


	// Check 4X MSAA quality support for our back buffer format.
	// All Direct3D 11 capable devices support 4X MSAA for all render 
	// target formats, so we only need to check quality support.

	/*HR(md3dDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality));
	assert(m4xMsaaQuality > 0);
	*/
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
	ID3D11Texture2D* backBuffer;
	(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	(mD3DDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView));
	SafeRelease(backBuffer);

	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_BACK;
	rsDesc.FrontCounterClockwise = true;
	rsDesc.DepthClipEnable = true;
	(mD3DDevice->CreateRasterizerState(&rsDesc, &mRasterizerState));
	mD3DImmediateContext->RSSetState(mRasterizerState);

	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = window.Width;
	depthStencilDesc.Height = window.Height;
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

	(mD3DDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer));
	(mD3DDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView));


	// Bind the render target view and depth/stencil view to the pipeline.

	mD3DImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);


	// Set the viewport transform.

	mScreenViewport.TopLeftX = 0;
	mScreenViewport.TopLeftY = 0;
	mScreenViewport.Width = static_cast<float>(window.Width);
	mScreenViewport.Height = static_cast<float>(window.Height);
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.MaxDepth = 1.0f;

	mD3DImmediateContext->RSSetViewports(1, &mScreenViewport);

	return 1;
}

void D3D11Renderer::ShutDown()
{
	SafeRelease(mRenderTargetView);
	SafeRelease(mDepthStencilView);
	SafeRelease(mRasterizerState);
	SafeRelease(mSwapChain);
	//SafeRelease(mDepthStencilBuffer);

	// Restore all default settings.
	if (mD3DImmediateContext)
		mD3DImmediateContext->ClearState();

	SafeRelease(mD3DImmediateContext);
	SafeRelease(mD3DDevice);
}


RenderObject* D3D11Renderer::CreateRenderObject(Renderable* renderable, Mesh* mesh, Material* material)
{
	D3D11RenderObject *object = new D3D11RenderObject();
	object->Parent = renderable;
	object->Material = material;
	object->Mesh = mesh;

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * mesh->GetNumberOfVertex();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
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

void D3D11Renderer::Render()
{
	const RenderQueue::RenderQueueMap& map = RenderQueue::GetInstance()->GetQueue();


	GLfloat vBlack[] = { .6f, 0.6f, 0.6f, 1.0f };
	DefaultShaderData shaderData;
	shaderData.ColorVector = vBlack;
	shaderData.VMatrix = Engine::GetInstance()->GetScene()->GetCurrentCamera()->CalcViewMatrix();
	shaderData.PMatrix = Engine::GetInstance()->GetScene()->GetCurrentCamera()->GetProjectMatrix();

	Begin();
	for each (auto& pair in map)
	{
		pair.first->Use();
		for each (auto object in pair.second)
		{
			D3D11RenderObject* d3d11Object = (D3D11RenderObject*)object;
			HLSLShader* shader = (HLSLShader*)pair.first->GetShader();
			shaderData.MMatrix = d3d11Object->Parent->GetWorldMatrix();
			shader->UpdateShaderData(shaderData);

			UINT offset = 0;
			UINT stride = sizeof(Vertex);

			mD3DImmediateContext->IASetVertexBuffers(0, 1, &d3d11Object->VertexBuffer, &stride, &offset);
			mD3DImmediateContext->Draw(d3d11Object->Mesh->GetNumberOfVertex(), 0);
		
			RtInfomation::GetInstance()->MoreTriangles(object->Mesh->GetNumberOfVertex() / 3);
		}
	}
	End();
	RenderQueue::GetInstance()->ClearQueue();
}

void D3D11Renderer::Begin()
{

	static float BackColor[] = {0.8, 0.8, 0.8, 1.0};
	mD3DImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(BackColor));
	mD3DImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	mD3DImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void D3D11Renderer::End()
{

	(mSwapChain->Present(0, 0));
}
