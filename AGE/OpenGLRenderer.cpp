#include "OpenGLRenderer.h"
#include "Engine.h"
#include "Mesh.h"
#include "Log.h"

#include "../3rd/GLTools/include/GLFrustum.h"
#include "../3rd/GLTools/include/math3d.h"
#include <GLTools.h>
#include <StopWatch.h>

using namespace AGE;
//#pragma comment(lib, "../3rd/glew-1.10.0/lib/Release/Win32/glew32.lib")

OpenGLRenderer::OpenGLRenderer(){
}

int OpenGLRenderer::SetupPixelFormat(){
	static PIXELFORMATDESCRIPTOR pfd=
	{ 
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER ,
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
GLFrustum           viewFrustum;
GLTriangleBatch     torusBatch;
GLShaderManager     shaderManager;

int OpenGLRenderer::StartUp(Window window){
	mMainWindow = window;

	if(!SetupPixelFormat())
		return 0;

	mRC =::wglCreateContext(mMainWindow.hDC);
	::wglMakeCurrent(mMainWindow.hDC, mRC);

	GLenum err = glewInit(); 
	if (GLEW_OK != err) {
		Log::Error("glewInit");
		return 0;
	}

	glViewport(0, 0, 800, 600);
	 viewFrustum.SetPerspective(35.0f, float(800)/float(600), 1.0f, 1000.0f);
	 gltMakeTorus(torusBatch, 0.4f, 0.15f, 30, 30);
	 shaderManager.InitializeStockShaders();
	glEnable(GL_DEPTH_TEST);
	//glClearColor(0.8f, 0.8f, 0.8f, 1.0f );
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	return 1;
}

void OpenGLRenderer::Begin(float* mvpMatrix){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mMVPMatrix = mvpMatrix;
}

void OpenGLRenderer::Render(Renderable* renderable){

	glBindVertexArray(renderable->GetRenderData()->VertexArrayBufferObject);
	glDrawElements(GL_TRIANGLES, torusBatch.nNumIndexes, GL_UNSIGNED_SHORT, 0);

	//shader->End();
}

void OpenGLRenderer::End(){
	SwapBuffers(mMainWindow.hDC);
}

void OpenGLRenderer::RenderScene(Scene* scene){
	static CStopWatch rotTimer;
    float yRot = rotTimer.GetElapsedSeconds() * 60.0f;
    
	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Matrix Variables
    M3DMatrix44f mTranslate, mRotate, mModelview, mModelViewProjection;
    
    // Create a translation matrix to move the torus back and into sight
    m3dTranslationMatrix44(mTranslate, 0.0f, 0.0f, -2.f);
    
    // Create a rotation matrix based on the current value of yRot
    m3dRotationMatrix44(mRotate, m3dDegToRad(yRot), 0.0f, 1.0f, 0.0f);
    
    // Add the rotation to the translation, store the result in mModelView
    m3dMatrixMultiply44(mModelview, mTranslate, mRotate);
    
    // Add the modelview matrix to the projection matrix, 
    // the final matrix is the ModelViewProjection matrix.
	
	const float* tmp = viewFrustum.GetProjectionMatrix();
	float* tmp2 = scene->GetCurrentCamera()->GetProjectMatrix();
	m3dMatrixMultiply44(mModelViewProjection, tmp2, mModelview);
		
    // Pass this completed matrix to the shader, and render the torus
    GLfloat vBlack[] = { 1.0f, 0.0f, 0.0f, 1.0f };


	Renderable* renderable = dynamic_cast<Renderable*>(scene->GetRoot()->GetAttachable()[0]);
	ShaderProgram* shader = renderable->GetShader();
	
	FlatShader shaderData;
	shaderData.ColorVector = vBlack;
	shaderData.MVPMatrix = mModelViewProjection;

	shader->Begin(&shaderData);


    //shaderManager.UseStockShader(GLT_SHADER_FLAT, mModelViewProjection, vBlack);
	GLuint vertexArrayBufferObject;
	glGenVertexArrays(1, &vertexArrayBufferObject);
	glBindVertexArray(vertexArrayBufferObject);

	static GLuint bufferObjects[4] = {0, 0, 0, 0};
	glGenBuffers(4, bufferObjects);

	glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[0]);
	glEnableVertexAttribArray(0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*torusBatch.nNumVerts*3, torusBatch.pVerts, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[NORMAL_DATA]);
	glEnableVertexAttribArray(GLT_ATTRIBUTE_NORMAL);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*torusBatch.nNumVerts*3, torusBatch.pNorms, GL_STATIC_DRAW);
	glVertexAttribPointer(GLT_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    // Texture coordinates
    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[TEXTURE_DATA]);
	glEnableVertexAttribArray(GLT_ATTRIBUTE_TEXTURE0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*torusBatch.nNumVerts*2, torusBatch.pTexCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(GLT_ATTRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    
    // Indexes
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObjects[INDEX_DATA]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*torusBatch.nNumIndexes, torusBatch.pIndexes, GL_STATIC_DRAW);
	
	glBindVertexArray(0);

	
	glBindVertexArray(vertexArrayBufferObject);
	glDrawElements(GL_TRIANGLES, torusBatch.nNumIndexes, GL_UNSIGNED_SHORT, 0);

	glDeleteBuffers(4, bufferObjects);
	
	glBindVertexArray(0);
    glDeleteVertexArrays(1, &vertexArrayBufferObject);
    //torusBatch.Draw();

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
	glBindAttribLocation(program, 2, "vNormal");

	glLinkProgram(program);

	glDeleteShader(vShader);
	glDeleteShader(fShader);  


	glGetProgramiv(program, GL_LINK_STATUS, &testVal);
	if(testVal == GL_FALSE)
	{
		glDeleteProgram(program);
		return (GLuint)NULL;
	}

	return program;  
}
