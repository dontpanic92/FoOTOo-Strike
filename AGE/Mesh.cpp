#include "Mesh.h"
#include "Texture2D.h"

using namespace AGE;

Material::Material(uint indexNum){
	mIndexNum = indexNum;
	mIndexData = new GLushort[indexNum];
}

Material::~Material(){
	glDeleteBuffers(1, &mIndexBufferObject);
	delete[] mIndexData;
}

void Material::BuildObject(){
	glGenBuffers(1, &mIndexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * mIndexNum, mIndexData, GL_STATIC_DRAW);
}

void Material::Use(){
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferObject);
	mTexture->Use();
}

Mesh::Mesh(int vertexNum, int indexNum){
	mVertexNum = vertexNum;
	mIndexNum = indexNum;

	mVertexData = new GLfloat[vertexNum * 3];
	mIndexData = new GLushort[indexNum];
	mNormalData = new GLfloat[vertexNum * 3];
	mTextureData = new GLfloat[vertexNum * 2];
}

Mesh::Mesh(GLfloat* vertexArray, GLfloat* normalArray, GLfloat* textureArray, int vertexNum, GLushort* indexArray, int indexNum){
	mVertexNum = vertexNum;
	mIndexNum = indexNum;
	/*GLfloat vVerts[] = {-0.5f, -0.5f, 0.0f, 
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5, 0.5, 0.0f};
	GLushort pIndexes[] = {0, 1, 2, 0, 2, 3};
	*/
	mVertexData = new GLfloat[vertexNum * 3];
	mIndexData = new GLushort[indexNum];
	mNormalData = new GLfloat[vertexNum * 3];
	mTextureData = new GLfloat[vertexNum * 2];

	memcpy(mVertexData, vertexArray, sizeof(GLfloat) * 3 * vertexNum);
	memcpy(mTextureData, textureArray, sizeof(GLfloat) * 2 * vertexNum);
	memcpy(mIndexData, indexArray, sizeof(GLushort) * indexNum);
	memcpy(mNormalData, normalArray, sizeof(GLfloat) * 3 * vertexNum);
}

Mesh::~Mesh(){
	delete[] mVertexData;
	delete[] mIndexData;
	delete[] mNormalData;
	delete[] mTextureData;

	for (uint i = 0; i < mMaterials.size(); i++){
		delete mMaterials[i];
	}
}


void Material::AddIndex(uint nface, ushort* index3){
	mIndexData[nface * 3] = index3[0];
	mIndexData[nface * 3 + 1] = index3[1];
	mIndexData[nface * 3 + 2] = index3[2];
}

void Mesh::SetData(GLfloat* vertexArray, GLfloat* normalArray, GLfloat* textureArray, GLushort* indexArray){
	if (vertexArray){
		memcpy(mVertexData, vertexArray, sizeof(GLfloat) * 3 * mVertexNum);
	}

	if (normalArray){
		memcpy(mNormalData, normalArray, sizeof(GLfloat) * 3 * mVertexNum);
	}

	if (textureArray){
		memcpy(mTextureData, textureArray, sizeof(GLfloat) * 2 * mVertexNum);
	}

	if (indexArray){
		memcpy(mIndexData, indexArray, sizeof(GLushort) * mIndexNum);
	}
}

void Mesh::AddVertex(uint nface, GLfloat* vertex9){
	memcpy(&mVertexData[nface * 9], vertex9, sizeof(GLfloat) * 9);
}

void Mesh::AddTexCoord(uint nface, GLfloat* texCoord9){
	memcpy(&mTextureData[nface * 6], texCoord9, sizeof(GLfloat) * 6);
}

void Mesh::AddNormal(uint nface, GLfloat* normal9){
	memcpy(&mNormalData[nface * 9], normal9, sizeof(GLfloat) * 9);
}