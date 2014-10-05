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

	class Renderable;
	class Mesh
	{
	public:

		Mesh(int vertexNum, Renderable* parent);

		//Mesh(GLfloat* vertexArray, GLfloat* normalArray, GLfloat* textureArray, int vertexNum);
		Mesh(GLushort* indexArray, GLfloat* normalArray, GLfloat* textureArray, int vertexNum, Renderable* parent);

		~Mesh();

		void SetData(GLushort* indexArray, GLfloat* normalArray, GLfloat* textureArray);

		//void SetVertexByFace(uint nface, GLfloat* vertex9);
		//void SetVertex(uint nvertex, GLfloat* vertex3);

		void SetIndexByFace(uint nface, GLushort* index3);

		void SetNormalByFace(uint nface, GLfloat* normal9);

		void SetTexCoordByFace(uint nface, GLfloat* texCoord6);

		const GLfloat* GetVertexData() const{ return mVertexData;}
		GLfloat* GetVertexData() { return mVertexData; }

		const GLushort* GetIndexData() const { return mIndexData; }
		GLushort* GetIndexData() { return mIndexData; }

		const GLfloat* GetNormalData() const{ return mNormalData; }
		GLfloat* GetNormalData() { return mNormalData; }

		const GLfloat* GetTextureData() const{ return mTextureData; }
		GLfloat* GetTextureData() { return mTextureData; }

		int GetVertexNum() const { return mVertexNum; }

		void UpdateVertex();

		//void SetMaterial(Material* material){ mMaterial = material; }

		//Material* GetMaterial() const { return mMaterial; }

	private:

		int mVertexNum;
		int mIndexNum;

		Renderable* mParent;

		GLfloat* mVertexData;
		GLushort* mIndexData;
		GLfloat* mNormalData;
		GLfloat* mTextureData;
		//GLushort* mIndexData;
	};
}
#endif
