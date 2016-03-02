#ifndef __AGE_D3D11FORWARDRENDERING_HEADER__
#define __AGE_D3D11FORWARDRENDERING_HEADER__

#include "../RenderPath.h"
#include "../Window.h"
#include "D3D11RenderTarget.h"

namespace AGE
{
	class D3D11ForwardRendering : public RenderPath
	{
	public:
		D3D11ForwardRendering(ID3D11Device* device, ID3D11DeviceContext* context, IDXGISwapChain* swapChain, Window window);

		~D3D11ForwardRendering();

		int ExecuteRendering() override;

	protected:

		void ShadowMapPass();
		void RenderPass();
		void SkyBoxPass();

		D3D11NormalTarget* mNormalTarget;
		D3D11ShadowMapTarget* mShadowMapTarget;

		ID3D11Device* mD3DDevice;
		ID3D11DeviceContext* mD3DImmediateContext;
		IDXGISwapChain* mSwapChain;
		ID3D11RasterizerState* mRasterizerState;
		ID3D11RasterizerState* mNoCullRasterizerState;
	};
}


#endif
