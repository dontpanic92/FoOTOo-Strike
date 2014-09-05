#include "Mesh.h"
#include "Texture2D.h"

using namespace AGE;

Material::Material(){
}

Material::~Material(){
}


void Material::Use(){
	mTexture->Use();
}

Mesh::Mesh(int vertexNum){
	mVertexNum = vertexNum;

	mVertexData = new GLfloat[vertexNum * 3];
	mNormalData = new GLfloat[vertexNum * 3];
	mTextureData = new GLfloat[vertexNum * 2];
}

Mesh::Mesh(GLfloat* vertexArray, GLfloat* normalArray, GLfloat* textureArray, int vertexNum){
	mVertexNum = vertexNum;
	mVertexData = new GLfloat[vertexNum * 3];
	mNormalData = new GLfloat[vertexNum * 3];
	mTextureData = new GLfloat[vertexNum * 2];

	memcpy(mVertexData, vertexArray, sizeof(GLfloat) * 3 * vertexNum);
	memcpy(mTextureData, textureArray, sizeof(GLfloat) * 2 * vertexNum);
	memcpy(mNormalData, normalArray, sizeof(GLfloat) * 3 * vertexNum);
}

Mesh::~Mesh(){
	delete[] mVertexData;
	delete[] mNormalData;
	delete[] mTextureData;

	//delete mMaterial; // This should be done by ResMan
}


void Mesh::SetData(GLfloat* vertexArray, GLfloat* normalArray, GLfloat* textureArray){
	if (vertexArray){
		memcpy(mVertexData, vertexArray, sizeof(GLfloat) * 3 * mVertexNum);
	}

	if (normalArray){
		memcpy(mNormalData, normalArray, sizeof(GLfloat) * 3 * mVertexNum);
	}

	if (textureArray){
		memcpy(mTextureData, textureArray, sizeof(GLfloat) * 2 * mVertexNum);
	}
}

void Mesh::SetVertex(uint nface, GLfloat* vertex9){
	memcpy(&mVertexData[nface * 9], vertex9, sizeof(GLfloat) * 9);
}

void Mesh::SetTexCoord(uint nface, GLfloat* texCoord9){
	memcpy(&mTextureData[nface * 6], texCoord9, sizeof(GLfloat) * 6);
}

void Mesh::SetNormal(uint nface, GLfloat* normal9){
	memcpy(&mNormalData[nface * 9], normal9, sizeof(GLfloat) * 9);
}

