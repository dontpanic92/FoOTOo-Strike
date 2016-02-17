#ifndef __AGE_RENDERENGINE_HEADER__
#define __AGE_RENDERENGINE_HEADER__

#include "Singleton.h"
#include "Window.h"
#include "Renderable.h"

#include "Shader.h"
#include "Texture.h"

namespace AGE{

	class AGE_EXPORT RenderEngine : public NonCopyable
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

		virtual int StartUp() = 0;

		virtual RenderObject* CreateRenderObject(Renderable* renderable, Mesh* mesh, Material* material, Shader* shader, bool is_static) = 0;

		virtual Shader* CreateShader(const char* shaderName) { return NULL; }

		virtual Texture* CreateTexture(const char* path, TextureType type) { return NULL; }
		
		virtual void ResizeToFit() {}

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
