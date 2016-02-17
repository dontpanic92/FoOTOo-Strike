#ifndef __AGE_OPENGLRENDERER_HEADER__
#define __AGE_OPENGLRENDERER_HEADER__

//#include "../3rd/glew-1.10.0/include/GL/glew.h"
//#include "../3rd/glew-1.10.0/include/GL/wglew.h"
#include "../Window.h"
//#include "Scene.h"
//#include "Singleton.h"

#define FREEGLUT_STATIC
#include <GL/glew.h>

#include "../RenderEngine.h"

namespace AGE{

	class Mesh;
	class OpenGLRenderer : public RenderEngine
	{
	public:
		OpenGLRenderer();
		~OpenGLRenderer(){}

		int StartUp();

		//void Render(Renderable* renderable);
		void Render();

		void Begin();

		RenderObject* CreateRenderObject(Renderable* renderable, Mesh* mesh, Material* material, Shader* shader, bool is_static) override;

		Shader* CreateShader(const char* shaderName) override;

		Texture* CreateTexture(const char* path, TextureType type) override;

		void End();

	protected:
		int SetupPixelFormat();

	private:
		
		float* mMVPMatrix;

		HGLRC mRC;
		Window mMainWindow;
	};

}
#endif