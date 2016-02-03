#include "RenderEngine.h"

#include "OpenGLRenderer\OpenGLRenderer.h"
#include "D3D11Renderer\D3D11Renderer.h"

using namespace AGE;

RenderEngine* RenderEngine::mSingleton = 0;
RenderEngine::Type RenderEngine::mEngineType = RenderEngine::Type::OpenGL;

RenderEngine* RenderEngine::CreateRenderEngine()
{
	switch (mEngineType) {
	case RenderEngine::Type::OpenGL:
		return new OpenGLRenderer();
		break;
	case RenderEngine::Type::Direct3D11:
		return new D3D11Renderer();
		break; 
	default:
		return new OpenGLRenderer();
	}
}
