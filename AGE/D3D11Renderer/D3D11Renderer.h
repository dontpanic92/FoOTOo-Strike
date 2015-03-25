#ifndef __AGE_D3D11RENDERER_HEADER__
#define __AGE_D3D11RENDERER_HEADER__

#include <d3dx11.h>

#include "../Window.h"
#include "../RenderEngine.h"
#include "D3D11Def.h"
#include "D3D11RenderTarget.h"

namespace AGE{

	class D3D11Renderer : public RenderEngine
	{
	public:
		D3D11Renderer() {}
		~D3D11Renderer() { ShutDown(); }

		int StartUp(Window window);

		void ShutDown();


		RenderObject* CreateRenderObject(Renderable* renderable, Mesh* mesh, Material* material, Shader* shader) override;

		Shader* CreateShader(const char* shaderName) override;

		Texture2D* CreateTextrue2D(const char* path) override;

		void Render();

		ID3D11Device* GetDevice() { return mD3DDevice; }

		ID3D11DeviceContext* GetDeviceContext() { return mD3DImmediateContext; }

	protected:

	private:

		void ShadowMapPass();

		ID3D11Device* mD3DDevice;
		ID3D11DeviceContext* mD3DImmediateContext;
		IDXGISwapChain* mSwapChain;
		ID3D11RasterizerState* mRasterizerState;
		ID3D11RasterizerState* mNoCullRasterizerState;

		D3D11NormalTarget* mNormalTarget;
		D3D11ShadowMapTarget* mShadowMapTarget;
		
		D3D_DRIVER_TYPE mD3DDriverType;

		HGLRC mRC;
		Window mMainWindow;
	};
}

#endif