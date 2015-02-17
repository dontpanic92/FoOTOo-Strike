#include "Mesh.h"
#include "Renderable.h"
using namespace AGE;

Material::Material()
{
}

Material::~Material()
{
}

Mesh::Mesh(Renderable* parent) : mVertexData(0), mBindposeVertexData(0)
{
	//mVertexNum = vertexNum;

	//mNormalData = new float[vertexNum * 3];
	//mTextureData = new float[vertexNum * 2];

	mParent = parent;
}

void Mesh::SetVertexData(Vertex* vertex, uint vertexNum)
{

	delete[] mVertexData;
	delete[] mBindposeVertexData;

	mVertexNum = vertexNum;
	mVertexData = new Vertex[vertexNum];
	mBindposeVertexData = new Vertex[vertexNum];
	mSkeletonData = new SkeletonData[vertexNum];
	//mIndexData = new ushort[vertexNum];

	memcpy(mVertexData, vertex, sizeof(Vertex) * vertexNum);
	memcpy(mBindposeVertexData, vertex, sizeof(Vertex) * vertexNum);
}

/*Mesh::Mesh(ushort* indexArray, float* normalArray, float* textureArray, int vertexNum, Renderable* parent)
{
	mVertexNum = vertexNum;
	mIndexData = new ushort[vertexNum];
	mVertexData = new Vertex[vertexNum * 3];
	//mNormalData = new float[vertexNum * 3];
	//mTextureData = new float[vertexNum * 2];

	memcpy(mIndexData, indexArray, sizeof(ushort) * vertexNum);
	//memcpy(mTextureData, textureArray, sizeof(float) * 2 * vertexNum);
	//memcpy(mNormalData, normalArray, sizeof(float) * 3 * vertexNum);

	mParent = parent;

	UpdateVertex();
}*/

Mesh::~Mesh()
{
	//delete[] mIndexData;
	delete[] mVertexData;
	delete[] mBindposeVertexData;
	delete[] mSkeletonData;
	//delete[] mNormalData;
	//delete[] mTextureData;

	//delete mMaterial; // This should be done by ResMan
}


/*void Mesh::SetData(ushort* indexArray, float* normalArray, float* textureArray)
{
	if (indexArray) {
		memcpy(mIndexData, indexArray, sizeof(ushort) * mVertexNum);
	}

	if (normalArray) {
		memcpy(mNormalData, normalArray, sizeof(float) * 3 * mVertexNum);
	}

	if (textureArray) {
		memcpy(mTextureData, textureArray, sizeof(float) * 2 * mVertexNum);
	}

	UpdateVertex();
}
*/

/*void Mesh::UpdateVertex()
{
	for (int i = 0; i < mVertexNum; i++) {
		mVertexData[i * 3] = mParent->GetVertex()[mIndexData[i] * 3];
		mVertexData[i * 3 + 1] = mParent->GetVertex()[mIndexData[i] * 3 + 1];
		mVertexData[i * 3 + 2] = mParent->GetVertex()[mIndexData[i] * 3 + 2];
	}
}
*/
/*void Mesh::SetVertex(uint nvertex, GLfloat* vertex3) {
	memcpy(&mVertexData[nvertex * 3], vertex3, sizeof(GLfloat) * 3);
	}*/

/*void Mesh::SetIndexByFace(uint nface, ushort* index3)
{
	memcpy(&mIndexData[nface * 3], index3, sizeof(ushort) * 3);
}

void Mesh::SetTexCoordByFace(uint nface, float* texCoord9)
{
	memcpy(&mTextureData[nface * 6], texCoord9, sizeof(float) * 6);
}

void Mesh::SetNormalByFace(uint nface, float* normal9)
{
	memcpy(&mNormalData[nface * 9], normal9, sizeof(float) * 9);
}
*/
