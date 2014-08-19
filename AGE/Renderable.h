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
			GLuint BufferObjects[4];
		};

		Renderable();
		virtual ~Renderable();

		void SetMesh(Mesh* mesh){ mMesh = mesh; BuildObjects();}

		void SetShader(ShaderProgram* shader){ mShader = shader; }

		void SetTexture(Texture2D* texture){ mTexture = texture; }

		Mesh* GetMesh(){ return mMesh; }

		ShaderProgram* GetShader(){ return mShader; }

		Texture2D* GetTexture(){ return mTexture; }

		const OpenGLRenderData* GetRenderData() const{ return &mOpenGLRenderData; }
			
	private:

		void BuildObjects();

		Mesh* mMesh;
		ShaderProgram* mShader;
		Texture2D* mTexture;

		OpenGLRenderData mOpenGLRenderData;
	};
}

#endif