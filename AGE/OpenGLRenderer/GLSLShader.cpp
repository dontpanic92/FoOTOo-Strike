#include <cstdio>
#include "GLSLShader.h"
#include "../ResourceManager.h"
#include "../Math.h"
#include "../Log.h"

using namespace AGE;

GLSLShader::GLSLShader(const char* shaderName)
{
	Load(shaderName);
}

GLSLShader::~GLSLShader()
{
	if (mProgram != 0) {
		glDeleteProgram(mProgram);
	}
}

void GLSLShader::Use() const { glUseProgram(mProgram); }

bool GLSLShader::Load(const char* shaderName)
{
	GLuint vShader, fShader, program;
	GLint testVal;

	char vertexShaderPath[512], fragShaderPath[512];
	sprintf(vertexShaderPath, "%ls/Shaders/GLSL/%s.vshader", ResourceManager::RootDirectory, shaderName);
	sprintf(fragShaderPath, "%ls/Shaders/GLSL/%s.fshader", ResourceManager::RootDirectory, shaderName);

	printf(vertexShaderPath);

	vShader = glCreateShader(GL_VERTEX_SHADER);
	fShader = glCreateShader(GL_FRAGMENT_SHADER);

	ulong size;
	FILE* fp = fopen(vertexShaderPath, "r");
	if (fp == NULL) {
		Log::Error("Cannot Open Vertex Shader File.");
		mProgram = 0;
		return false;
	}
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char* szIdentityShaderVP = new char[size + 1];
	memset(szIdentityShaderVP, 0, sizeof(char) * (size + 1));

	fread(szIdentityShaderVP, 1, size, fp);

	fclose(fp);


	GLchar *fsStringPtr[1];
	fsStringPtr[0] = (GLchar *)szIdentityShaderVP;
	glShaderSource(vShader, 1, (const GLchar **)fsStringPtr, NULL);

	glCompileShader(vShader);
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &testVal);

	if (testVal == GL_FALSE) {
		Log::Error("Vertex Shader Compile Failed.");
		char infoLog[1024];
		glGetShaderInfoLog(fShader, 1024, NULL, infoLog);
		Log::Error(infoLog);
		glDeleteShader(vShader);
		glDeleteShader(fShader);
		mProgram = 0;
		delete[] szIdentityShaderVP;
		return false;
	}


	fp = fopen(fragShaderPath, "r");
	if (fp == NULL) {
		Log::Error("Cannot Open Fragment Shader File.");
		mProgram = 0;
		return false;
	}
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char* szIdentityShaderFP = new char[size + 1];
	memset(szIdentityShaderFP, 0, sizeof(char) * (size + 1));

	fread(szIdentityShaderFP, 1, size, fp);

	fclose(fp);


	fsStringPtr[0] = (GLchar *)szIdentityShaderFP;
	glShaderSource(fShader, 1, (const GLchar **)fsStringPtr, NULL);


	glCompileShader(fShader);

	glGetShaderiv(fShader, GL_COMPILE_STATUS, &testVal);
	if (testVal == GL_FALSE) {
		Log::Error("Fragment Shader Compile Failed.");
		char infoLog[1024];
		glGetShaderInfoLog(fShader, 1024, NULL, infoLog);
		Log::Error(infoLog);
		delete[] szIdentityShaderFP;
		delete[] szIdentityShaderVP;
		glDeleteShader(vShader);
		glDeleteShader(fShader);
		mProgram = 0;
		return false;
	}

	program = glCreateProgram();
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);

	glBindAttribLocation(program, 0, "vVertex");
	glBindAttribLocation(program, 1, "vNormal");
	glBindAttribLocation(program, 2, "vTexCoord0");


	glLinkProgram(program);

	glDeleteShader(vShader);
	glDeleteShader(fShader);

	delete[]szIdentityShaderVP;
	delete[]szIdentityShaderFP;

	glGetProgramiv(program, GL_LINK_STATUS, &testVal);
	if (testVal == GL_FALSE) {
		glDeleteProgram(program);
		mProgram = 0;
		return false;
	}

	mProgram = program;
	return true;
}

bool GLSLShader::ProcessParameter(const ShaderParameter& parameter)
{

	//printf("parameter %s \n", parameter.Name.c_str());
	GLint location = glGetUniformLocation(mProgram, parameter.Name.c_str());
	switch (parameter.ParameterType) {
	case ShaderParameter::Type::INT1:
		glUniform1i(location, *(int*)parameter.Parameter);
		break;
	case ShaderParameter::Type::FLOAT1:
		glUniform1f(location, *(float*)parameter.Parameter);
		break;
	case ShaderParameter::Type::FLOAT4:
		glUniform4fv(location, 1, *(float**)parameter.Parameter);
		break;
	case ShaderParameter::Type::MATRIX4F:
		glUniformMatrix4fv(location, 1, GL_FALSE, *(float**)parameter.Parameter);

		//for (int i = 0; i < 16; i++) {
		//	printf("%f ", (*(float**)parameter.Parameter)[i]);
		//}
		//printf("\n");
		//getchar();
		break;
	default:
		return false;
		break;
	}

	return true;
}

void GLSLShader::UpdateShaderData(const ShaderData& shaderData)
{
	for each (auto parameter in shaderData)
	{
		ProcessParameter(parameter);
	}
}