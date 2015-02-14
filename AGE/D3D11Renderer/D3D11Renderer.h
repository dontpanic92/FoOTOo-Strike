#ifndef __AGE_D3D11RENDERER_HEADER__
#define __AGE_D3D11RENDERER_HEADER__

#include <d3dx11.h>

#include "../Window.h"
#include "../RenderEngine.h"

namespace AGE{

#define SafeRelease(x) { if(x){ x->Release(); x = 0; } }

	class D3D11Renderer : public RenderEngine
	{
	public:
		D3D11Renderer() {}
		~D3D11Renderer() { ShutDown(); }

		int StartUp(Window window);

		void ShutDown();


		RenderObject* CreateRenderObject(Renderable* renderable, Mesh* mesh, Material* material);

		Shader* CreateShader(const char* shaderName);

		void Render();

		void Begin();

		void End();

		ID3D11Device* GetDevice() { return mD3DDevice; }

		ID3D11DeviceContext* GetDeviceContext() { return mD3DImmediateContext; };

	protected:

	private:

		ID3D11Device* mD3DDevice;
		ID3D11DeviceContext* mD3DImmediateContext;
		IDXGISwapChain* mSwapChain;
		ID3D11Texture2D* mDepthStencilBuffer;
		ID3D11RenderTargetView* mRenderTargetView;
		ID3D11DepthStencilView* mDepthStencilView;
		ID3D11RasterizerState* mRasterizerState;
		D3D11_VIEWPORT mScreenViewport;
		D3D_DRIVER_TYPE mD3DDriverType;

		HGLRC mRC;
		Window mMainWindow;
	};
}

#endif