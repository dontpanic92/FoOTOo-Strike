#ifndef __AGE_SHADERPROGRAM_HEADER__
#define __AGE_SHADERPROGRAM_HEADER__

#include <string>
#include <vector>
#include "Singleton.h"
#include "Texture.h"
#include "Light.h"
#include "ShaderData.h"


namespace AGE
{

	class DefaultShaderData : public ShaderData
	{
	public:
		float* MVPMatrix;
		float* NormalMatrix;
		float* ColorVector;
		float* ShadowMatrix;
		Texture* TextureUnit;
		Texture* ShadowTextureUnit;
		Light* Light;

		DefaultShaderData() { 
			mParameters = { 
				{ ShaderUniformParameter::Type::MATRIX4F, &MVPMatrix, "MVPMatrix" },
				{ ShaderUniformParameter::Type::MATRIX4F, &NormalMatrix, "NormalMatrix" },
				{ ShaderUniformParameter::Type::MATRIX4F, &ShadowMatrix, "shadowTransform" },
				{ ShaderUniformParameter::Type::FLOAT4, &ColorVector, "vColor" },
				{ ShaderUniformParameter::Type::TEXTURE, &TextureUnit, "textureUnit0" },
				{ ShaderUniformParameter::Type::TEXTURE, &ShadowTextureUnit, "shadowMap" },
				{ ShaderUniformParameter::Type::LIGHT, &Light, "dirLight" }
			}; 
		}
	};

	class Shader
	{
	public:
		virtual void Use() const = 0;

		//template<class ShaderData>
		virtual void UpdateShaderData(const ShaderUniforms& shaderData) = 0;

		//virtual bool ProcessSingleParameter(const ShaderParameter& parameter) = 0;

		//virtual bool ProcessParameterCompelete() = 0;

		virtual ~Shader() {}
	};
}

#endif