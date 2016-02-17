#ifndef __AGE_MESH_HEADER__
#define __AGE_MESH_HEADER__

#include "Math.h"
#include "Texture.h"
#include "Shader.h"
#include <vector>
#include <functional>

namespace AGE
{

	class Material
	{
	public:
		Material();

		~Material();

		void SetTexture(Texture* texture) { mTexture = texture; }

		Texture* GetTexture() const { return mTexture; }


	private:
		Texture* mTexture;
	};

	class Renderable;
	class Mesh
	{
	public:

		struct Vertex
		{
			float Position[3];
			float Normal[3];
			float TextureCoord[2];

			void SetPosition(float x, float y, float z);
			void SetNormal(float x, float y, float z);
			void SetTexCoord(float u, float v);
		};

		struct SkeletonData
		{
			int BoneID[4];
			float weight[4];
		};

		Mesh();
		Mesh(Vertex* vertex, uint vertexNum);
		~Mesh();


		Vertex* GetVertexData() { return mVertexData; }
		const Vertex* GetVertexBindPose() const { return mBindposeVertexData; }

		//Vertex* GetVertexData() { return mVertexData; }
		void SetVertexData(Vertex* vertex, uint vertexNum);
		void SetVertexData(float* vertexArray, float* normalArray, float* textureCoord, uint vertexNum);
		void SetVertexData(float* vertexArray, std::function<uint(uint)> indexFunc, float* normalArray, float* textureCoord, uint vertexNum);

		void SetSkeletonData(uint i, const SkeletonData& data)
		{
			mSkeletonData[i] = data;
		}
		const SkeletonData* GetSkeletonData() { return mSkeletonData; }
		//const ushort* GetIndexData() const { return mIndexData; }
		//ushort* GetIndexData() { return mIndexData; }

		//const float* GetNormalData() const { return mNormalData; }
		//float* GetNormalData() { return mNormalData; }

		//const float* GetTextureData() const { return mTextureData; }
		//float* GetTextureData() { return mTextureData; }

		int GetNumberOfVertex() const { return mVertexNum; }

		//void UpdateVertex();

		//void SetMaterial(Material* material){ mMaterial = material; }

		//Material* GetMaterial() const { return mMaterial; }

	private:

		void SetVertexDataPrivate(std::function<void()> callback, uint vertexNum);

		int mVertexNum;
		int mIndexNum;

		Vertex* mBindposeVertexData = nullptr;
		Vertex* mVertexData = nullptr;
		SkeletonData* mSkeletonData = nullptr;
		//float* mNormalData;
		//float* mTextureData;
		//GLushort* mIndexData;
	};
}
#endif
