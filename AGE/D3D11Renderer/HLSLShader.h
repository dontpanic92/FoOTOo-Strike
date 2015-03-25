#ifndef __AGE_HLSLSHADER_HEADER__
#define __AGE_HLSLSHADER_HEADER__

#include <D3DX11.h>
#include "../Shader.h"
#include "d3dx11effect.h"
#include "D3D11Renderer.h"

namespace AGE{

	class HLSLShader : public Shader
	{
	public:
		HLSLShader(const char* shaderName);

		~HLSLShader();

		void Use() const;

		void UpdateShaderData(const ShaderUniforms& shaderData);

		bool ProcessParameter(const ShaderUniformParameter& parameter);

	public:
		bool Load(const char* shaderName);

		//DXGI_FORMAT GetDXGIFormat(ShaderParameter::Type type);

		ID3DX11Effect* Effect;
		ID3DX11EffectTechnique* Technique;
		ID3D11InputLayout* InputLayout;
	};
}

#endif