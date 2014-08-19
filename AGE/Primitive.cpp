#include "Primitive.h"
#include "ShaderProgram.h"
#include <GLTools.h>
using namespace AGE;

Renderable* Primitive::CreateTorusUnmanage(){
	GLTriangleBatch torusBatch;
	gltMakeTorus(torusBatch, 0.4f, 0.15f, 30, 30);

	Mesh* mesh = new Mesh((GLfloat*)torusBatch.pVerts, (GLfloat*)torusBatch.pNorms, (GLfloat*)torusBatch.pTexCoords, torusBatch.nNumVerts, torusBatch.pIndexes, torusBatch.nNumIndexes);
	
	ShaderProgram* shader = new ShaderProgram();
	shader->LoadAndCompile("../Resources/Shaders/GLSL/DefaultLight.vshader", "../Resources/Shaders/GLSL/DefaultLight.fshader");

	Texture2D* texture2 = new Texture2D();
	texture2->Load("../Resources/Textures/Marslike.tga");

	Renderable* newRenderable = new Renderable();
	newRenderable->SetMesh(mesh);
	newRenderable->SetShader(shader);
	newRenderable->SetTexture(texture2);
	return newRenderable;
}

Renderable* Primitive::CreatePlaneUnmanage(Vector3f points[4]){
	GLfloat vertex[12];
	GLfloat normal[12];
	
	for(uint i = 0; i < 4; i++){
		for(uint j = 0; j < 3; j++){
			vertex[i * 3 + j] = points[i][j];
		}
		normal[i * 3] = 0.0f;
		normal[i * 3 + 1] = 1.0f;
		normal[i * 3 + 2] = 0.0f;
	}

	GLfloat texture[] = {0, 0, 1, 0, 1, 1, 0, 1};

	GLushort index[6] = {0, 1, 2, 0, 2, 3};

	Mesh* mesh = new Mesh(vertex, normal, texture, 4, index, 6);
	
	ShaderProgram* shader = new ShaderProgram();
	shader->LoadAndCompile("../Resources/Shaders/GLSL/DefaultLight.vshader", "../Resources/Shaders/GLSL/DefaultLight.fshader");

	Texture2D* texture2 = new Texture2D();
	texture2->Load("../Resources/Textures/Marble.tga");

	Renderable* newRenderable = new Renderable();
	newRenderable->SetMesh(mesh);
	newRenderable->SetShader(shader);
	newRenderable->SetTexture(texture2);
	return newRenderable;
}