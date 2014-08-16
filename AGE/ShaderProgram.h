#ifndef __AGE_SHADERPROGRAM_HEADER__
#define __AGE_SHADERPROGRAM_HEADER__

//#include "../3rd/glew-1.10.0/include/GL/glew.h"
//#include "../3rd/glew-1.10.0/include/GL/wglew.h"

#include <GL\glew.h>

namespace AGE
{
	struct FlatShader
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
	};

	class ShaderProgram
	{
	public:
		ShaderProgram();
		~ShaderProgram();

		bool LoadAndCompile(const char* vShaderPath, const char* fShaderPath);
		GLuint GetProgram(){ return mProgram; }

		template<class Shader>
		void Begin(const Shader* shader){ glUseProgram(mProgram); shader->InitUniforms(mProgram); }

		void End(){ glUseProgram(0); }
	private:
		GLuint mProgram;
	};
}

#endif