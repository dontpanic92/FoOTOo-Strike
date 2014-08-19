#include "ShaderProgram.h"
#include "Log.h"
#include "Math.h"
#include <cstdio>

using namespace AGE;


ShaderProgram::ShaderProgram() : mProgram(0){}

ShaderProgram::~ShaderProgram(){
	if(mProgram != 0){
		glDeleteProgram(mProgram);
	}
}

bool ShaderProgram::LoadAndCompile(const char* vShaderPath, const char* fShaderPath){
	GLuint vShader, fShader, program;
	GLint testVal;

	
	vShader = glCreateShader(GL_VERTEX_SHADER);
	fShader = glCreateShader(GL_FRAGMENT_SHADER);

	ulong size;
    FILE* fp = fopen(vShaderPath, "r");
    if(fp == NULL){
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

	if(testVal == GL_FALSE){
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


	fp = fopen(fShaderPath, "r");
    if(fp == NULL){
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
	if(testVal == GL_FALSE){
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

	delete []szIdentityShaderVP;
	delete []szIdentityShaderFP;

	glGetProgramiv(program, GL_LINK_STATUS, &testVal);
	if(testVal == GL_FALSE)
	{
		glDeleteProgram(program);
		mProgram = 0;
		return false;
	}

	mProgram = program;
	return true;
}

