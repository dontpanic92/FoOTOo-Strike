#include "Primitive.h"
#include "Shader.h"
#include <GLTools.h>
using namespace AGE;

Mesh* Primitive::CreateTorusUnmanage()
{
	/*GLTriangleBatch torusBatch;
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
	return newRenderable;*/
	return NULL;
}

Mesh* Primitive::CreateRectangleUnmanage(float width, float height)
{
	float x = width / 2;
	float z = height / 2;

	float vertex[18] = { -x, 0, z, x, 0, z, -x, 0, -z, x, 0, z, x, 0, -z, -x, 0, -z };
	float normal[18] = { 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0};
	float texture[] = { 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1 };
	Mesh* mesh = new Mesh();

	mesh->SetVertexData(vertex, normal, texture, 6);

	return mesh;
}