#include "D3D11ForwardRendering.h"
#include "D3D11Renderer.h"
#include "D3D11RenderObject.h"
#include "D3D11Texture.h"
#include "HLSLShader.h"
#include "../ResourceManager.h"
#include "../LevelManager.h"
#include "../RenderQueue.h"
#include "../RtInformation.h"

using namespace AGE;

D3D11ForwardRendering::D3D11ForwardRendering(ID3D11Device* device, ID3D11DeviceContext* context, IDXGISwapChain* swapChain, Window window)
	:mD3DDevice(device), mD3DImmediateContext(context), mSwapChain(swapChain)
{
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
	nocullDesc.DepthBias = 0;// 10000;
	nocullDesc.DepthBiasClamp = 0.0f;
	nocullDesc.SlopeScaledDepthBias = 1.0f;
	(mD3DDevice->CreateRasterizerState(&nocullDesc, &mNoCullRasterizerState));


	mShadowMapTarget = new D3D11ShadowMapTarget(mD3DDevice, 4096, 4096);
	mNormalTarget = new D3D11NormalTarget(mD3DDevice, mSwapChain, window.Width, window.Height);
	mNormalTarget->SetRenderTarget(mD3DImmediateContext);

}

D3D11ForwardRendering::~D3D11ForwardRendering()
{
	delete mShadowMapTarget;
	delete mNormalTarget;
	SafeRelease(mRasterizerState);
}


struct SceneBounds
{
	Vector3f Center = Vector3f(0, 0, 0);
	float	 Radius = 2800;
}SceneBounds;

Matrix4x4f lightProjMatrix;// = MakeOrthoProjectionMatrix(SceneBounds.Radius * 2, SceneBounds.Radius * 2, 1, SceneBounds.Radius * 2);
Matrix4x4f ShadowVPTMatrix;
Matrix4x4f lightViewMatrix;
Matrix4x4f tMatrix;

