#ifndef __AGE_SHADERDATA_HEADER__
#define __AGE_SHADERDATA_HEADER__

#include <string>
#include <vector>

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
			TEXTURE,
			LIGHT
		};

		Type ParameterType;
		void* Parameter;
		std::string Name;
	};

	typedef std::vector<ShaderUniformParameter> ShaderUniforms;

	class ShaderData {
	public:
		std::vector<ShaderUniformParameter>::const_iterator begin() const { return mParameters.begin(); }
		std::vector<ShaderUniformParameter>::const_iterator end() const { return mParameters.end(); }

		operator const ShaderUniforms& () { return mParameters; }

	protected:
		std::vector<ShaderUniformParameter> mParameters;
	};
}

#endif