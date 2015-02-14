#ifndef __AGE_SHADERPROGRAM_HEADER__
#define __AGE_SHADERPROGRAM_HEADER__

#include "Singleton.h"
#include <string>
#include <vector>


namespace AGE
{
	/*struct FlatShader
	{
		float* MVPMatrix;
		float* ColorVector;

		void InitUniforms(GLint program) const{
			GLint iTransform, iColor;
			iTransform = glGetUniformLocation(program, "mvpMatrix");
			glUniformMatrix4fv(iTransform, 1, GL_FALSE, MVPMatrix);

			iColor = glGetUniformLocation(program, "vColor");
			glUniform4fv(iColor, 1, ColorVector);
		}
	};*/

	struct ShaderParameter
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
			MATRIX4F
		};

		Type ParameterType;
		void* Parameter;
		std::string Name;
	};

	typedef std::vector<ShaderParameter> ShaderData;

	class DefaultShaderData
	{
	public:
		float* MMatrix;
		float* VMatrix;
		float* PMatrix;
		float* ColorVector;
		int TextureUnit;

		DefaultShaderData() { 
			TextureUnit = 0;
			mParameters = { 
				{ ShaderParameter::Type::MATRIX4F, &MMatrix, "mMatrix" }, 
				{ ShaderParameter::Type::MATRIX4F, &VMatrix, "vMatrix" }, 
				{ ShaderParameter::Type::MATRIX4F, &PMatrix, "pMatrix" } ,
				{ ShaderParameter::Type::FLOAT4, &ColorVector, "vColor" },
				{ ShaderParameter::Type::INT1,  &TextureUnit, "textureUnit0"}
			}; 
		}

		std::vector<ShaderParameter>::const_iterator begin() const { return mParameters.begin(); }
		std::vector<ShaderParameter>::const_iterator end() const { return mParameters.end(); }

		operator const ShaderData& () { return mParameters; }

	private:
		std::vector<ShaderParameter> mParameters;
	};

	class Shader
	{
	public:
		virtual void Use() const = 0;

		//template<class ShaderData>
		virtual void UpdateShaderData(const ShaderData& shaderData) = 0;

		//virtual bool ProcessSingleParameter(const ShaderParameter& parameter) = 0;

		//virtual bool ProcessParameterCompelete() = 0;

		virtual ~Shader() {}
	};
}

#endif