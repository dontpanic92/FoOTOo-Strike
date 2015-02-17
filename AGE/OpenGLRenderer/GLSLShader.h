#ifndef __AGE_GLSLSHADER_HEADER__
#define __AGE_GLSLSHADER_HEADER__

#include <GL/glew.h>
#include "../Shader.h"

namespace AGE{

	class GLSLShader : public Shader
	{
	public:
		GLSLShader(const char* shaderName);

		~GLSLShader();
		
		void UpdateShaderData(const ShaderUniforms& shaderData);

		bool UpdateShaderData(const ShaderUniformParameter& parameter);

		void Use() const;

	protected:
		bool Load(const char* shaderName);


		//bool ProcessSingleParameter(const ShaderParameter& parameter);

		//bool ProcessParameterCompelete();

		GLuint mProgram;
	};
}

#endif