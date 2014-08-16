#ifndef __AGE_RENDERABLE_HEADER__
#define __AGE_RENDERABLE_HEADER__

#include "Attachable.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include <GLTools.h>
namespace AGE
{

	class Renderable : public Attachable
	{
	public:
		struct OpenGLRenderData
		{
			GLuint VertexArrayBufferObject;
			GLuint BufferObjects[4];
		};

		Renderable();
		virtual ~Renderable();

		void SetMesh(Mesh* mesh){ mMesh = mesh; gltMakeTorus(torusBatch, 0.4f, 0.15f, 30, 30); BuildObjects();}

		void SetShader(ShaderProgram* shader){ mShader = shader; }

		Mesh* GetMesh(){ return mMesh; }

		ShaderProgram* GetShader(){ return mShader; }

		const OpenGLRenderData* GetRenderData() const{ return &mOpenGLRenderData; }
			
	private:

		void BuildObjects();

		Mesh* mMesh;
		ShaderProgram* mShader;

		GLTriangleBatch torusBatch;

		OpenGLRenderData mOpenGLRenderData;
	};
}

#endif