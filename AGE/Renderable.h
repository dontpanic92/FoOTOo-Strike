#ifndef __AGE_RENDERABLE_HEADER__
#define __AGE_RENDERABLE_HEADER__

#include "Attachable.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "Skeleton.h"

namespace AGE
{

	struct RenderObject
	{
		Renderable* Parent;
		Mesh* Mesh;
		Material* Material;
		GLuint VertexArrayBufferObject;
		GLuint BufferObjects[3];
	};

	class Renderable : public Attachable
	{
	public:

		Renderable();
		virtual ~Renderable();

		//void SetMesh(Mesh* mesh){ mMesh = mesh; BuildObjects();}

		void AddRenderObject(Mesh* mesh, Material* material);

		void SetVertexData(GLfloat* vertex, uint vertexNum);

		void SetSkeleton(Skeleton* skeleton) { mSkeleton = skeleton; skeleton->SetRenderable(this); }

		Skeleton* GetSkeleton() { return mSkeleton; }

		//void SetShader(ShaderProgram* shader) { mShader = shader; }

		//ShaderProgram* GetShader() { return mShader; }

		//void SetMaterial(Material* material){ mMaterial = material; }

		//Mesh* GetMesh(){ return mMesh; }

		//Material* GetMaterial(){ return mMaterial; }
		GLuint GetVertexBufferObject() const { return mVertexBufferObject; }
		GLfloat* GetVertex() { return mVertex; }
		uint GetNumberOfVertex() const { return mVertexNum; }

		uint GetNumberOfRenderObjects() const { return mRenderObjects.size(); }
		const RenderObject* GetRenderObject(uint i) const { return &mRenderObjects[i]; }

		void UpdateSkinnedVertex();

		std::vector<RenderObject>::const_iterator begin() const { return mRenderObjects.begin(); }
		std::vector<RenderObject>::const_iterator end() const { return mRenderObjects.end(); }
			
	private:

		void BuildObjects(RenderObject& object);

		void BuildVertexBuffer();

		GLuint mVertexBufferObject;
		GLfloat* mVertex;
		GLfloat* mVertexBindPose;
		uint mVertexNum;

		std::vector<RenderObject> mRenderObjects;
		Skeleton* mSkeleton;
	};
}

#endif