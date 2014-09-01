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
		Material(uint intexNum);

		~Material();

		void AddIndex(uint nface, ushort* index3);

		const GLushort* GetIndexData() const{ return mIndexData; }

		uint GetIndexNum() const { return mIndexNum; }

		void BuildObject();

		void SetTexture(Texture2D* texture) { mTexture = texture; }

		void Use();
		

	private:
		GLushort* mIndexData;
		uint mIndexNum;
		Texture2D* mTexture;

		GLuint mIndexBufferObject;
	};

	class Mesh
	{
	public:
		typedef std::vector<Material*> MaterialCollection;

		Mesh(int vertexNum, int indexNum);

		Mesh(GLfloat* vertexArray, GLfloat* normalArray, GLfloat* textureArray, int vertexNum, GLushort* indexArray, int indexNum);

		~Mesh();

		void SetData(GLfloat* vertexArray, GLfloat* normalArray, GLfloat* textureArray, GLushort* indexArray);

		void AddVertex(uint nface, GLfloat* vertex9);
		void AddNormal(uint nface, GLfloat* normal9);
		void AddTexCoord(uint nface, GLfloat* texCoord6);

		const GLfloat* GetVertexData() const{ return mVertexData;}

		const GLushort* GetIndexData() const{ return mIndexData; }

		const GLfloat* GetNormalData() const{ return mNormalData; }

		const GLfloat* GetTextureData() const{ return mTextureData; }

		int GetVertexNum() const{ return mVertexNum; }

		int GetIndexNum() const{ return mIndexNum; }

		void AddMaterial(Material* material){ mMaterials.push_back(material); }

		const MaterialCollection& GetMaterials() const { return mMaterials; }

		void BuildMatierials(){ for (uint i = 0; i < mMaterials.size(); i++) mMaterials[i]->BuildObject(); }

	private:
		int mVertexNum;
		int mIndexNum;

		MaterialCollection mMaterials;

		GLfloat* mVertexData;
		GLfloat* mNormalData;
		GLfloat* mTextureData;
		GLushort* mIndexData;
	};

}
#endif
