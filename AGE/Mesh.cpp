#include "Mesh.h"
#include "Texture2D.h"
#include "Renderable.h"
using namespace AGE;

Material::Material(){
}

Material::~Material(){
}


void Material::Use(){
	mTexture->Use();
}

Mesh::Mesh(int vertexNum, Renderable* parent) {
	mVertexNum = vertexNum;

	mVertexData = new GLfloat[vertexNum * 3];
	mIndexData = new GLushort[vertexNum];
	mNormalData = new GLfloat[vertexNum * 3];
	mTextureData = new GLfloat[vertexNum * 2];

	mParent = parent;
}

Mesh::Mesh(GLushort* indexArray, GLfloat* normalArray, GLfloat* textureArray, int vertexNum, Renderable* parent) {
	mVertexNum = vertexNum;
	mIndexData = new GLushort[vertexNum];
	mVertexData = new GLfloat[vertexNum * 3];
	mNormalData = new GLfloat[vertexNum * 3];
	mTextureData = new GLfloat[vertexNum * 2];

	memcpy(mIndexData, indexArray, sizeof(GLushort) * vertexNum);
	memcpy(mTextureData, textureArray, sizeof(GLfloat) * 2 * vertexNum);
	memcpy(mNormalData, normalArray, sizeof(GLfloat) * 3 * vertexNum);

	mParent = parent;

	UpdateVertex();
}

Mesh::~Mesh(){
	delete[] mIndexData;
	delete[] mVertexData;
	delete[] mNormalData;
	delete[] mTextureData;

	//delete mMaterial; // This should be done by ResMan
}


void Mesh::SetData(GLushort* indexArray, GLfloat* normalArray, GLfloat* textureArray){
	if (indexArray) {
		memcpy(mIndexData, indexArray, sizeof(GLushort) * mVertexNum);
	}

	if (normalArray){
		memcpy(mNormalData, normalArray, sizeof(GLfloat) * 3 * mVertexNum);
	}

	if (textureArray){
		memcpy(mTextureData, textureArray, sizeof(GLfloat) * 2 * mVertexNum);
	}
	
	UpdateVertex();
}

void Mesh::UpdateVertex() {
	for (int i = 0; i < mVertexNum; i++) {
		mVertexData[i * 3] = mParent->GetVertex()[mIndexData[i] * 3];
		mVertexData[i * 3 + 1] = mParent->GetVertex()[mIndexData[i] * 3 + 1];
		mVertexData[i * 3 + 2] = mParent->GetVertex()[mIndexData[i] * 3 + 2];
	}
}

/*void Mesh::SetVertex(uint nvertex, GLfloat* vertex3) {
	memcpy(&mVertexData[nvertex * 3], vertex3, sizeof(GLfloat) * 3);
}*/

void Mesh::SetIndexByFace(uint nface, GLushort* index3) {
	memcpy(&mIndexData[nface * 3], index3, sizeof(GLushort) * 3);
}

void Mesh::SetTexCoordByFace(uint nface, GLfloat* texCoord9) {
	memcpy(&mTextureData[nface * 6], texCoord9, sizeof(GLfloat) * 6);
}

void Mesh::SetNormalByFace(uint nface, GLfloat* normal9) {
	memcpy(&mNormalData[nface * 9], normal9, sizeof(GLfloat) * 9);
}

