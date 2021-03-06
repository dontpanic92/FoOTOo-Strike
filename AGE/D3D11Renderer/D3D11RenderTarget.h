#ifndef __AGE_D3D11RENDERTARGET_HEADER__
#define __AGE_D3D11RENDERTARGET_HEADER__

#include <d3d11.h>
#include "../LinearMath.h"
#include "D3D11Def.h"

namespace AGE{
	class D3D11RenderTarget
	{
	public:
		D3D11RenderTarget(ID3D11Device* device) : mDevice(device) {}

		virtual ~D3D11RenderTarget() {}

		virtual void SetRenderTarget(ID3D11DeviceContext* dc) = 0;

	protected:

		ID3D11Device* mDevice;
	};

	class D3D11ShadowMapTarget : public D3D11RenderTarget
	{
	public:
		D3D11ShadowMapTarget(ID3D11Device* device, uint width, uint height);

		~D3D11ShadowMapTarget();

		void SetRenderTarget(ID3D11DeviceContext* dc);
		ID3D11ShaderResourceView* GetDepthShaderResourceView()
		{
			return mDepthMapSRV;
		}

		ID3D11DepthStencilView* GetDepthStencilView()
		{
			return mDepthMapDSV;
		}

		ID3D11ShaderResourceView* GetRenderTextureShaderResourceView()
		{
			return mRenderTargetMapSRV;
		}


		ID3D11Texture2D* GetRenderTexture()
		{
			return mRenderTargetMap;
		}

		ID3D11Texture2D* GetDepthTexture()
		{
			return mDepthMap;
		}

	protected:

		uint mWidth;
		uint mHeight;
		ID3D11Texture2D*	mRenderTargetMap = nullptr;
		ID3D11RenderTargetView*		mRenderTargetView = nullptr;
		ID3D11ShaderResourceView*	mRenderTargetMapSRV = nullptr;
		ID3D11DepthStencilView*		mDepthMapDSV = nullptr;
		ID3D11ShaderResourceView*	mDepthMapSRV = nullptr;
		D3D11_VIEWPORT		mViewport;
		ID3D11Texture2D*	mDepthMap = nullptr;
	};

	class D3D11NormalTarget : public D3D11RenderTarget
	{
	public:
		D3D11NormalTarget(ID3D11Device* device, IDXGISwapChain* swapChain, uint width, uint height);

		~D3D11NormalTarget();

		void SetRenderTarget(ID3D11DeviceContext* dc);

		void ClearView(ID3D11DeviceContext* dc);
	private:
		uint mWidth;
		uint mHeight;
		ID3D11RenderTargetView* mRenderTargetView;
		ID3D11DepthStencilView* mDepthStencilView;
		ID3D11Texture2D*	mDepthStencilBuffer;
		D3D11_VIEWPORT		mViewport;
	};

}
#endif
