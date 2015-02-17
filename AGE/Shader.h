#ifndef __AGE_SHADERPROGRAM_HEADER__
#define __AGE_SHADERPROGRAM_HEADER__

#include <string>
#include <vector>
#include "Singleton.h"
#include "Texture.h"


namespace AGE
{

	struct ShaderUniformParameter
	{
		enum class Type
		{
			INT1,
			INT2,
			INT3,
			INT4,
			FLOAT1,
			FLOAT2,
			FLOAT3,
			FLOAT4,
			MATRIX4F,
			TEXTURE
		};

		Type ParameterType;
		void* Parameter;
		std::string Name;
	};

	typedef std::vector<ShaderUniformParameter> ShaderUniforms;

	class DefaultShaderData
	{
	public:
		float* MMatrix;
		float* VMatrix;
		float* PMatrix;
		float* ColorVector;
		Texture2D* TextureUnit;

		DefaultShaderData() { 
			mParameters = { 
				{ ShaderUniformParameter::Type::MATRIX4F, &MMatrix, "mMatrix" },
				{ ShaderUniformParameter::Type::MATRIX4F, &VMatrix, "vMatrix" },
				{ ShaderUniformParameter::Type::MATRIX4F, &PMatrix, "pMatrix" },
				{ ShaderUniformParameter::Type::FLOAT4, &ColorVector, "vColor" },
				{ ShaderUniformParameter::Type::TEXTURE, &TextureUnit, "textureUnit0" }
			}; 
		}

		std::vector<ShaderUniformParameter>::const_iterator begin() const { return mParameters.begin(); }
		std::vector<ShaderUniformParameter>::const_iterator end() const { return mParameters.end(); }

		operator const ShaderUniforms& () { return mParameters; }

	private:
		std::vector<ShaderUniformParameter> mParameters;
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