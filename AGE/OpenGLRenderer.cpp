#include "OpenGLRenderer.h"
#include "Engine.h"
#include "Mesh.h"
#include "Log.h"

using namespace AGE;

OpenGLRenderer::OpenGLRenderer(){
}

int OpenGLRenderer::SetupPixelFormat(){
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

	if ((m_PixelFormat = ChoosePixelFormat(mMainWindow.hDC, &pfd)) == 0)
	{
		return 0;
	}

	if (::SetPixelFormat(mMainWindow.hDC, m_PixelFormat, &pfd) == FALSE)
	{
		return 0;
	}
	return 1;
}

int OpenGLRenderer::StartUp(Window window){
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

	glViewport(0, 0, 800, 600);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_SMOOTH);//, GL_LINE);
	return 1;
}

void OpenGLRenderer::Begin(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::Render(Renderable* renderable){
	glBindVertexArray(renderable->GetRenderData()->VertexArrayBufferObject);

	//const Mesh::MaterialCollection& ms = renderable->GetMesh()->GetMaterials();
	//for (uint i = 0; i < ms.size(); i++){
	renderable->GetMaterial()->Use();
	glDrawArrays(GL_TRIANGLES, 0, renderable->GetMesh()->GetVertexNum());
	//glDrawElements(GL_TRIANGLES, ms[i]->GetIndexNum(), GL_UNSIGNED_SHORT, 0);
	//}

}

void OpenGLRenderer::End(){
	SwapBuffers(mMainWindow.hDC);
}
