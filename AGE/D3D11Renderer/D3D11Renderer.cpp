#include "D3D11Renderer.h"
#include "D3D11RenderObject.h"
#include "HLSLShader.h"
#include "D3D11Texture.h"
#include "../RenderQueue.h"
#include "../Engine.h"
#include "../RtInfomation.h"
#include "../ResourceManager.h"
#include "../LevelManager.h"
//#pragma comment(lib, "d3d11.lib")
//#pragma comment(lib, "d3d11d.lib")


using namespace AGE;

int D3D11Renderer::StartUp(Window window)
{
	mMainWindow = window;

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

	if (featureLevel != D3D_FEATURE_LEVEL_11_0) {
		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
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
	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_BACK;
	rsDesc.FrontCounterClockwise = true;
	rsDesc.DepthClipEnable = true;
	(mD3DDevice->CreateRasterizerState(&rsDesc, &mRasterizerState));
	mD3DImmediateContext->RSSetState(mRasterizerState);

	D3D11_RASTERIZER_DESC nocullDesc;
	ZeroMemory(&nocullDesc, sizeof(D3D11_RASTERIZER_DESC));
	nocullDesc.FillMode = D3D11_FILL_SOLID;
	nocullDesc.CullMode = D3D11_CULL_NONE;
	nocullDesc.FrontCounterClockwise = true;
	nocullDesc.DepthClipEnable = true;
	nocullDesc.DepthBias = 10000;
	nocullDesc.DepthBiasClamp = 0.0f;
	nocullDesc.SlopeScaledDepthBias = 1.0f;
	(mD3DDevice->CreateRasterizerState(&nocullDesc, &mNoCullRasterizerState));

	// Bind the render target view and depth/stencil view to the pipeline.


	mShadowMapTarget = new D3D11ShadowMapTarget(mD3DDevice, 2048, 2048);
	mNormalTarget = new D3D11NormalTarget(mD3DDevice, mSwapChain, window.Width, window.Height);
	mNormalTarget->SetRenderTarget(mD3DImmediateContext);

	HLSLShader* shader = (HLSLShader*)ResourceManager::GetInstance()->LoadShader("DefaultLight");
	mD3DImmediateContext->IASetInputLayout(shader->InputLayout);
	return 1;
}

void D3D11Renderer::ShutDown()
{
	SafeRelease(mRasterizerState);
	SafeRelease(mSwapChain);
	//SafeRelease(mDepthStencilBuffer);

	// Restore all default settings.
	if (mD3DImmediateContext)
		mD3DImmediateContext->ClearState();

	SafeRelease(mD3DImmediateContext);
	SafeRelease(mD3DDevice);

	delete mShadowMapTarget;
	delete mNormalTarget;
}


RenderObject* D3D11Renderer::CreateRenderObject(Renderable* renderable, Mesh* mesh, Material* material, Shader* shader)
{
	D3D11RenderObject *object = new D3D11RenderObject();
	object->Parent = renderable;
	object->Material = material;
	object->Mesh = mesh;
	object->Shader = shader;

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(Mesh::Vertex) * mesh->GetNumberOfVertex();
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

Texture2D* D3D11Renderer::CreateTextrue2D(const char* path)
{
	return new D3D11Texture(mD3DDevice, path);
}


struct SceneBounds
{
	Vector3f Center = Vector3f(0, 0, 0);
	float	 Radius = 3000;
}SceneBounds;

Matrix4x4f lightProjMatrix;// = MakeOrthoProjectionMatrix(SceneBounds.Radius * 2, SceneBounds.Radius * 2, 1, SceneBounds.Radius * 2);
Matrix4x4f ShadowVPTMatrix;
Matrix4x4f lightViewMatrix;
Matrix4x4f tMatrix;

void D3D11Renderer::ShadowMapPass()
{
	HLSLShader* shadowShader = (HLSLShader*)ResourceManager::GetInstance()->LoadShader("ShadowMap");

	class ShadowMapShaderData
	{
	public:
		float* MVPMatrix;

		ShadowMapShaderData()
		{
			mParameters = {
				{ ShaderUniformParameter::Type::MATRIX4F, &MVPMatrix, "MVPMatrix" },
			};
		}

		std::vector<ShaderUniformParameter>::const_iterator begin() const { return mParameters.begin(); }
		std::vector<ShaderUniformParameter>::const_iterator end() const { return mParameters.end(); }

		operator const ShaderUniforms& () { return mParameters; }

	private:
		std::vector<ShaderUniformParameter> mParameters;
	}shaderData;

	mShadowMapTarget->SetRenderTarget(mD3DImmediateContext);
	mD3DImmediateContext->RSSetState(mNoCullRasterizerState);

	Light l = *GetScene()->GetLights()[0];
	Transform tran;
	Vector3f lightDir = l.Direction;
	lightDir.Normalize();
	Vector3f lightPos = lightDir * (-SceneBounds.Radius) * 2;

	tran.SetPosition(lightPos);
	tran.RotateByRadian(Deg2Rad(-135), 0, 1, 0);
	tran.RotateByRadian(Deg2Rad(45), 1, 0, 0);
	//tran.SetPosition(Vector3f(1000, 1000, 1000));
	lightViewMatrix = tran.GetTransformMatrix().Transpose().Inverse();

	Vector3f transedCenter = SceneBounds.Center * lightViewMatrix;
	float left = transedCenter[0] - SceneBounds.Radius;
	float bottom = transedCenter[1] - SceneBounds.Radius;
	float n = transedCenter[2] - SceneBounds.Radius;
	float right = transedCenter[0] + SceneBounds.Radius;
	float top = transedCenter[1] + SceneBounds.Radius;
	float f = transedCenter[2] + SceneBounds.Radius;
	lightProjMatrix = MakeOrthoProjectionMatrix(left, right, bottom, top, n, f);

	float t[] = { 0.5f, 0.0f, 0.0f, 0.0f,
				0.0f, -0.5f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.5f, 0.5f, 0.0f, 1.0f };
	tMatrix = t;

	//ShadowVPTMatrix = lightViewMatrix * lightProjMatrix * t;

	const RenderQueue::RenderQueueMap& map = RenderQueue::GetInstance()->GetQueue();

	mD3DImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	for each (auto& pair in map)
	{
		for each (auto object in pair.second)
		{
			D3D11RenderObject* d3d11Object = (D3D11RenderObject*)object;
			//HLSLShader* shader = (HLSLShader*)d3d11Object->Shader;
			Matrix4x4f MVPMatrix = d3d11Object->Parent->GetWorldMatrix() * lightViewMatrix * lightProjMatrix;
			shaderData.MVPMatrix = MVPMatrix;


			shadowShader->UpdateShaderData(shaderData);
			//shader->Use();
			//mD3DImmediateContext->IASetInputLayout(shader->InputLayout);
			shadowShader->Technique->GetPassByIndex(0)->Apply(0, mD3DImmediateContext);

			UINT offset = 0;
			UINT stride = sizeof(Mesh::Vertex);

			mD3DImmediateContext->IASetVertexBuffers(0, 1, &d3d11Object->VertexBuffer, &stride, &offset);
			mD3DImmediateContext->Draw(d3d11Object->Mesh->GetNumberOfVertex(), 0);
		}
	}

}

void D3D11Renderer::Render()
{

	ShadowMapPass();
	//mD3DImmediateContext->RSSetState(0);
	mNormalTarget->SetRenderTarget(mD3DImmediateContext);
	mNormalTarget->ClearView(mD3DImmediateContext);
	mD3DImmediateContext->RSSetState(mRasterizerState);
	mD3DImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	const RenderQueue::RenderQueueMap& map = RenderQueue::GetInstance()->GetQueue();

	GLfloat vBlack[] = { 0.0, 0.0, 0.0, 1.0f };
	DefaultShaderData shaderData;
	shaderData.ColorVector = vBlack;
	Camera* camera = GetScene()->GetCurrentCamera();

	Light l = *GetScene()->GetLights()[0];
	l.Direction = l.Direction * (Matrix3x3f)camera->GetWorldMatrix().Transpose().Inverse();//camera->CalcViewMatrix();//
	shaderData.Light = &l;

	D3D11Texture shadowTexture(mD3DDevice);
	shadowTexture.TextureSRV = mShadowMapTarget->GetShaderResourceView();

	shaderData.ShadowTextureUnit = &shadowTexture;


	for each (auto& pair in map)
	{
		shaderData.TextureUnit = pair.first->GetTexture();
		for each (auto object in pair.second)
		{
			D3D11RenderObject* d3d11Object = (D3D11RenderObject*)object;
			HLSLShader* shader = (HLSLShader*)d3d11Object->Shader;

			Matrix4x4f NormalMatrix = d3d11Object->Parent->GetWorldMatrix() * camera->GetWorldMatrix().Transpose().Inverse();//lightViewMatrix;//
			Matrix4x4f MVPMatrix = NormalMatrix * camera->GetProjectMatrix();//lightProjMatrix;//
			shaderData.MVPMatrix = MVPMatrix;
			shaderData.NormalMatrix = NormalMatrix;
			Matrix4x4f ShadowMatrix = d3d11Object->Parent->GetWorldMatrix() * lightViewMatrix * lightProjMatrix * tMatrix;
			shaderData.ShadowMatrix = ShadowMatrix;

			shader->UpdateShaderData(shaderData);

			//shader->Use();
			mD3DImmediateContext->IASetInputLayout(shader->InputLayout);
			shader->Technique->GetPassByIndex(0)->Apply(0, mD3DImmediateContext);

			UINT offset = 0;
			UINT stride = sizeof(Mesh::Vertex);

			mD3DImmediateContext->IASetVertexBuffers(0, 1, &d3d11Object->VertexBuffer, &stride, &offset);
			mD3DImmediateContext->Draw(d3d11Object->Mesh->GetNumberOfVertex(), 0);
		
			RtInfomation::GetInstance()->MoreTriangles(object->Mesh->GetNumberOfVertex() / 3);
		}
	}

	//mD3DImmediateContext->RSSetState(0);
	//mD3DImmediateContext->OMSetDepthStencilState(0, 0);
	ID3D11ShaderResourceView* nullSRV[16] = { 0 };
	mD3DImmediateContext->PSSetShaderResources(0, 16, nullSRV);
	(mSwapChain->Present(0, 0));
	RenderQueue::GetInstance()->ClearQueue();
}
