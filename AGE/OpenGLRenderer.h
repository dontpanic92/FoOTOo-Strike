#ifndef __AGE_OPENGLRENDERER_HEADER__
#define __AGE_OPENGLRENDERER_HEADER__

#include "../3rd/glew-1.10.0/include/GL/glew.h"
#include "../3rd/glew-1.10.0/include/GL/wglew.h"
#include "Window.h"
#include "Scene.h"

namespace AGE{

	class Mesh;
	class OpenGLRenderer
	{
	public:
		OpenGLRenderer();
		~OpenGLRenderer(){}

		int StartUp(Window window);

		void TestRender();

		void RenderMesh(const Mesh* mesh);

		void RenderScene(Scene* scene);

	protected:
		GLuint LoadShader(const char* vPath, const char* fPath);

		//GLuint CompileShader(const char )

		int SetupPixelFormat();

	private:
		HGLRC mRC;
		Window mMainWindow;
	};
}
#endif