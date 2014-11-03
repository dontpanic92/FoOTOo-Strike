#ifndef __AGE_OPENGLRENDERER_HEADER__
#define __AGE_OPENGLRENDERER_HEADER__

//#include "../3rd/glew-1.10.0/include/GL/glew.h"
//#include "../3rd/glew-1.10.0/include/GL/wglew.h"
#include "Window.h"
#include "Scene.h"
#include "Singleton.h"

#define FREEGLUT_STATIC
#include <GL/glew.h>

namespace AGE{

	class Mesh;
	class OpenGLRenderer
	{
	public:
		OpenGLRenderer();
		~OpenGLRenderer(){}

		int StartUp(Window window);

		void Render(Renderable* renderable);

		void Begin();

		void End();

	protected:
		int SetupPixelFormat();

	private:
		
		float* mMVPMatrix;

		HGLRC mRC;
		Window mMainWindow;
	};

	class RenderEngine : public Singleton<RenderEngine>
	{
	public:
		RenderEngine(){ mOpenGLRenderer = new OpenGLRenderer(); }
		~RenderEngine(){ delete mOpenGLRenderer; }

		//void StartUp(Window window){ mOpenGLRenderer->StartUp(window); }

		static OpenGLRenderer* GetInstance(){ return Singleton<RenderEngine>::GetInstance()->mOpenGLRenderer; }

	private:
		OpenGLRenderer* mOpenGLRenderer;
		friend class Singleton<RenderEngine>;
	};
}
#endif