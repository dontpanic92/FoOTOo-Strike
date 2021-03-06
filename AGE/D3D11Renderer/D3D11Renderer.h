#ifndef __AGE_D3D11RENDERER_HEADER__
#define __AGE_D3D11RENDERER_HEADER__

#include <d3dx11.h>

#include "../Window.h"
#include "../RenderEngine.h"
#include "D3D11Def.h"
#include "D3D11RenderTarget.h"

namespace AGE{

	class RenderPath;
	class D3D11Renderer : public RenderEngine
	{
	public:
		D3D11Renderer() {}
		~D3D11Renderer() { ShutDown(); }

		int StartUp();

		void ShutDown();


		RenderObject* CreateRenderObject(Renderable* renderable, Mesh* mesh, Material* material, Shader* shader, bool is_static) override;

		Shader* CreateShader(const char* shaderName) override;

		Texture* CreateTexture(const char* path, TextureType type) override;

		void ResizeToFit() override;

		void Render() override;

		ID3D11Device* GetDevice() { return mD3DDevice; }

		ID3D11DeviceContext* GetDeviceContext() { return mD3DImmediateContext; }

	protected:

	private:

		ID3D11Device* mD3DDevice;
		ID3D11DeviceContext* mD3DImmediateContext;
		IDXGISwapChain* mSwapChain;
		RenderPath*		mRenderPath;
		
		D3D_DRIVER_TYPE mD3DDriverType;

		//uint mResolutionX, mResolutionY;
	};
}

#endif