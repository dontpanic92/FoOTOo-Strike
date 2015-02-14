#ifndef __AGE_RENDERABLE_HEADER__
#define __AGE_RENDERABLE_HEADER__

#include "Attachable.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture2D.h"
#include "Skeleton.h"

namespace AGE
{

	class RenderObject
	{
	public:
		Renderable* Parent;
		Mesh* Mesh;
		Material* Material;

		virtual void Update() { Mesh->UpdateVertex(); }
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

		void SetVertexData(float* vertex, uint vertexNum);

		void SetSkeleton(Skeleton* skeleton) { mSkeleton = skeleton; skeleton->SetRenderable(this); }

		Skeleton* GetSkeleton() { return mSkeleton; }

		//GLuint GetVertexBufferObject() const { return mVertexBufferObject; }
		float* GetVertex() { return mVertex; }
		uint GetNumberOfVertex() const { return mVertexNum; }

		uint GetNumberOfRenderObjects() const { return mRenderObjects.size(); }
		const RenderObject* GetRenderObject(uint i) const { return mRenderObjects[i]; }

		void UpdateSkinnedVertex();

		std::vector<RenderObject*>::const_iterator begin() const { return mRenderObjects.begin(); }
		std::vector<RenderObject*>::const_iterator end() const { return mRenderObjects.end(); }

	private:


		float* mVertex;
		float* mVertexBindPose;
		uint mVertexNum;

		std::vector<RenderObject*> mRenderObjects;
		Skeleton* mSkeleton;
	};
}

#endif