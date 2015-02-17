#ifndef __AGE_RENDERABLE_HEADER__
#define __AGE_RENDERABLE_HEADER__

#include "Attachable.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Skeleton.h"

namespace AGE
{

	class RenderObject
	{
	public:
		Renderable* Parent;
		Mesh* Mesh;
		Shader* Shader;
		Material* Material;

		virtual void Update() {}
		virtual void Destroy() {}
		virtual ~RenderObject() { Destroy(); }
	};

	class Renderable : public Attachable
	{
	public:

		Renderable();
		virtual ~Renderable();

		//void SetMesh(Mesh* mesh){ mMesh = mesh; BuildObjects();}

		void AddRenderObject(RenderObject* object);

		//void SetVertexData(float* vertex, uint vertexNum);

		void SetSkeleton(Skeleton* skeleton) { mSkeleton = skeleton; skeleton->SetRenderable(this); }

		Skeleton* GetSkeleton() { return mSkeleton; }

		//GLuint GetVertexBufferObject() const { return mVertexBufferObject; }
		//Mesh::Vertex* GetVertex() { return mVertex; }
		//uint GetNumberOfVertex() const { return mVertexNum; }

		uint GetNumberOfRenderObjects() const { return mRenderObjects.size(); }

		RenderObject* GetRenderObject(uint i) { return mRenderObjects[i]; }

		uint GetNumberOfVertex() { return mNumberOfVertex; }

		void UpdateSkinnedVertex();

		std::vector<RenderObject*>::const_iterator begin() const { return mRenderObjects.begin(); }
		std::vector<RenderObject*>::const_iterator end() const { return mRenderObjects.end(); }

	private:


		//Mesh::Vertex* mVertex;
		//Mesh::Vertex* mVertexBindPose;
		uint mNumberOfVertex;

		std::vector<RenderObject*> mRenderObjects;
		Skeleton* mSkeleton;
	};
}

#endif