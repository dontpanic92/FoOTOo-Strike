#ifndef __AGE_RENDERABLE_HEADER__
#define __AGE_RENDERABLE_HEADER__

#include "Attachable.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture2D.h"
#include "Skeleton.h"

namespace AGE
{

	class Renderable : public Attachable
	{
	public:
		struct RenderObject
		{
			Mesh* Mesh;
			Material* Material;
			GLuint VertexArrayBufferObject;
			GLuint BufferObjects[3];
		};

		Renderable();
		virtual ~Renderable();

		//void SetMesh(Mesh* mesh){ mMesh = mesh; BuildObjects();}

		void AddRenderObject(Mesh* mesh, Material* material);

		void SetVertexData(GLfloat* vertex, uint vertexNum);

		void SetSkeleton(Skeleton* skeleton) { mSkeleton = skeleton; skeleton->SetRenderable(this); }

		Skeleton* GetSkeleton() { return mSkeleton; }

		void SetShader(ShaderProgram* shader) { mShader = shader; }

		ShaderProgram* GetShader() { return mShader; }

		//void SetMaterial(Material* material){ mMaterial = material; }

		//Mesh* GetMesh(){ return mMesh; }

		//Material* GetMaterial(){ return mMaterial; }
		GLuint GetVertexBufferObject() { return mVertexBufferObject; }
		GLfloat* GetVertex() { return mVertex; }
		uint GetNumberOfVertex() { return mVertexNum; }

		uint GetNumberOfRenderObjects() { return mRenderObjects.size(); }
		const RenderObject* GetRenderObject(uint i) const { return &mRenderObjects[i]; }

		void UpdateSkinnedVertex();
			
	private:

		void BuildObjects(RenderObject& object);

		void BuildVertexBuffer();

		GLuint mVertexBufferObject;
		GLfloat* mVertex;
		GLfloat* mVertexBindPose;
		uint mVertexNum;

		std::vector<RenderObject> mRenderObjects;
		ShaderProgram* mShader;
		Skeleton* mSkeleton;
	};
}

#endif