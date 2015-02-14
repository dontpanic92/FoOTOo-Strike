#ifndef __AGE_MESH_HEADER__
#define __AGE_MESH_HEADER__

#include "Math.h"
#include "Texture2D.h"
#include "Shader.h"
#include <vector>

namespace AGE
{

	class Material
	{
	public:
		Material();

		~Material();

		void SetTexture(Texture2D* texture) { mTexture = texture; }

		void SetShader(Shader* shader) { mShader = shader; }

		Shader* GetShader() const { return mShader; }

		void Use() const;


	private:
		Texture2D* mTexture;
		Shader* mShader;
	};

	class Renderable;
	class Mesh
	{
	public:

		Mesh(int vertexNum, Renderable* parent);

		//Mesh(GLfloat* vertexArray, GLfloat* normalArray, GLfloat* textureArray, int vertexNum);
		Mesh(ushort* indexArray, float* normalArray, float* textureArray, int vertexNum, Renderable* parent);

		~Mesh();

		void SetData(ushort* indexArray, float* normalArray, float* textureArray);

		//void SetVertexByFace(uint nface, GLfloat* vertex9);
		//void SetVertex(uint nvertex, GLfloat* vertex3);

		void SetIndexByFace(uint nface, ushort* index3);

		void SetNormalByFace(uint nface, float* normal9);

		void SetTexCoordByFace(uint nface, float* texCoord6);

		const float* GetVertexData() const { return mVertexData; }
		float* GetVertexData() { return mVertexData; }

		const ushort* GetIndexData() const { return mIndexData; }
		ushort* GetIndexData() { return mIndexData; }

		const float* GetNormalData() const { return mNormalData; }
		float* GetNormalData() { return mNormalData; }

		const float* GetTextureData() const { return mTextureData; }
		float* GetTextureData() { return mTextureData; }

		int GetNumberOfVertex() const { return mVertexNum; }

		void UpdateVertex();

		//void SetMaterial(Material* material){ mMaterial = material; }

		//Material* GetMaterial() const { return mMaterial; }

	private:

		int mVertexNum;
		int mIndexNum;

		Renderable* mParent;

		float* mVertexData;
		ushort* mIndexData;
		float* mNormalData;
		float* mTextureData;
		//GLushort* mIndexData;
	};
}
#endif
