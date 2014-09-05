#ifndef __AGE_RENDERABLE_HEADER__
#define __AGE_RENDERABLE_HEADER__

#include "Attachable.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture2D.h"

namespace AGE
{

	class Renderable : public Attachable
	{
	public:
		struct OpenGLRenderData
		{
			GLuint VertexArrayBufferObject;
			GLuint BufferObjects[3];
		};

		Renderable();
		virtual ~Renderable();

		void SetMesh(Mesh* mesh){ mMesh = mesh; BuildObjects();}

		void SetShader(ShaderProgram* shader){ mShader = shader; }

		void SetMaterial(Material* material){ mMaterial = material; }

		Mesh* GetMesh(){ return mMesh; }

		ShaderProgram* GetShader(){ return mShader; }

		Material* GetMaterial(){ return mMaterial; }

		const OpenGLRenderData* GetRenderData() const{ return &mOpenGLRenderData; }
			
	private:

		void BuildObjects();

		Mesh* mMesh;
		ShaderProgram* mShader;
		Material* mMaterial;

		OpenGLRenderData mOpenGLRenderData;
	};
}

#endif