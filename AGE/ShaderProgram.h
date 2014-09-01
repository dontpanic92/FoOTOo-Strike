#ifndef __AGE_SHADERPROGRAM_HEADER__
#define __AGE_SHADERPROGRAM_HEADER__

//#include "../3rd/glew-1.10.0/include/GL/glew.h"
//#include "../3rd/glew-1.10.0/include/GL/wglew.h"

#include <GL\glew.h>

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

	struct Shader
	{
		float* MMatrix;
		float* VMatrix;
		float* PMatrix;
		float* ColorVector;

		void InitUniforms(GLint program) const{
			GLint iTransform, iView, iProject, iColor, iTextureUnit;
 			iTransform = glGetUniformLocation(program, "mMatrix");
			glUniformMatrix4fv(iTransform, 1, GL_FALSE, MMatrix);

			iView = glGetUniformLocation(program, "vMatrix");
			glUniformMatrix4fv(iView, 1, GL_FALSE, VMatrix);

			iProject = glGetUniformLocation(program, "pMatrix");
			glUniformMatrix4fv(iProject, 1, GL_FALSE, PMatrix);

			iColor = glGetUniformLocation(program, "vColor");
			glUniform4fv(iColor, 1, ColorVector);

			iTextureUnit = glGetUniformLocation(program, "textureUnit0");
			glUniform1i(iTextureUnit, 0);
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