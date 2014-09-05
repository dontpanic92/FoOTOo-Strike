#ifndef __AGE_MESH_HEADER__
#define __AGE_MESH_HEADER__

//#include "../3rd/glew-1.10.0/include/GL/glew.h"
//#include "../3rd/glew-1.10.0/include/GL/wglew.h"
#include "Math.h"
#include "Texture2D.h"
#include <GL\glew.h>
#include <vector>

namespace AGE{

	class Material
	{
	public:
		Material();

		~Material();

		void SetTexture(Texture2D* texture) { mTexture = texture; }

		void Use();

	private:
		Texture2D* mTexture;
	};

	class Mesh
	{
	public:

		Mesh(int vertexNum);

		Mesh(GLfloat* vertexArray, GLfloat* normalArray, GLfloat* textureArray, int vertexNum);

		~Mesh();

		void SetData(GLfloat* vertexArray, GLfloat* normalArray, GLfloat* textureArray);

		void SetVertex(uint nface, GLfloat* vertex9);

		void SetNormal(uint nface, GLfloat* normal9);

		void SetTexCoord(uint nface, GLfloat* texCoord6);

		const GLfloat* GetVertexData() const{ return mVertexData;}
		GLfloat* GetVertexData() { return mVertexData; }

		const GLfloat* GetNormalData() const{ return mNormalData; }
		GLfloat* GetNormalData() { return mNormalData; }

		const GLfloat* GetTextureData() const{ return mTextureData; }
		GLfloat* GetTextureData() { return mTextureData; }

		int GetVertexNum() const{ return mVertexNum; }

		//void SetMaterial(Material* material){ mMaterial = material; }

		//Material* GetMaterial() const { return mMaterial; }

	private:
		int mVertexNum;
		int mIndexNum;

		GLfloat* mVertexData;
		GLfloat* mNormalData;
		GLfloat* mTextureData;
		//GLushort* mIndexData;
	};
}
#endif
