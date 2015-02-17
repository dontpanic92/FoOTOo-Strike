#ifndef __AGE_D3D11RENDEROBJECTDATA_HEADER__
#define __AGE_D3D11RENDEROBJECTDATA_HEADER__

#include "../Renderable.h"
#include <D3D11.h>

namespace AGE{

	class D3D11RenderObject : public RenderObject
	{
	public:
		ID3D11Buffer* VertexBuffer;
		//ID3D11Buffer* IndexBuffer;

		void Update()
		{
			
			//glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[0]);
			//glBufferSubData(GL_ARRAY_BUFFER, 0,
			//				sizeof(GLfloat) * Mesh->GetVertexNum() * 3, Mesh->GetVertexData());
			if (VertexBuffer) {
				D3D11_MAPPED_SUBRESOURCE subData;
				ID3D11DeviceContext* context = ((D3D11Renderer*)(RenderEngine::GetInstance()))->GetDeviceContext();
				context->Map(VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subData);

				memcpy(subData.pData, Mesh->GetVertexData(), Mesh->GetNumberOfVertex() * sizeof(Mesh::Vertex));

				context->Unmap(VertexBuffer, 0);
			} 
		}

		void Destroy()
		{
			SafeRelease(VertexBuffer);
		}
	};

}

#endif