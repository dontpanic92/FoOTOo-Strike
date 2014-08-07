#include "OpenGLRenderer.h"
#include "Engine.h"
#include "Mesh.h"

using namespace AGE;
#pragma comment(lib, "../3rd/glew-1.10.0/lib/Release/Win32/glew32.lib")

OpenGLRenderer::OpenGLRenderer(){
}

int OpenGLRenderer::SetupPixelFormat(){
	static PIXELFORMATDESCRIPTOR pfd=
	{ 
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, 
		PFD_TYPE_RGBA,
		24,
		0,0,0,0,0,0,0,0,0,0,0,0,0,
		32,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0,0,0
	};

	int m_PixelFormat;

	if((m_PixelFormat = ChoosePixelFormat(mMainWindow.hDC, &pfd)) == 0)
	{
		return 0;
	}

	if(::SetPixelFormat(mMainWindow.hDC, m_PixelFormat, &pfd) == FALSE)
	{
		return 0;
	}
	return 1;
}

int OpenGLRenderer::StartUp(Window window){
	mMainWindow = window;

	if(!SetupPixelFormat())
		return 0;

	mRC =::wglCreateContext(mMainWindow.hDC);
	::wglMakeCurrent(mMainWindow.hDC, mRC);

	GLenum err = glewInit(); 
	if (GLEW_OK != err) {
		return 0;
	}

	glViewport(0, 0, 800, 600);
	return 1;
}

void OpenGLRenderer::RenderScene(Scene* scene){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	static GLuint bufferObjects[2] = {0, 0};

	static GLuint shaderProg = LoadShader(NULL, NULL);

	glUseProgram(shaderProg);

	static GLint iColor = glGetUniformLocation(shaderProg, "vColor");;
	static GLfloat vColor[] = {0, 1, 0, 1};

	glUniform4fv(iColor, 1, vColor);

	Mesh* mesh = dynamic_cast<Mesh*>(scene->GetRoot()->GetAttachable()[0]);

	if(true/*bufferObjects[0] == 0*/){
		glGenBuffers(2, bufferObjects);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[0]);

		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * mesh->GetVertexNum(), mesh->GetVertexData(), GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObjects[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * mesh->GetIndexNum(), mesh->GetIndexData(), GL_STATIC_DRAW);

	}else{
		glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObjects[1]);

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 3 * mesh->GetVertexNum(), mesh->GetVertexData());
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLushort) * mesh->GetIndexNum(), mesh->GetIndexData());
		
	}

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawElements(GL_TRIANGLES, mesh->GetIndexNum(), GL_UNSIGNED_SHORT, 0);

	glDisableVertexAttribArray(0);

	glDeleteBuffers(2, bufferObjects);
	SwapBuffers(mMainWindow.hDC);
}

void OpenGLRenderer::RenderMesh(const Mesh* mesh){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	static GLuint bufferObjects[2] = {0, 0};

	static GLuint shaderProg = LoadShader(NULL, NULL);

	glUseProgram(shaderProg);

	static GLint iColor = glGetUniformLocation(shaderProg, "vColor");;
	static GLfloat vColor[] = {0, 1, 0, 1};

	glUniform4fv(iColor, 1, vColor);

	if(true/*bufferObjects[0] == 0*/){
		glGenBuffers(2, bufferObjects);
		glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[0]);

		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * mesh->GetVertexNum(), mesh->GetVertexData(), GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObjects[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * mesh->GetIndexNum(), mesh->GetIndexData(), GL_STATIC_DRAW);

	}else{
		glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObjects[1]);

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 3 * mesh->GetVertexNum(), mesh->GetVertexData());
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLushort) * mesh->GetIndexNum(), mesh->GetIndexData());
		
	}

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawElements(GL_TRIANGLES, mesh->GetIndexNum(), GL_UNSIGNED_SHORT, 0);

	glDisableVertexAttribArray(0);

	glDeleteBuffers(2, bufferObjects);
	SwapBuffers(mMainWindow.hDC);
}

void OpenGLRenderer::TestRender(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	GLuint bufferObjects[2];
	GLfloat vVerts[] = {-0.5f, -0.5f, 0.0f, 
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5, 0.5, 0.0f};

	GLuint shaderProg = LoadShader(NULL, NULL);

	glUseProgram(shaderProg);

	GLint iColor;
	GLfloat vColor[] = {0, 1, 0, 1};

	iColor = glGetUniformLocation(shaderProg, "vColor");

	glUniform4fv(iColor, 1, vColor);

	glGenBuffers(2, bufferObjects);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * 4, vVerts, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLushort pIndexes[] = {0, 1, 2, 0, 2, 3};

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObjects[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*6, pIndexes, GL_STATIC_DRAW);

	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);


	glDisableVertexAttribArray(0);

	glDeleteBuffers(2, bufferObjects);
	SwapBuffers(mMainWindow.hDC);
}

GLuint OpenGLRenderer::LoadShader(const char* vPath, const char* fPath){
	GLuint vShader, fShader, program;
	GLint testVal;

	static const char *szIdentityShaderVP = "attribute vec4 vVertex;"
		"void main(void) "
		"{ gl_Position = vVertex; "
		"}";

	static const char *szIdentityShaderFP = 
		"uniform vec4 vColor;"
		"void main(void) "
		"{ gl_FragColor = vColor;"
		"}";

	vShader = glCreateShader(GL_VERTEX_SHADER);
	fShader = glCreateShader(GL_FRAGMENT_SHADER);

	GLchar *fsStringPtr[1];
	fsStringPtr[0] = (GLchar *)szIdentityShaderVP;
	glShaderSource(vShader, 1, (const GLchar **)fsStringPtr, NULL);

	fsStringPtr[0] = (GLchar *)szIdentityShaderFP;
	glShaderSource(fShader, 1, (const GLchar **)fsStringPtr, NULL);

	glCompileShader(vShader);
	glCompileShader(fShader);


	glGetShaderiv(vShader, GL_COMPILE_STATUS, &testVal);

	if(testVal == GL_FALSE){
		glDeleteShader(vShader); 
		glDeleteShader(fShader);
		return 0;
	}

	glGetShaderiv(fShader, GL_COMPILE_STATUS, &testVal);
	if(testVal == GL_FALSE){
		glDeleteShader(vShader); 
		glDeleteShader(fShader);
		return 0;
	}

	program = glCreateProgram();
	glAttachShader(program, vShader);
	glAttachShader(program, fShader);

	glBindAttribLocation(program, 0, "vVertex");

	glLinkProgram(program);

	glDeleteShader(vShader);
	glDeleteShader(fShader);  

	// Make sure link worked too
	glGetProgramiv(program, GL_LINK_STATUS, &testVal);
	if(testVal == GL_FALSE)
	{
		glDeleteProgram(program);
		return (GLuint)NULL;
	}

	return program;  
}
