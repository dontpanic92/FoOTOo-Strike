#include "Mesh.h"

using namespace AGE;

Mesh::Mesh(GLfloat* vertexArray, int vertexNum, GLushort* indexArray, int indexNum){
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

	memcpy(mVertexData, vertexArray, sizeof(GLfloat) * 3 * vertexNum);
	memcpy(mIndexData, indexArray, sizeof(GLushort) * indexNum);
}

Mesh::~Mesh(){
	delete[] mVertexData;
	delete[] mIndexData;
}
