#include "d3dx11effect.h"
#include "HLSLShader.h"
#include "../ResourceManager.h"
#include "D3D11Texture.h"


using namespace AGE;

HLSLShader::HLSLShader(const char* shaderName) : Effect(0), InputLayout(0)
{
	Load(shaderName);
}

HLSLShader::~HLSLShader()
{
	SafeRelease(Effect);
	SafeRelease(InputLayout);
}

bool HLSLShader::Load(const char* shaderName)
{
	DWORD shaderFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	wchar_t shaderPath[512];
	wsprintf(shaderPath, L"%s/Shaders/HLSL/%hs.fx", ResourceManager::RootDirectory, shaderName);

	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;
	HRESULT hr = D3DX11CompileFromFile(shaderPath, 0, 0, 0, "fx_5_0", shaderFlags,
									   0, 0, &compiledShader, &compilationMsgs, 0);

	// compilationMsgs can store errors or warnings.
	if (compilationMsgs != 0) {
		MessageBoxA(0, (char*)compilationMsgs->GetBufferPointer(), 0, 0);
		SafeRelease(compilationMsgs);
	}

	// Even if there are no compilationMsgs, check to make sure there were no other errors.
	if (FAILED(hr)) {
		//DXTrace(__FILE__, (DWORD)__LINE__, hr, L"D3DX11CompileFromFile", true);
	}

	(D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(),
		0, ((D3D11Renderer*)(RenderEngine::GetInstance()))->GetDevice(), &Effect));

	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// Done with compiled shader.
	SafeRelease(compiledShader);

	Technique = Effect->GetTechniqueByName("ColorTech");

	ID3D11Device* device = ((D3D11Renderer*)(RenderEngine::GetInstance()))->GetDevice();
	D3DX11_PASS_DESC passDesc;
	Technique->GetPassByIndex(0)->GetDesc(&passDesc);
	(device->CreateInputLayout(vertexDesc, 3, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &InputLayout));
	
	//(device->CreateInputLayout(vertexDesc, 1, compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), &InputLayout));

	return true;
}

bool HLSLShader::ProcessParameter(const ShaderUniformParameter& parameter)
{
	ID3DX11EffectVariable* var = Effect->GetVariableByName(parameter.Name.c_str());
	switch (parameter.ParameterType) {
	case ShaderUniformParameter::Type::INT1:
		var->AsScalar()->SetInt(*(int*)parameter.Parameter);
		return true;
		break;
	case ShaderUniformParameter::Type::FLOAT1:
		var->AsScalar()->SetFloat(*(float*)parameter.Parameter);
		return true;
		break;
	case ShaderUniformParameter::Type::FLOAT4:
		var->AsVector()->SetFloatVector(*(float**)parameter.Parameter);
		return true;
		break;
	case ShaderUniformParameter::Type::MATRIX4F:
		var->AsMatrix()->SetMatrix(*(float**)parameter.Parameter);
		return true;
		break;
	case ShaderUniformParameter::Type::TEXTURE:
		var->AsShaderResource()->SetResource((*(D3D11Texture**)parameter.Parameter)->TextureSRV);
		return true;
		break;
	case ShaderUniformParameter::Type::LIGHT:
		float color[4];
		memcpy(color, (*(Light**)parameter.Parameter)->Direction, 3 * sizeof(float));
		var->SetRawValue(color, 0, sizeof(color));
		return true;
		break;
	}
	return false;
}

void HLSLShader::Use() const
{
	ID3D11DeviceContext* context = ((D3D11Renderer*)(RenderEngine::GetInstance()))->GetDeviceContext();
	context->IASetInputLayout(InputLayout);
	Technique->GetPassByIndex(0)->Apply(0, context);
}

void HLSLShader::UpdateShaderData(const ShaderUniforms& shaderData)
{
	for each (auto parameter in shaderData)
	{
		ProcessParameter(parameter);
	}
}
