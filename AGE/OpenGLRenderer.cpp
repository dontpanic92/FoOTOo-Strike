#include "OpenGLRenderer.h"
#include "Engine.h"
#include "Mesh.h"
#include "Log.h"
#include "RtInfomation.h"
#include "RenderQueue.h"

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
	//glPolygonMode(GL_FRONT_AND_BACK, GL_SMOOTH);//, GL_LINE);
	return 1;
}

void OpenGLRenderer::Begin()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::Render(Renderable* renderable)
{
	RtInfomation::GetInstance()->MoreObjects(renderable->GetNumberOfRenderObjects());
	for (uint i = 0; i < renderable->GetNumberOfRenderObjects(); i++) {
		const RenderObject* object = renderable->GetRenderObject(i);

		glBindVertexArray(object->VertexArrayBufferObject);

		object->Material->Use();
		glDrawArrays(GL_TRIANGLES, 0, object->Mesh->GetVertexNum());
		RtInfomation::GetInstance()->MoreTriangles(object->Mesh->GetVertexNum() / 3);

		//glDrawElements(GL_TRIANGLES, object->Mesh->GetVertexNum(), GL_UNSIGNED_SHORT, 0);
	}

}

void OpenGLRenderer::Render()
{
	const RenderQueue::RenderQueueMap& map = RenderQueue::GetInstance()->GetQueue();
	Matrix4x4f viewMatrix = Engine::GetInstance()->GetScene()->GetCurrentCamera()->CalcViewMatrix();
	Begin();

	GLfloat vBlack[] = { .6f, 0.6f, 0.6f, 1.0f };
	DefaultShaderData shaderData;
	shaderData.ColorVector = vBlack;
	shaderData.VMatrix = viewMatrix;
	shaderData.PMatrix = Engine::GetInstance()->GetScene()->GetCurrentCamera()->GetProjectMatrix();

	for each (auto& pair in map)
	{
		pair.first->Use();
		for each (auto object in pair.second)
		{
			shaderData.MMatrix = object->Parent->GetWorldMatrix();
			pair.first->GetShaderProgram()->UpdateShaderData(&shaderData);
			glBindVertexArray(object->VertexArrayBufferObject);
			glDrawArrays(GL_TRIANGLES, 0, object->Mesh->GetVertexNum());
			RtInfomation::GetInstance()->MoreTriangles(object->Mesh->GetVertexNum() / 3);
		}
	}
	End();
	RenderQueue::GetInstance()->ClearQueue();
}

void OpenGLRenderer::End()
{
	SwapBuffers(mMainWindow.hDC);
}
