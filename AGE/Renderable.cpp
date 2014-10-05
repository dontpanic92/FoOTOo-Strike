#include "Renderable.h"
#include "Primitive.h"
using namespace AGE;

Renderable::Renderable() :mShader(0), mVertexBufferObject(0), mVertex(0), mVertexBindPose(0), mSkeleton(0) {

}

Renderable::~Renderable(){
	///if(mMesh)
		//delete mMesh;
	//if(mShader)
		//delete mShader;
	//if(mTexture)
		//delete mTexture;

	for (uint i = 0; i < mRenderObjects.size(); i++) {
		glDeleteBuffers(3, mRenderObjects[i].BufferObjects);
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &mRenderObjects[i].VertexArrayBufferObject);
	}

	glDeleteBuffers(1, &mVertexBufferObject);

	delete[] mVertex;
	delete[] mVertexBindPose;
}

void Renderable::AddRenderObject(Mesh* mesh, Material* material) {
	RenderObject object;
	object.Mesh = mesh;
	object.Material = material;

	BuildObjects(object);

	mRenderObjects.push_back(object);
}

void Renderable::SetVertexData(GLfloat* vertex, uint vertexNum) {

	//if (mVertex)
	delete[] mVertex;

	//if (mVertexBindPose)
	delete[] mVertexBindPose;

	mVertex = new GLfloat[vertexNum * 3];
	mVertexBindPose = new GLfloat[vertexNum * 3];
	memcpy(mVertex, vertex, sizeof(GLfloat) * vertexNum * 3);
	memcpy(mVertexBindPose, vertex, sizeof(GLfloat) * vertexNum * 3);

	if (mVertexBufferObject)
		glDeleteBuffers(1, &mVertexBufferObject);

	/*glGenBuffers(1, &mVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexNum * 3, mVertex, GL_STATIC_DRAW);
	*/

	mVertexNum = vertexNum;
}

void Renderable::BuildObjects(RenderObject& object) {
	glGenVertexArrays(1, &object.VertexArrayBufferObject);
	glBindVertexArray(object.VertexArrayBufferObject);

	glGenBuffers(3, object.BufferObjects);

	glBindBuffer(GL_ARRAY_BUFFER, object.BufferObjects[0]);
	glEnableVertexAttribArray(0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*object.Mesh->GetVertexNum() * 3, object.Mesh->GetVertexData(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	

	glBindBuffer(GL_ARRAY_BUFFER, object.BufferObjects[1]);
	glEnableVertexAttribArray(1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*object.Mesh->GetVertexNum() * 3, object.Mesh->GetNormalData(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, object.BufferObjects[2]);
	glEnableVertexAttribArray(2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*object.Mesh->GetVertexNum() * 2, object.Mesh->GetTextureData(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Indices
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.BufferObjects[2]);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*object.Mesh->GetVertexNum(), object.Mesh->GetIndexData(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Renderable::UpdateSkinnedVertex() {
	const Matrix4x4f* matrix = mSkeleton->GetVertexTransform();
	for (uint i = 0; i < mVertexNum; i++) {
		Vector3f v(&mVertexBindPose[i * 3]);
		v = v * matrix[i];
		mVertex[i * 3] = v[0];
		mVertex[i * 3 + 1] = v[1];
		mVertex[i * 3 + 2] = v[2];
	}

	for (uint i = 0; i < mRenderObjects.size(); i++) {
		mRenderObjects[i].Mesh->UpdateVertex();
		glBindBuffer(GL_ARRAY_BUFFER, mRenderObjects[i].BufferObjects[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0,
			sizeof(GLfloat) * mRenderObjects[i].Mesh->GetVertexNum() * 3, mRenderObjects[i].Mesh->GetVertexData());
		
	}
}
