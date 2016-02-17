#include "Primitive.h"
#include "Shader.h"
#include <GLTools.h>
using namespace AGE;

Primitive::MeshBuilder::MeshBuilder()
{

}

Primitive::MeshBuilder::MeshBuilder(const Mesh::Vertex* base)
{
	mBase = base;
}

void Primitive::MeshBuilder::AddFace(Mesh::Vertex a, Mesh::Vertex b, Mesh::Vertex c)
{
	mVertex.push_back(a);
	mVertex.push_back(b);
	mVertex.push_back(c);
}
void Primitive::MeshBuilder::AddFaceByIndex(uint a, uint b, uint c)
{
	mVertex.push_back(mBase[a]);
	mVertex.push_back(mBase[b]);
	mVertex.push_back(mBase[c]);
}

Mesh* Primitive::MeshBuilder::BuildMesh()
{
	return new Mesh(&mVertex[0], mVertex.size());
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

Mesh* Primitive::CreateSphereUnmanage(float radius)
{
	//http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html

	float phi = (1 + 2.236067) / 2 * radius;
	auto func = [](Mesh::Vertex& v, float x, float y, float z){
		v.SetNormal(x, y, z);
		v.SetPosition(x, y, z);
		v.SetTexCoord(0, 0);
	};

	Mesh::Vertex vertex[12];
	func(vertex[0], -radius, phi, 0); 
	func(vertex[1], radius, phi, 0);
	func(vertex[2], -radius, -phi, 0);
	func(vertex[3], radius, -phi, 0);

	func(vertex[4], 0, -radius, phi);
	func(vertex[5], 0, radius, phi);
	func(vertex[6], 0, -radius, -phi);
	func(vertex[7], 0, radius, -phi);

	func(vertex[8], phi, 0, -radius);
	func(vertex[9], phi, 0, radius);
	func(vertex[10], -phi, 0, -radius);
	func(vertex[11], -phi, 0, radius);


	MeshBuilder builder(vertex);

	builder.AddFaceByIndex(0, 11, 5);
	builder.AddFaceByIndex(0, 5, 1);
	builder.AddFaceByIndex(0, 1, 7);
	builder.AddFaceByIndex(0, 7, 10);
	builder.AddFaceByIndex(0, 10, 11);

	// 5 adjacent faces
	builder.AddFaceByIndex(1, 5, 9);
	builder.AddFaceByIndex(5, 11, 4);
	builder.AddFaceByIndex(11, 10, 2);
	builder.AddFaceByIndex(10, 7, 6);
	builder.AddFaceByIndex(7, 1, 8);

	// 5 faces around point 3
	builder.AddFaceByIndex(3, 9, 4);
	builder.AddFaceByIndex(3, 4, 2);
	builder.AddFaceByIndex(3, 2, 6);
	builder.AddFaceByIndex(3, 6, 8);
	builder.AddFaceByIndex(3, 8, 9);

	// 5 adjacent faces
	builder.AddFaceByIndex(4, 9, 5);
	builder.AddFaceByIndex(2, 4, 11);
	builder.AddFaceByIndex(6, 2, 10);
	builder.AddFaceByIndex(8, 6, 7);
	builder.AddFaceByIndex(9, 8, 1);

	return builder.BuildMesh();
}