void D3D11ForwardRendering::ShadowMapPass()
{
	if (!GetScene())
		return;

	HLSLShader* shadowShader = (HLSLShader*)ResourceManager::GetInstance()->LoadShader("ShadowMap");

	class ShadowMapShaderData : public ShaderData
	{
	public:
		float* MVPMatrix;

		ShadowMapShaderData()
		{
			mParameters = {
				{ ShaderUniformParameter::Type::MATRIX4F, &MVPMatrix, "MVPMatrix" },
			};
		}
	} shaderData;

	mShadowMapTarget->SetRenderTarget(mD3DImmediateContext);
	mD3DImmediateContext->RSSetState(mNoCullRasterizerState);

	auto v = GetScene()->GetLights();
	if (!v.size())
		return;
	Light l = *v[0];
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
			Matrix4x4f MVPMatrix = d3d11Object->Parent->GetParent()->GetWorldMatrix() * lightViewMatrix * lightProjMatrix;
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

void D3D11ForwardRendering::SkyBoxPass()
{
	if (!GetScene())
		return;

	SkyBox* skyBox = GetScene()->GetSkyBox();

	if (!skyBox)
		return;

	HLSLShader* shadowShader = (HLSLShader*)ResourceManager::GetInstance()->LoadShader("sky");

	class SkyBoxShaderData : public ShaderData
	{
	public:
		float* MVPMatrix;
		Texture* CubeTexture;

		SkyBoxShaderData()
		{
			mParameters = {
				{ ShaderUniformParameter::Type::MATRIX4F, &MVPMatrix, "gWorldViewProj" },
				{ ShaderUniformParameter::Type::TEXTURE, &CubeTexture, "gCubeMap" }
			};
		}
	} shaderData;

	//mD3DImmediateContext->RSSetState(mNoCullRasterizerState);

	Camera* camera = GetScene()->GetCurrentCamera();

	Renderable* r = skyBox->GetRenderable();

	for (int i = 0; i < r->GetNumberOfRenderObjects(); i++)
	{
		D3D11RenderObject* d3d11Object = (D3D11RenderObject*)r->GetRenderObject(i);
		HLSLShader* shader = (HLSLShader*)d3d11Object->Shader;

		Transform t = camera->GetWorldTransform();
		t.ClearRotation();
		Matrix4x4f NormalMatrix = t.GetTransformMatrix() * camera->GetWorldMatrix().Transpose().Inverse();
		Matrix4x4f MVPMatrix = NormalMatrix * camera->GetProjectMatrix();
		shaderData.MVPMatrix = MVPMatrix;
		shaderData.CubeTexture = d3d11Object->Material->GetTexture();


		shader->UpdateShaderData(shaderData);

		mD3DImmediateContext->IASetInputLayout(shader->InputLayout);
		shader->Technique->GetPassByIndex(0)->Apply(0, mD3DImmediateContext);

		UINT offset = 0;
		UINT stride = sizeof(Mesh::Vertex);

		mD3DImmediateContext->IASetVertexBuffers(0, 1, &d3d11Object->VertexBuffer, &stride, &offset);
		mD3DImmediateContext->Draw(d3d11Object->Mesh->GetNumberOfVertex(), 0);
	}
}

void D3D11ForwardRendering::RenderPass()
{
	mNormalTarget->SetRenderTarget(mD3DImmediateContext);
	mNormalTarget->ClearView(mD3DImmediateContext);
	mD3DImmediateContext->RSSetState(mRasterizerState);
	mD3DImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if (!GetScene())
		return;

	const RenderQueue::RenderQueueMap& map = RenderQueue::GetInstance()->GetQueue();

	GLfloat vBlack[] = { 0.0, 0.0, 0.0, 1.0f };
	DefaultShaderData shaderData;
	shaderData.ColorVector = vBlack;
	Camera* camera = GetScene()->GetCurrentCamera();

	Light l = *GetScene()->GetLights()[0];
	l.Direction = l.Direction * (Matrix3x3f)camera->GetWorldMatrix().Transpose().Inverse();//camera->CalcViewMatrix();//
	shaderData.Light = &l;

	D3D11Texture2D shadowTexture(mD3DDevice);
	shadowTexture.SetTextureSRV(mShadowMapTarget->GetRenderTextureShaderResourceView());

	shaderData.ShadowTextureUnit = &shadowTexture;


	for each (auto& pair in map)
	{
		shaderData.TextureUnit = pair.first->GetTexture();
		//vector<ID3D11Buffer*> buffers(pair.second.size());

		for each (auto object in pair.second)
		{
			D3D11RenderObject* d3d11Object = (D3D11RenderObject*)object;
			HLSLShader* shader = (HLSLShader*)d3d11Object->Shader;

			Matrix4x4f NormalMatrix = d3d11Object->Parent->GetParent()->GetWorldMatrix() * camera->GetWorldMatrix().Transpose().Inverse();//lightViewMatrix;//
			Matrix4x4f MVPMatrix = NormalMatrix * camera->GetProjectMatrix();//lightProjMatrix;//
			shaderData.MVPMatrix = MVPMatrix;
			shaderData.NormalMatrix = NormalMatrix;
			Matrix4x4f ShadowMatrix = d3d11Object->Parent->GetParent()->GetWorldMatrix() * lightViewMatrix * lightProjMatrix * tMatrix;
			shaderData.ShadowMatrix = ShadowMatrix;

			shader->UpdateShaderData(shaderData);

			//shader->Use();
			mD3DImmediateContext->IASetInputLayout(shader->InputLayout);
			shader->Technique->GetPassByIndex(0)->Apply(0, mD3DImmediateContext);

			UINT offset = 0;
			UINT stride = sizeof(Mesh::Vertex);

			mD3DImmediateContext->IASetVertexBuffers(0, 1, &d3d11Object->VertexBuffer, &stride, &offset);
			mD3DImmediateContext->Draw(d3d11Object->Mesh->GetNumberOfVertex(), 0);

			RtInformation::GetInstance()->MoreTriangles(object->Mesh->GetNumberOfVertex() / 3);
		}
	}
}

int D3D11ForwardRendering::ExecuteRendering()
{

	ShadowMapPass();
	RenderPass();
	SkyBoxPass();

	mD3DImmediateContext->RSSetState(0);
	mD3DImmediateContext->OMSetDepthStencilState(0, 0);
	ID3D11ShaderResourceView* nullSRV[16] = { 0 };
	mD3DImmediateContext->PSSetShaderResources(0, 16, nullSRV);
	(mSwapChain->Present(0, 0));
	RenderQueue::GetInstance()->ClearQueue();
	return 0;
}

