#include "Renderable.h"
#include "Primitive.h"
using namespace AGE;

Renderable::Renderable() : mMesh(0), mShader(0), mTexture(0){

}

Renderable::~Renderable(){
	if(mMesh)
		delete mMesh;
	if(mShader)
		delete mShader;
	if(mTexture)
		delete mTexture;

	if(mOpenGLRenderData.VertexArrayBufferObject){
		glDeleteBuffers(4, mOpenGLRenderData.BufferObjects);
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &mOpenGLRenderData.VertexArrayBufferObject);
	}
}


void Renderable::BuildObjects(){
	glGenVertexArrays(1, &mOpenGLRenderData.VertexArrayBufferObject);
	glBindVertexArray(mOpenGLRenderData.VertexArrayBufferObject);

	glGenBuffers(4, mOpenGLRenderData.BufferObjects);

	glBindBuffer(GL_ARRAY_BUFFER, mOpenGLRenderData.BufferObjects[0]);
	glEnableVertexAttribArray(0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*mMesh->GetVertexNum()*3, mMesh->GetVertexData(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, mOpenGLRenderData.BufferObjects[1]);
	glEnableVertexAttribArray(1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*mMesh->GetVertexNum()*3, mMesh->GetNormalData(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Texture coordinates
	glBindBuffer(GL_ARRAY_BUFFER, mOpenGLRenderData.BufferObjects[2]);
	glEnableVertexAttribArray(2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*mMesh->GetVertexNum()*2, mMesh->GetTextureData(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Indexes
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mOpenGLRenderData.BufferObjects[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*mMesh->GetIndexNum(), mMesh->GetIndexData(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}
