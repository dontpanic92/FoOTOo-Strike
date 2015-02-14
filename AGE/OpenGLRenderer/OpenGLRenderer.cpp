#include "OpenGLRenderer.h"
#include "OpenGLRenderObject.h"
#include "GLSLShader.h"
#include "../Engine.h"
#include "../Mesh.h"
#include "../Log.h"
#include "../RtInfomation.h"
#include "../RenderQueue.h"
using namespace AGE;

OpenGLRenderer::OpenGLRenderer()
{
}

int OpenGLRenderer::SetupPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		32,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	int m_PixelFormat;

	if ((m_PixelFormat = ChoosePixelFormat(mMainWindow.hDC, &pfd)) == 0) {
		return 0;
	}

	if (::SetPixelFormat(mMainWindow.hDC, m_PixelFormat, &pfd) == FALSE) {
		return 0;
	}
	return 1;
}

void createGBufTex(GLenum texUnit, GLenum format, GLuint &texid)
{
	glActiveTexture(texUnit);
	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	//glTexStorage2D(GL_TEXTURE_2D, 1, format);
	glTexImage2D(GL_TEXTURE_2D, 1, GL_RGB, 800, 600, 0, GL_RGB, format, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

GLuint deferredFBO;

void setupFBO()
{
	GLuint depthBuf, posTex, normTex, colorTex;

	// Create and bind the FBO
	glGenFramebuffers(1, &deferredFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, deferredFBO);

	// The depth buffer
	glGenRenderbuffers(1, &depthBuf);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 800, 600);

	// Create the textures for position, normal and color
	createGBufTex(GL_TEXTURE0, GL_RGB32F, posTex);  // Position
	createGBufTex(GL_TEXTURE1, GL_RGB32F, normTex); // Normal
	createGBufTex(GL_TEXTURE2, GL_RGB8, colorTex);  // Color

	// Attach the textures to the framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, posTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, colorTex, 0);

	GLenum drawBuffers[] = { GL_NONE, GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(4, drawBuffers);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int OpenGLRenderer::StartUp(Window window)
{
	mMainWindow = window;

	if (!SetupPixelFormat())
		return 0;

	mRC = ::wglCreateContext(mMainWindow.hDC);
	::wglMakeCurrent(mMainWindow.hDC, mRC);

	GLenum err = glewInit();
	if (GLEW_OK != err) {
		Log::Error("glewInit");
		return 0;
	}

	glViewport(0, 0, window.Width, window.Height);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	//setupFBO();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_SMOOTH);//, GL_LINE);
	return 1;
}

RenderObject* OpenGLRenderer::CreateRenderObject(Renderable* renderable, Mesh* mesh, Material* material)
{
	OpenGLRenderObject* object = new OpenGLRenderObject();
	object->Parent = renderable;
	object->Material = material;
	object->Mesh = mesh;

	glGenVertexArrays(1, &object->VertexArrayBufferObject);
	glBindVertexArray(object->VertexArrayBufferObject);

	glGenBuffers(3, object->BufferObjects);

	glBindBuffer(GL_ARRAY_BUFFER, object->BufferObjects[0]);
	glEnableVertexAttribArray(0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*mesh->GetNumberOfVertex() * 3, mesh->GetVertexData(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


	glBindBuffer(GL_ARRAY_BUFFER, object->BufferObjects[1]);
	glEnableVertexAttribArray(1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*mesh->GetNumberOfVertex() * 3, mesh->GetNormalData(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, object->BufferObjects[2]);
	glEnableVertexAttribArray(2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*mesh->GetNumberOfVertex() * 2, mesh->GetTextureData(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Indices
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.BufferObjects[2]);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*object.Mesh->GetNumberOfVertex(), object.Mesh->GetIndexData(), GL_STATIC_DRAW);

	glBindVertexArray(0);

	return object;
}

void OpenGLRenderer::Begin()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Shader* OpenGLRenderer::CreateShader(const char* shaderName)
{
	return new GLSLShader(shaderName);
}
/*void OpenGLRenderer::Render(Renderable* renderable)
{
	RtInfomation::GetInstance()->MoreObjects(renderable->GetNumberOfRenderObjects());
	for (uint i = 0; i < renderable->GetNumberOfRenderObjects(); i++) {
		const RenderObject* object = renderable->GetRenderObject(i);

		glBindVertexArray(object->VertexArrayBufferObject);

		object->Material->Use();
		//glMultiDrawArrays()
		glDrawArrays(GL_TRIANGLES, 0, object->Mesh->GetVertexNum());
		RtInfomation::GetInstance()->MoreTriangles(object->Mesh->GetVertexNum() / 3);

		//glDrawElements(GL_TRIANGLES, object->Mesh->GetVertexNum(), GL_UNSIGNED_SHORT, 0);
	}

}*/

void OpenGLRenderer::Render()
{
	const RenderQueue::RenderQueueMap& map = RenderQueue::GetInstance()->GetQueue();


	GLfloat vBlack[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	DefaultShaderData shaderData;
	shaderData.ColorVector = vBlack;
	shaderData.VMatrix = Engine::GetInstance()->GetScene()->GetCurrentCamera()->CalcViewMatrix();
	shaderData.PMatrix = Engine::GetInstance()->GetScene()->GetCurrentCamera()->GetProjectMatrix();

	/*printf("vmatrix \n");
	for (int i = 0; i < 16; i++) {
		printf("%f ", shaderData.VMatrix[i]);
	}
	printf("\n");
	printf("pmatrix \n");
	for (int i = 0; i < 16; i++) {
		printf("%f ", shaderData.PMatrix[i]);
	}
	printf("\n");
	getchar();
	*/

	Begin();
	for each (auto& pair in map)
	{
		pair.first->Use();
		for each (auto object in pair.second)
		{
			OpenGLRenderObject* openGLObject = (OpenGLRenderObject*)object;
			shaderData.MMatrix = openGLObject->Parent->GetWorldMatrix();

			pair.first->GetShader()->UpdateShaderData(shaderData);
			glBindVertexArray(openGLObject->VertexArrayBufferObject);
			glDrawArrays(GL_TRIANGLES, 0, openGLObject->Mesh->GetNumberOfVertex());
			RtInfomation::GetInstance()->MoreTriangles(object->Mesh->GetNumberOfVertex() / 3);
		}
	}
	End();
	RenderQueue::GetInstance()->ClearQueue();
}

void OpenGLRenderer::End()
{
	SwapBuffers(mMainWindow.hDC);
}
