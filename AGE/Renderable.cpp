#include "Renderable.h"
#include "Primitive.h"
using namespace AGE;

Renderable::Renderable() : /*mVertex(0), mVertexBindPose(0), */mSkeleton(0)
{

}

Renderable::~Renderable()
{

	for (uint i = 0; i < mRenderObjects.size(); i++) {
		delete mRenderObjects[i];
	}

	//delete[] mVertex;
	//delete[] mVertexBindPose;
}

void Renderable::AddRenderObject(RenderObject* object)
{
	mRenderObjects.push_back(object);
}

/*void Renderable::SetVertexData(GLfloat* vertex, uint vertexNum)
{

	//if (mVertex)
	//delete[] mVertex;

	//if (mVertexBindPose)
	//delete[] mVertexBindPose;

	//mVertex = new Mesh::Vertex[vertexNum];
	//mVertexBindPose = new Mesh::Vertex[vertexNum];
	//memcpy(mVertex, vertex, sizeof(GLfloat) * vertexNum * 3);
	//memcpy(mVertexBindPose, vertex, sizeof(GLfloat) * vertexNum * 3);

	//if (mVertexBufferObject)
	//	glDeleteBuffers(1, &mVertexBufferObject);

	//glGenBuffers(1, &mVertexBufferObject);
	//glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexNum * 3, mVertex, GL_STATIC_DRAW);


	//mVertexNum = vertexNum;
}*/


void Renderable::UpdateSkinnedVertex()
{
	/*const Matrix4x4f* matrix = mSkeleton->GetVertexTransform();
	for (uint i = 0; i < mVertexNum; i++) {
		Vector3f v(&mVertexBindPose[i * 3]);
		v = v * matrix[i];
		mVertex[i * 3] = v[0];
		mVertex[i * 3 + 1] = v[1];
		mVertex[i * 3 + 2] = v[2];
	}

	for (uint i = 0; i < mRenderObjects.size(); i++) {
		mRenderObjects[i]->Update();
	}*/
}
