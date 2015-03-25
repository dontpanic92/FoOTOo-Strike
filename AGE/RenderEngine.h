#ifndef __AGE_RENDERENGINE_HEADER__
#define __AGE_RENDERENGINE_HEADER__

#include "Singleton.h"
#include "Window.h"
#include "Renderable.h"

#include "Shader.h"
#include "Texture.h"

namespace AGE{

	class RenderEngine : public NonCopyable
	{
	public:

		enum class Type { Direct3D11, OpenGL };

		virtual ~RenderEngine() {}

		static void SetEngineType(Type engineType)
		{
			if (!mSingleton)
				mEngineType = engineType;
		}

		static RenderEngine* GetInstance()
		{
			if (!mSingleton) {
				mSingleton = CreateRenderEngine();
			}

			return mSingleton;
		}

		virtual int StartUp(Window window) = 0;

		virtual RenderObject* CreateRenderObject(Renderable* renderable, Mesh* mesh, Material* material, Shader* shader) = 0;

		virtual Shader* CreateShader(const char* shaderName) { return NULL; }

		virtual Texture2D* CreateTextrue2D(const char* path) { return NULL; }

		virtual void Render() = 0;

		//virtual bool AddLight() = 0;

	protected:
		RenderEngine() {}

		static Type mEngineType;

	private:
		static RenderEngine* CreateRenderEngine();

		static RenderEngine* mSingleton;
	};
}

#endif
